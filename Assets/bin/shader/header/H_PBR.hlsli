#if !defined(H_PBR)
#define H_PBR

#include "header\H_Math.hlsli"

float3 CalcIluminance(float lumen)
{
    return lumen / PI;
}

float3 FresnelSchlick(float cosTheta, float3 f0)
{
	// Schlick
	// F( N, V ) = F_0 + (1 - F_0) * ( 1 - N · V) ^ 5
	// F_0 = ( (n_1 - n_2) / (n_1 + n_2) ) ^ 2 => IOR 
	float s = 1.0 - cosTheta;

	float powT = s * s * s * s * s;

	return f0 + (1.0 - f0) * saturate(powT);
}

// [Heitz 2014, "Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs"]
float Vis_SmithJoint(float a2, float NoV, float NoL)
{
	float Vis_SmithV = NoL * sqrt(NoV * (NoV - NoV * a2) + a2);
	float Vis_SmithL = NoV * sqrt(NoL * (NoL - NoL * a2) + a2);
	return 0.5f / max(Vis_SmithV + Vis_SmithL, 1e-5f);
	return 0.5 * rcp(Vis_SmithV + Vis_SmithL);
}

float GGX_Distribution(float cosThetaNH, float alpha)
{
	float a2 = alpha * alpha;
	float d = (cosThetaNH * a2 - cosThetaNH) * cosThetaNH + 1.0;
	return a2 / (d * d * PI);
}

float3 Diffuse_Lambert(float3 DiffuseColor)
{
	return DiffuseColor * (1 / PI);
}

float3 CookTorrance_GGX(in float roughness2
	, in float metallic
	, in float3 specColor
	, in float3 diffColor
	, in float NdotV
	, in float NdotL
	, in float LdotH
	, in float NdotH
)
{
    float3 F = FresnelSchlick(LdotH, specColor);
	float G = Vis_SmithJoint(roughness2, NdotV, NdotL);
	float D = GGX_Distribution(NdotH, roughness2);

	float3 specK = F * G * D;

	float3 kD = 1 - F;
	kD *= 1 - metallic;

	float3 diffK = Diffuse_Lambert(kD * diffColor);

	return (diffK + specK) * NdotL;
}

// Shlick's approximation of the Fresnel factor.
float3 fresnelSchlick(float3 F0, float cosTheta)
{
	return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float ndfGGX(float cosLh, float roughness)
{
	float alpha   = roughness * roughness;
	float alphaSq = alpha * alpha;

	float denom = (cosLh * cosLh) * (alphaSq - 1.0f) + 1.0f;

	return alphaSq / max(PI * denom * denom, EPSILON);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
	return cosTheta / (cosTheta * (1.0f - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
	float r = roughness + 1.0f;
	float k = (r * r) / 8.0f; // EPBR_PIc suggests using this roughness remapPBR_PIng for analytic lights.
	return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}


float3 Unreal_PBR(
    in float roughness
	, in float metallic
	, in float3 specColor
	, in float3 albedoColor
	, in float NdotV
	, in float NdotL
	, in float LdotH
	, in float NdotH
    )
{
    float3 F = fresnelSchlick(specColor, max(0.0f, LdotH));
    float D = ndfGGX(NdotH, roughness);
    float G = gaSchlickGGX(NdotL, NdotV, roughness);

    float3 specK = (F * D * G) / max(0.0000001f, 4.0 * NdotL * NdotV);

    float3 diffK = albedoColor / PI;

    return diffK + specK;
}
#endif