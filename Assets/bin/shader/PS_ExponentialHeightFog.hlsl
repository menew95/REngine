#include "header\H_Const.hlsli"
#include "header\H_Input.hlsli"

cbuffer PerMaterial : register(b2)
{
    float _fogDensity;
    float _fogMaxOpacity;     // 최대 안개 수치(0~1사이의 값)
    float _fogStartDistance;  // 카메라로부터의 거리
    float _fogCutOffDistance; // 카메라로부터 안개가 끝나는 지점의 거리

    float _fogHeightFalloff; // fog 높이 감쇠. 높이에따라 밀도가 줄어드는 양을 설정합니다. 값으 작으면 변화량이 줄어들어 블렌딩이 일어나는 부분이 많아집니다(안개에 영향을 받는 영역)
    float3 _fogHeight;

    float3 _fogScatteringColor;
    float _AdaptiveCubeMap;
};

Texture2D g_PosW : register(t0);
Texture2D g_Color : register(t1);

SamplerState g_samLinear : register(s0);

static const float FLT_EPSILON2 = 0.01f;
float Pow2(float x) { return x * x; }

// UE 4.22 HeightFogCommon.ush
// Calculate the line integral of the ray from the camera to the receiver position through the fog density function
// The exponential fog density function is d = GlobalDensity * exp(-HeightFalloff * y)
float CalculateLineIntegralShared(float FogHeightFalloff, float RayDirectionY, float RayOriginTerms)
{
    float Falloff = max(-127.0f, FogHeightFalloff * RayDirectionY); // if it's lower than -127.0, then exp2() goes crazy in OpenGL's GLSL.
    float LineIntegral = (1.0f - exp2(-Falloff)) / Falloff;
    float LineIntegralTaylor = log(2.0) - (0.5 * Pow2(log(2.0))) * Falloff; // Taylor expansion around 0

    return RayOriginTerms * (abs(Falloff) > FLT_EPSILON2 ? LineIntegral : LineIntegralTaylor);
}

// viewpos = world - camPos
float4 GetExponentialHeightFog(float3 WorldPositionRelativeToCamera)
{
    const half MinFogOpacity = 0.f;

    //월드좌표계상에서의 카메라와 대상의 차이
    float3 CameraToReceiver = WorldPositionRelativeToCamera;

    float CameraToReceiverLengthSqr = dot(CameraToReceiver, CameraToReceiver);
    //rsqrt , reciprocal square root 제곱근의 역수.
    float CameraToReceiverLengthInv = rsqrt(CameraToReceiverLengthSqr); //1 / 길이
    float CameraToReceiverLength = CameraToReceiverLengthSqr * CameraToReceiverLengthInv; // 길이
    float3 CameraToReceiverNormalized = CameraToReceiver * CameraToReceiverLengthInv; // normalize

    // FogDensity * exp2(-FogHeightFalloff * (CameraWorldPosition.y - FogHeight))
    //높이에 따른 안개량을 결정한다. 카메라 높이와 fog의 높이의 길이에따라..
    float RayOriginTerms = _fogDensity * exp2(-_fogHeightFalloff * (_camera._world.y - _fogHeight.y));
    float RayLength = CameraToReceiverLength;
    float RayDirectionY = CameraToReceiver.y;

    float ExcludeDistance = _fogStartDistance;

    if (ExcludeDistance > 0)
    {
        float ExcludeIntersectionTime = ExcludeDistance * CameraToReceiverLengthInv; // 시작거리 / 길이
        float CameraToExclusionIntersectionY = ExcludeIntersectionTime * CameraToReceiver.y; // 시작거리 * y축 차이 / 길이 // 카메라 to intersection 위치
        float ExclusionIntersectionY = _camera._world.y + CameraToExclusionIntersectionY; // world 좌표에서의 Section까지의 y좌표
        float ExclusionIntersectionToReceiverY = CameraToReceiver.y - CameraToExclusionIntersectionY; // 인터섹션에서 타겟으로 

        // Calculate fog off of the ray starting from the exclusion distance, instead of starting from the camera
        RayLength = (1.0f - ExcludeIntersectionTime) * CameraToReceiverLength; //인터섹션에서 포지션으로의 거리 
        RayDirectionY = ExclusionIntersectionToReceiverY; //상동 
        
        float Exponent = max(-127.0f, _fogHeightFalloff * (ExclusionIntersectionY - _fogHeight.y));
        
        RayOriginTerms = _fogDensity * exp2(-Exponent);
        
    }

    float ExponentialHeightLineIntegralShared = CalculateLineIntegralShared(_fogHeightFalloff, RayDirectionY, RayOriginTerms);

    float ExponentialHeightLineIntegral = ExponentialHeightLineIntegralShared * RayLength;

    float3 InscatteringColor = _fogScatteringColor;
    float3 DirectionalInscattering = 0;
    
    // Calculate the amount of light that made it through the fog using the transmission equation
    float ExpFogFactor = max(saturate(exp2(-ExponentialHeightLineIntegral)), MinFogOpacity);

    // ExponentialFogParameters3.w : FogCutoffDistance
    if (_fogCutOffDistance > 0 && CameraToReceiverLength > _fogCutOffDistance)
    {
        ExpFogFactor = 1;
        DirectionalInscattering = 0;
    }

    float3 FogColor = (InscatteringColor) * (1 - ExpFogFactor) + DirectionalInscattering;

    return float4(FogColor, ExpFogFactor);

}

float4 main(VSOutput input)
        : SV_TARGET
{
    float4 posW  = g_PosW.Sample(g_samLinear,  input.uv);
    float4 color = g_Color.Sample(g_samLinear, input.uv);

    //위치값이 없는 상황입니다.
    if (posW.w <= 0.0f)
    {
        float2 screenPos = input.uv;

        screenPos.y = 1.f - screenPos.y;

        //다시 NDC공간으로 돌려놓기.
        screenPos *= 2.f;
        screenPos -= 1.f;

        // 이상황에서는 위치값이 없음. 즉 카메라영역에서의 가장 끝을 기준으로 계산합니다.
        posW = mul(float4(screenPos.xy, 1.f, 1.f), transpose(_camera._viewProjInvTranspose));
        posW.xyz /= posW.w;
    }

    float4 fogColor = GetExponentialHeightFog(posW.xyz - _camera._world);
    float4 ret = float4(color.rgb * fogColor.a + fogColor.rgb, color.a);

    return ret;
}