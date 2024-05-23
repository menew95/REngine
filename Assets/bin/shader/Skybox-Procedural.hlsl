#include "header\H_Const.hlsli"
#include "header\H_Color.hlsli"
#include "header\H_Input.hlsli"

cbuffer PerMaterial: register(b3)
{
    float _Exposure = 1.3;     // HDR exposure
    float3 _GroundColor = float3(.369, .349, .341);
    float _SunSize = 0.04;
    float _SunSizeConvergence = 5;
    float3 _SkyTint = float3(0.5f, 0.5, 0.5);
    float _AtmosphereThickness = 1.0;
}


    #define GAMMA 2.2
    // HACK: to get gfx-tests in Gamma mode to agree until UNITY_ACTIVE_COLORSPACE_IS_GAMMA is working properly
    #define COLOR_2_GAMMA(color) ((colorSpaceDouble.r>2.0) ? pow(color,1.0/GAMMA) : color)
    #define COLOR_2_LINEAR(color) color
    #define LINEAR_2_LINEAR(color) color

    // RGB wavelengths
    // .35 (.62=158), .43 (.68=174), .525 (.75=190)
    static const float3 kDefaultScatteringWavelength = float3(.65, .57, .475);
    static const float3 kVariableRangeForScatteringWavelength = float3(.15, .15, .15);

    #define OUTER_RADIUS 1.025
    static const float kOuterRadius = OUTER_RADIUS;
    static const float kOuterRadius2 = OUTER_RADIUS*OUTER_RADIUS;
    static const float kInnerRadius = 1.0;
    static const float kInnerRadius2 = 1.0;

    static const float kCameraHeight = 0.0001;

    #define kRAYLEIGH (lerp(0.0, 0.0025, pow(_AtmosphereThickness,2.5)))      // Rayleigh constant
    #define kMIE 0.0010             // Mie constant
    #define kSUN_BRIGHTNESS 20.0    // Sun brightness

    #define kMAX_SCATTER 50.0 // Maximum scattering value, to prevent math overflows on Adrenos

    static const float kHDSundiskIntensityFactor = 15.0;
    static const float kSimpleSundiskIntensityFactor = 27.0;

    static const float kSunScale = 400.0 * kSUN_BRIGHTNESS;
    static const float kKmESun = kMIE * kSUN_BRIGHTNESS;
    static const float kKm4PI = kMIE * 4.0 * 3.14159265;
    static const float kScale = 1.0 / (OUTER_RADIUS - 1.0);
    static const float kScaleDepth = 0.25;
    static const float kScaleOverScaleDepth = (1.0 / (OUTER_RADIUS - 1.0)) / 0.25;
    static const float kSamples = 2.0; // THIS IS UNROLLED MANUALLY, DON'T TOUCH

    #define MIE_G (-0.990)
    #define MIE_G2 0.9801

    #define SKY_GROUND_THRESHOLD 0.02

    // fine tuning of performance. You can override defines here if you want some specific setup
    // or keep as is and allow later code to set it according to target api

    // if set vprog will output color in final color space (instead of linear always)
    // in case of rendering in gamma mode that means that we will do lerps in gamma mode too, so there will be tiny difference around horizon
    // #define SKYBOX_COLOR_IN_TARGET_COLOR_SPACE 0

    // sun disk rendering:
    // no sun disk - the fastest option
    #define SKYBOX_SUNDISK_NONE 0
    // simplistic sun disk - without mie phase function
    #define SKYBOX_SUNDISK_SIMPLE 1
    // full calculation - uses mie phase function
    #define SKYBOX_SUNDISK_HQ 2

    // uncomment this line and change SKYBOX_SUNDISK_SIMPLE to override material settings
    #define SKYBOX_SUNDISK SKYBOX_SUNDISK_SIMPLE

#ifndef SKYBOX_SUNDISK
    #if defined(_SUNDISK_NONE)
        #define SKYBOX_SUNDISK SKYBOX_SUNDISK_NONE
    #elif defined(_SUNDISK_SIMPLE)
        #define SKYBOX_SUNDISK SKYBOX_SUNDISK_SIMPLE
    #else
        #define SKYBOX_SUNDISK SKYBOX_SUNDISK_HQ
    #endif
#endif

