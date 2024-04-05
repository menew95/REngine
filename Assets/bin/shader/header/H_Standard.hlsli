#include "header\H_Utility.hlsli"

#if !defined(H_STANDARD)
#define H_STANDARD

// pbr standard input
Texture2D AlbedoMap				: register(t0);
Texture2D NormalMap				: register(t1);
Texture2D MetallicRougnessMap	: register(t3);
Texture2D EmissiveMap			: register(t4);
Texture2D AmbientOcclusionMap	: register(t5);

SamplerState samWrapLinear : register(s0);

cbuffer PerMaterial : register(b2)
{
    float4 _albedoColor = { 1.f, 1.f, 1.f, 1.f };
	float _roughness = 0.5f;
	float _metallic = 0.f;
	float _bumpScale = 1.f;
	float4 _emissionColor = {0.f, 0.f, 0.f, 1.f};
}

float3 Albedo(float2 uv)
{
    float3 albedo = _albedoColor.rgb * AlbedoMap.Sample(samWrapLinear, uv).rgb;

    return albedo;
}

float3 NormalInTangentSpace(float2 uv)
{
    float3 normalTangent = UnpackScaleNormal(NormalMap.Sample(samWrapLinear, uv), _bumpScale);

    return normalTangent;
}

float2 MetallicRough(float2 uv)
{
    float2 mr = float2(_metallic, _roughness) * MetallicRougnessMap.Sample(samWrapLinear, uv).rg;

    return mr;
}

float3 Emission(float2 uv)
{
    float3 emission = _emissionColor.rgb * EmissiveMap.Sample(samWrapLinear, uv).rgb;

    return emission;
}

float AmbeintOcclution(float2 uv)
{
    float ao = AmbientOcclusionMap.Sample(samWrapLinear, uv).g;

    return ao;
}

#endif