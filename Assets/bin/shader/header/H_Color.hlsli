#if !defined(H_COLOR)
#define H_COLOR

#ifdef COLORSPACE_GAMMA
#define colorSpaceGrey half4(0.5, 0.5, 0.5, 0.5)
#define colorSpaceDouble half4(2.0, 2.0, 2.0, 2.0)
#define colorSpaceDielectricSpec half4(0.220916301, 0.220916301, 0.220916301, 1.0 - 0.220916301)
#define colorSpaceLuminance half4(0.22, 0.707, 0.071, 0.0) // Legacy: alpha is set to 0.0 to specify gamma mode
#else // Linear values
#define colorSpaceGrey half4(0.214041144, 0.214041144, 0.214041144, 0.5)
#define colorSpaceDouble half4(4.59479380, 4.59479380, 4.59479380, 2.0)
#define colorSpaceDielectricSpec half4(0.04, 0.04, 0.04, 1.0 - 0.04) // standard dielectric reflectivity coef at incident angle (= 4%)
#define colorSpaceLuminance half4(0.0396819152, 0.458021790, 0.00609653955, 1.0) // Legacy: alpha is set to 1.0 to specify linear mode
#endif

// Decodes HDR textures
// handles dLDR, RGBM formats
inline float3 DecodeHDR (float4 data, float4 decodeInstructions)
{
    // Take into account texture alpha if decodeInstructions.w is true(the alpha value affects the RGB channels)
    float alpha = decodeInstructions.w * (data.a - 1.0) + 1.0;

    // If Linear mode is not supported we can skip exponent part
    #if defined(COLORSPACE_GAMMA)
        return (decodeInstructions.x * alpha) * data.rgb;
    #else
        #if defined(USE_NATIVE_HDR)
            return decodeInstructions.x * data.rgb; // Multiplier for future HDRI relative to absolute conversion.
        #else
            return (decodeInstructions.x * pow(alpha, decodeInstructions.y)) * data.rgb;
        #endif
    #endif
}

float3 DecodeRGBE(float4 rgbe, float exposure, int exponent)
{
    const float gamma = 1.0 / 2.2;

    #if defined(COLORSPACE_GAMMA)
        return pow(rgbe.xyz * exposure, gamma);
    #else
        return rgbe.xyz * exposure;
    #endif
}

// Decodes HDR textures
// handles dLDR, RGBM formats
inline float3 DecodeLightmapRGBM (float4 data, float4 decodeInstructions)
{
    // If Linear mode is not supported we can skip exponent part
    #if defined(COLORSPACE_GAMMA)
        #if defined(FORCE_LINEAR_READ_FOR_RGBM)
            return (decodeInstructions.x * data.a) * sqrt(data.rgb);
        #else
            return (decodeInstructions.x * data.a) * data.rgb;
        #endif
    #else
        return (decodeInstructions.x * pow(data.a, decodeInstructions.y)) * data.rgb;
    #endif
}
#endif