#ifndef SKYBOX_COLOR_IN_TARGET_COLOR_SPACE
    #if defined(SHADER_API_MOBILE)
        #define SKYBOX_COLOR_IN_TARGET_COLOR_SPACE 1
    #else
        #define SKYBOX_COLOR_IN_TARGET_COLOR_SPACE 0
    #endif
#endif

// Calculates the Rayleigh phase function

//레일리 산란
float getRayleighPhase(float eyeCos2)
{
    return 0.75 + 0.75 * eyeCos2;
}
 
float getRayleighPhase(float3 light, float3 ray)
{   
    // directional 빛의 방향과 -eye를 내적
    float eyeCos = dot(light, ray);
    return getRayleighPhase(eyeCos * eyeCos);
}


struct appdata_t
{
    float4 vertex : POSITION;
};

struct v2f
{
    float4  pos             : SV_POSITION;

#if SKYBOX_SUNDISK == SKYBOX_SUNDISK_HQ
    // for HQ sun disk, we need vertex itself to calculate ray-dir per-pixel
    float3  vertex          : TEXCOORD0;
#elif SKYBOX_SUNDISK == SKYBOX_SUNDISK_SIMPLE
    float3   rayDir          : TEXCOORD0;
#else
    // as we dont need sun disk we need just rayDir.y (sky/ground threshold)
    float    skyGroundFactor : TEXCOORD0;
#endif

    // calculate sky colors in vprog
    float3   groundColor     : TEXCOORD1;
    float3   skyColor        : TEXCOORD2;

#if SKYBOX_SUNDISK != SKYBOX_SUNDISK_NONE
    float3   sunColor        : TEXCOORD3;
#endif
};


