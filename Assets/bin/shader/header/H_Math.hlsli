#if !defined(H_MATH)

#define H_MATH

#define PI            3.14159265359f
#define TWO_PI        6.28318530718f
#define FOUR_PI       12.56637061436f
#define INV_PI        0.31830988618f
#define INV_TWO_PI    0.15915494309f
#define INV_FOUR_PI   0.07957747155f
#define HALF_PI       1.57079632679f
#define INV_HALF_PI   0.636619772367f
static const float EPSILON = 1e-6f;
static const float3 Fdielectric = 0.04f;

#define GAMMA = 2.2f;
#define INV_GAMMA = 2.2f;

//---------------------------------------------------------------------------------------
// Transforms a normal map sample to world space.
//---------------------------------------------------------------------------------------
void NormalSampleToWorldSpace(out float3 bumpedNormalW, in float3 normalMapSample, in float3 unitNormalW, in float3 tangentW)
{
	//bumpedNormalW = { 0.0f, 0.0f, 0.0f };
	// Uncompress each component from [0,1] to [-1,1].
	float3 normalT = 2.0f * normalMapSample - 1.0f;

	// Build orthonormal basis.
	float3 N = unitNormalW;
	float3 T = normalize(tangentW - dot(tangentW, N) * N);
	float3 B = cross(N, T);
	float3 BT = normalize(B - dot(B, N) * N - B * dot(B, T) * T);

	float3x3 TBN = float3x3(T, BT, N);

	// Transform from tangent space to world space.
	bumpedNormalW = mul(normalT, TBN);

	normalize(bumpedNormalW);
}

//---------------------------------------------------------------------------------------
// CubeMap Sampler
//---------------------------------------------------------------------------------------
float4 CubeMapSampler(SamplerState sam, TextureCube cubeMap, float3 toEye, float3 normal)
{
	// CubeMap Reflection
	float3 incident = -toEye;
	float3 reflectionVector = reflect(incident, normal);
	float4 reflectionColor = cubeMap.Sample(sam, reflectionVector);

	return reflectionColor;
}


inline float3 GetPositionVS(float2 texcoord, float depth, matrix projInv)
{
	float4 clipSpaceLocation;
	clipSpaceLocation.xy = texcoord * 2.0f - 1.0f;
	clipSpaceLocation.y *= -1;
	clipSpaceLocation.z = depth;
	clipSpaceLocation.w = 1.0f;
	float4 homogenousLocation = mul(clipSpaceLocation, projInv);
	return homogenousLocation.xyz / homogenousLocation.w;
}

#endif