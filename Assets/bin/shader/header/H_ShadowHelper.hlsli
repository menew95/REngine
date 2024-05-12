#if !defined(H_SHADOW_HELPER)
#define H_SHADOW_HELPER

float2 LightSpacePosToUV(float4 lightSpacePos)
{
    float2 uv = lightSpacePos.xy + 1.f;

    uv /= 2.0f;
    uv.y = 1.f - uv.y;

    return uv;
}

void GetAtlasUV(
    in float2 lightUV
    , in float2 resolution
    , in float2 uv0
    , in float2 atlasResolution
    , out float2 atlasUV
)
{
    // 해상도 영역 내의 uv 값을 실제 텍스처 좌표로 변환
    float2 texCoord = (uv0 + lightUV * resolution) / atlasResolution;

    // 반환할 아틀라스 UV 좌표
    atlasUV = texCoord;
}

float CalcShadowFactorFromShadowAtlas(
    in SamplerComparisonState samShadow
    , in Texture2D shadowMapAtlas
    , in float2 shadowResolution
    , in float2 uv0
    , in float2 uv1
    , in float2 atlasResolution
    , in float4x4 shadowMatrix
    , in float3 worldPosition
    )
{
    // light space proj 값을 구함
    float4 lightSpacePos = mul(float4(worldPosition.xyz, 1.f), shadowMatrix);
        
    float3 projCoords = float3(0.0f, 0.0f, 0.0f);

    projCoords = lightSpacePos.xyz / lightSpacePos.w;
	projCoords.x = projCoords.x * 0.5 + 0.5f;
	projCoords.y = -projCoords.y * 0.5 + 0.5f;

    // 아틀라스 텍스처에서의 uv 값을 구함
    float2 _atlasUV = float2(0, 0);
    GetAtlasUV(projCoords.xy, shadowResolution, uv0, atlasResolution, _atlasUV);

    float percentLit = 0.0f;
    float percentLit_blend = 0.0f;

    // offset 값을 할당된 그림자맵의 해상도로부터 구함
    // Variance 해보자
    const float dx = 1.f / (atlasResolution.x);
    const float dy = 1.f / (atlasResolution.y);

    const float2 offsets[9] =
    {
        float2(-dx, -dy), float2(0.f, -dy), float2(+dx, -dy),
        float2(-dx, 0.f), float2(0.f, 0.f), float2(+dx, 0.f),
        float2(-dx, +dy), float2(0.f, +dy), float2(+dx, +dy)
    };

    const int2 _offset[9] = 
    {
        int2(-1, -1), int2(0, -1), int2(1, -1),
        int2(-1, 0), int2(0, 0), int2(1, 0),
        int2(-1, 1), int2(0, 1), int2(1, 1)
    };

    float3 vShadowMapTextureCoordDDX;
    float3 vShadowMapTextureCoordDDY;

    for (int i = 0; i < 9; i++)
    {
        //percentLit += shadowMapAtlas.SampleCmpLevelZero(samShadow, float2(_atlasUV + offsets[i]), projCoords.z).r;
        percentLit += shadowMapAtlas.SampleCmpLevelZero(samShadow, _atlasUV, projCoords.z, _offset[i]).r;
    }
    percentLit /= 9.0f;

    // for light bleeding
    //static const float magicNumber = 2.f;
    //percentLit = pow(percentLit, magicNumber);

    return percentLit;
}

#endif