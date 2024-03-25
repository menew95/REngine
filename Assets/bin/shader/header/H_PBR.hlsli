#include "header\H_Math.hlsli"

#if !defined(H_PBR)
#define H_PBR

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float _a      = roughness * roughness;
    float _a2     = _a * _a;
    float _NdotH  = max(dot(N, H), 0.0);
    float _NdotH2 = _NdotH * _NdotH;
	
    float _nom    = _a2;
    float _denom  = (_NdotH2 * (_a2 - 1.0) + 1.0);
    _denom        = PI * _denom * _denom;
	
    return _nom / _denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float _r = (roughness + 1.0f);
    float _k = (_r * _r) / 8.0f;

    float _num = NdotV;
    float _denom = NdotV * (1.0f - _k) + _k;

    return _num / _denom;
}
  
float GeometrySmith(float N, float3 V, float3 L, float k)
{
    float _NdotV = max(dot(N, V), 0.0);
    float _NdotL = max(dot(N, L), 0.0);
    float _ggx1 = GeometrySchlickGGX(_NdotV, k);
    float _ggx2 = GeometrySchlickGGX(_NdotL, k);
	
    return _ggx1 * _ggx2;
}

float3 fresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}  

float4 BRDF(half3 diffColor, half3 specColor, half oneMinusReflectivity, half smoothness,
    float3 normal, float3 viewDir)
{
    float3 _color;

    

    return half4(_color, 1);
}

#endif