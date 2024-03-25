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
	float3 _emissionColor = {0.f, 0.f, 0.f};
}

float3 Albedo(float2 uv)
{
    float3 _albedo = _albedoColor.rgb * AlbedoMap.Sample(samWrapLinear, uv).rgb;

    return _albedo;
}

float3 NormalInTangentSpace(float2 uv)
{
    float3 _normalTangent = UnpackScaleNormal(NormalMap.Sample(samWrapLinear, uv), _bumpScale);

    return _normalTangent;
}

float2 MetallicRough(float2 uv)
{
    float2 _mr = float2(_metallic, _roughness) * MetallicRougnessMap.Sample(samWrapLinear, uv).rg;

    return _mr;
}

float3 Emission(float2 uv)
{
    float3 _emission = _emissionColor.rgb * EmissiveMap.Sample(samWrapLinear, uv).rgb;

    return _emission;
}

float AmbeintOcclution(float2 uv)
{
    float _ao = AmbientOcclusionMap.Sample(samWrapLinear, uv).g;

    return _ao;
}

#endif