inline float scale(float inCos)
{
    float x = 1.0 - inCos;
    return 0.25 * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

v2f vert (VSInput input)
{
    v2f OUT;

    OUT.pos = mul(mul(float4(input.posL.xyz, 1.0f), _world), _camera._viewProj);

    float3 kSkyTintInGammaSpace = COLOR_2_GAMMA(_SkyTint); // tint color를 감마 영역으로 변환
    float3 kScatteringWavelength = lerp (
        kDefaultScatteringWavelength-kVariableRangeForScatteringWavelength,
        kDefaultScatteringWavelength+kVariableRangeForScatteringWavelength,
        float3(1,1,1) - kSkyTintInGammaSpace); // using Tint in sRGB gamma allows for more visually linear interpolation and to keep (.5) at (128, gray in sRGB) point
    float3 kInvWavelength = 1.0 / pow(kScatteringWavelength, 4);

    float kKrESun = kRAYLEIGH * kSUN_BRIGHTNESS;
    float kKr4PI = kRAYLEIGH * 4.0 * 3.14159265;

    float3 cameraPos = float3(0,kInnerRadius + kCameraHeight,0);    // The camera's current position

    // Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)
    // eye vector는 그냥 posL를 사용해도 무방해 보이긴 한다. 어차피 _world는 Identity 값일테니
    float3 eyeRay = normalize(mul(input.posL.xyz, (float3x3)_world));

    float far = 0.0;
    float3 cIn, cOut;

    if(eyeRay.y >= 0.0)
    {
        // 반구 위라면 하늘이기에 대기를 계산한다.
        // Sky
        // Calculate the length of the "atmosphere"
        far = sqrt(kOuterRadius2 + kInnerRadius2 * eyeRay.y * eyeRay.y - kInnerRadius2) - kInnerRadius * eyeRay.y;

        float3 pos = cameraPos + far * eyeRay;

        // Calculate the ray's starting position, then calculate its scattering offset
        float height = kInnerRadius + kCameraHeight;
        float depth = exp(kScaleOverScaleDepth * (-kCameraHeight));
        float startAngle = dot(eyeRay, cameraPos) / height;
        float startOffset = depth*scale(startAngle);


        // Initialize the scattering loop variables
        float sampleLength = far / kSamples;
        float scaledLength = sampleLength * kScale;
        float3 sampleRay = eyeRay * sampleLength;
        float3 samplePoint = cameraPos + sampleRay * 0.5;

        // Now loop through the sample rays
        float3 frontColor = float3(0.0, 0.0, 0.0);
        // Weird workaround: WP8 and desktop FL_9_3 do not like the for loop here
        // (but an almost identical loop is perfectly fine in the ground calculations below)
        // Just unrolling this manually seems to make everything fine again.
//              for(int i=0; i<int(kSamples); i++)
        {
            float height = length(samplePoint);
            float depth = exp(kScaleOverScaleDepth * (kInnerRadius - height));
            float lightAngle = dot(-_WorldSpaceLightPos0.xyz, samplePoint) / height;
            float cameraAngle = dot(eyeRay, samplePoint) / height;
            float scatter = (startOffset + depth*(scale(lightAngle) - scale(cameraAngle)));
            float3 attenuate = exp(-clamp(scatter, 0.0, kMAX_SCATTER) * (kInvWavelength * kKr4PI + kKm4PI));

            frontColor += attenuate * (depth * scaledLength);
            samplePoint += sampleRay;
        }
        {
            float height = length(samplePoint);
            float depth = exp(kScaleOverScaleDepth * (kInnerRadius - height));
            float lightAngle = dot(-_WorldSpaceLightPos0.xyz, samplePoint) / height;
            float cameraAngle = dot(eyeRay, samplePoint) / height;
            float scatter = (startOffset + depth*(scale(lightAngle) - scale(cameraAngle)));
            float3 attenuate = exp(-clamp(scatter, 0.0, kMAX_SCATTER) * (kInvWavelength * kKr4PI + kKm4PI));

            frontColor += attenuate * (depth * scaledLength);
            samplePoint += sampleRay;
        }



        // Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader
        cIn = frontColor * (kInvWavelength * kKrESun);
        cOut = frontColor * kKmESun;
    }
    else
    {
        // 반구아래라면 지면이다.
        // Ground
        far = (-kCameraHeight) / (min(-0.001, eyeRay.y));

        float3 pos = cameraPos + far * eyeRay;

        // Calculate the ray's starting position, then calculate its scattering offset
        float depth = exp((-kCameraHeight) * (1.0/kScaleDepth));
        float cameraAngle = dot(-eyeRay, pos);
        float lightAngle = dot(-_WorldSpaceLightPos0.xyz, pos);
        float cameraScale = scale(cameraAngle);
        float lightScale = scale(lightAngle);
        float cameraOffset = depth*cameraScale;
        float temp = (lightScale + cameraScale);

        // Initialize the scattering loop variables
        float sampleLength = far / kSamples;
        float scaledLength = sampleLength * kScale;
        float3 sampleRay = eyeRay * sampleLength;
        float3 samplePoint = cameraPos + sampleRay * 0.5;

        // Now loop through the sample rays
        float3 frontColor = float3(0.0, 0.0, 0.0);
        float3 attenuate;

        //for(int i=0; i<int(kSamples); i++) // Loop removed because we kept hitting SM2.0 temp variable limits. Doesn't affect the image too much.
        {
            float height = length(samplePoint);
            float depth = exp(kScaleOverScaleDepth * (kInnerRadius - height));
            float scatter = depth*temp - cameraOffset;
            attenuate = exp(-clamp(scatter, 0.0, kMAX_SCATTER) * (kInvWavelength * kKr4PI + kKm4PI));
            frontColor += attenuate * (depth * scaledLength);
            samplePoint += sampleRay;
        }

        cIn = frontColor * (kInvWavelength * kKrESun + kKmESun);
        cOut = clamp(attenuate, 0.0, 1.0);
    }

#if SKYBOX_SUNDISK == SKYBOX_SUNDISK_HQ
    OUT.vertex          = -eyeRay;
#elif SKYBOX_SUNDISK == SKYBOX_SUNDISK_SIMPLE
    OUT.rayDir          = float3(-eyeRay);
#else
    OUT.skyGroundFactor = -eyeRay.y / SKY_GROUND_THRESHOLD;
#endif

    // if we want to calculate color in vprog:
    // 1. in case of linear: multiply by _Exposure in here (even in case of lerp it will be common multiplier, so we can skip mul in fshader)
    // 2. in case of gamma and SKYBOX_COLOR_IN_TARGET_COLOR_SPACE: do sqrt right away instead of doing that in fshader

    // 하늘과 지면 색깔을 구함
    OUT.groundColor = _Exposure * (cIn + COLOR_2_LINEAR(_GroundColor) * cOut);
    OUT.skyColor    = _Exposure * (cIn * getRayleighPhase(_WorldSpaceLightPos0.xyz, -eyeRay));

#if SKYBOX_SUNDISK != SKYBOX_SUNDISK_NONE
    // The sun should have a stable intensity in its course in the sky. Moreover it should match the highlight of a purely specular material.
    // This matching was done using the standard shader BRDF1 on the 5/31/2017
    // Finally we want the sun to be always bright even in LDR thus the normalization of the lightColor for low intensity.
    float lightColorIntensity = clamp(length(_LightColor0.xyz), 0.25, 1);
    #if SKYBOX_SUNDISK == SKYBOX_SUNDISK_SIMPLE
        OUT.sunColor    = kSimpleSundiskIntensityFactor * saturate(cOut * kSunScale) * _LightColor0.xyz / lightColorIntensity;
    #else // SKYBOX_SUNDISK_HQ
        OUT.sunColor    = kHDSundiskIntensityFactor * saturate(cOut) * _LightColor0.xyz / lightColorIntensity;
    #endif

#endif

#if defined(UNITY_COLORSPACE_GAMMA) && SKYBOX_COLOR_IN_TARGET_COLOR_SPACE
    OUT.groundColor = sqrt(OUT.groundColor);
    OUT.skyColor    = sqrt(OUT.skyColor);
    #if SKYBOX_SUNDISK != SKYBOX_SUNDISK_NONE
        OUT.sunColor= sqrt(OUT.sunColor);
    #endif
#endif

    return OUT;
}


// Calculates the Mie phase function
float getMiePhase(float eyeCos, float eyeCos2)
{
    float temp = 1.0 + MIE_G2 - 2.0 * MIE_G * eyeCos;
    temp = pow(temp, pow(_SunSize,0.65) * 10);
    temp = max(temp,1.0e-4); // prevent division by zero, esp. in float precision
    temp = 1.5 * ((1.0 - MIE_G2) / (2.0 + MIE_G2)) * (1.0 + eyeCos2) / temp;
    #if defined(UNITY_COLORSPACE_GAMMA) && SKYBOX_COLOR_IN_TARGET_COLOR_SPACE
        temp = pow(temp, .454545);
    #endif
    return temp;
}

// Calculates the sun shape
float calcSunAttenuation(float3 lightPos, float3 ray)
{
#if SKYBOX_SUNDISK == SKYBOX_SUNDISK_SIMPLE
    float3 delta = lightPos - ray;
    float dist = length(delta);
    float spot = 1.0 - smoothstep(0.0, _SunSize, dist);
    return spot * spot;
#else // SKYBOX_SUNDISK_HQ
    float focusedEyeCos = pow(saturate(dot(lightPos, ray)), _SunSizeConvergence);
    return getMiePhase(-focusedEyeCos, focusedEyeCos * focusedEyeCos);
#endif
}

float4 frag (v2f IN) : SV_Target
{
    float3 col = float3(0.0, 0.0, 0.0);

// if y > 1 [eyeRay.y < -SKY_GROUND_THRESHOLD] - ground
// if y >= 0 and < 1 [eyeRay.y <= 0 and > -SKY_GROUND_THRESHOLD] - horizon
// if y < 0 [eyeRay.y > 0] - sky
#if SKYBOX_SUNDISK == SKYBOX_SUNDISK_HQ
    float3 ray = normalize(IN.vertex.xyz);
    float y = ray.y / SKY_GROUND_THRESHOLD;
#elif SKYBOX_SUNDISK == SKYBOX_SUNDISK_SIMPLE
    float3 ray = IN.rayDir.xyz;
    float y = ray.y / SKY_GROUND_THRESHOLD;
#else
    float y = IN.skyGroundFactor;
#endif

    // if we did precalculate color in vprog: just do lerp between them
    col = lerp(IN.skyColor, IN.groundColor, saturate(y));

#if SKYBOX_SUNDISK != SKYBOX_SUNDISK_NONE
    if(y < 0.0)
    {
        col += IN.sunColor * calcSunAttenuation(-_WorldSpaceLightPos0.xyz, -ray);
    }
#endif

#if defined(UNITY_COLORSPACE_GAMMA) && !SKYBOX_COLOR_IN_TARGET_COLOR_SPACE
    col = LINEAR_2_OUTPUT(col);
#endif

    return float4(col,1.0);

}