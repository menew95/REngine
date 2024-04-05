#if !defined(H_UTILITY)
#define H_UTILITY

float LerpOneTo(float b, float t)
{
	float oneMinus = 1.f - t;
	return oneMinus + b * t;
}

// world space의 tangent와 normal값으로 TBN 행렬을 생성
float3x3 CreateTangentToWorldPerVertex(float3 normalW, float3 tangentW)
{
    float3 T = normalize(tangentW - dot(tangentW, normalW) * normalW);
    float3 B = cross(normalW, T);

    return float3x3(T, B, normalW);
}

// tangent space normal을 world space로 변경
float3 TangentNormalToWorldNormal(float3 tangentToWorld[3], float3 tangentNormal)
{
    float3x3 TBN = float3x3(tangentToWorld[0], tangentToWorld[1], tangentToWorld[2]);

	// Transform from tangent space to world space.
	float3 normalW = mul(tangentNormal, TBN);

    return normalW;
}

// [0,1] packing 된 값을 [-1,1]으로 Unpacking
float3 UnpackScaleNormal(float4 packednormal, float bumpScale)
{
    float3 normal = packednormal.xyz * 2.f - 1.f;
    normal.xy *= bumpScale;
    return normal;
}

#endif