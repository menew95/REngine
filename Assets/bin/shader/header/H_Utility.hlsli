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

inline float GammaToLinearSpaceExact (float value)
{
    if (value <= 0.04045F)
        return value / 12.92F;
    else if (value < 1.0F)
        return pow((value + 0.055F)/1.055F, 2.4F);
    else
        return pow(value, 2.2F);
}

inline float3 GammaToLinearSpace (float3 sRGB)
{
    // Approximate version from http://chilliant.blogspot.com.au/2012/08/srgb-approximations-for-hlsl.html?m=1
    return sRGB * (sRGB * (sRGB * 0.305306011h + 0.682171111h) + 0.012522878h);

    // Precise version, useful for debugging.
    //return float3(GammaToLinearSpaceExact(sRGB.r), GammaToLinearSpaceExact(sRGB.g), GammaToLinearSpaceExact(sRGB.b));
}

inline float LinearToGammaSpaceExact (float value)
{
    if (value <= 0.0F)
        return 0.0F;
    else if (value <= 0.0031308F)
        return 12.92F * value;
    else if (value < 1.0F)
        return 1.055F * pow(value, 0.4166667F) - 0.055F;
    else
        return pow(value, 0.45454545F);
}

inline float3 LinearToGammaSpace (float3 linRGB)
{
    linRGB = max(linRGB, float3(0.h, 0.h, 0.h));
    // An almost-perfect approximation from http://chilliant.blogspot.com.au/2012/08/srgb-approximations-for-hlsl.html?m=1
    return max(1.055h * pow(linRGB, 0.416666667h) - 0.055h, 0.h);

    // Exact version, useful for debugging.
    //return float3(LinearToGammaSpaceExact(linRGB.r), LinearToGammaSpaceExact(linRGB.g), LinearToGammaSpaceExact(linRGB.b));
}

#endif