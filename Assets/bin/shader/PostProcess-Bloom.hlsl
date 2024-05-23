#include "header/H_Input.hlsli"

#define ENABLE_THRESHOLD

#if defined(ENABLE_THRESHOLD)
//--------------------------------------------------------------------------------------
// Bloom with threshold
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// Bloom Curve
//--------------------------------------------------------------------------------------

cbuffer PerMaterial : register(b2)
{
    float _intensity        : packoffset(c0);
    float _threshold        : packoffset(c0.y);
    float _coefficient      : packoffset(c0.z);
    float2 _blurTexelSize   : packoffset(c0.w);
    float2 _blurOrder       : packoffset(c1.y);
}

Texture2D gLightTexture : register(t0);
SamplerState gSamWrapLinear : register(s0);

float GetBloomCurve(float intensity, float threshold)
{
    float result = intensity;
    intensity *= 2.0f;

#ifdef BLOOMCURVE_METHOD_1
    result = intensity * 0.05 + max(0, intensity - threshold) * 0.5; //default threshold =1.26
#endif

#ifdef BLOOMCURVE_METHOD_2
    result = intensity * intensity / 3.2;
#endif

#ifdef BLOOMCURVE_METHOD_3
    result = max(0, intensity - threshold); //default threshold =1.0
    result *= result;
#endif

    return result * 0.5f;
}


void PSBloomCurve(out float4 color : SV_Target, in VSOutput input)
{
    color = gLightTexture.Sample(gSamWrapLinear, input.uv);

    float intensity = dot(color.rgb, float3(_intensity.rrr));

    float bloom_Intensity = GetBloomCurve(intensity, _threshold);

    // 강도가 0으로 나오면 문제가 생길수 있다.
    if (intensity == (float)0.f)
        color = float4(0.f, 0.f, 0.f, 0.f);

    color * bloom_Intensity / intensity;
}

//--------------------------------------------------------------------------------------
// Bloom Downsample
//--------------------------------------------------------------------------------------
Texture2D gDownSampleSrcTexture : register(t0);
SamplerState gSamBorderLinear : register(s0);

float4 PSDownSample(VSOutput input) : SV_TARGET
{
    float3 outColor = float3(0.0f, 0.0f, 0.0f);
    
    for (uint y = 0; y < 6; y++)
    {
        for (uint x = 0; x < 6; x++)
        {
            // Compute the sum of color values
            outColor += gDownSampleSrcTexture.Sample(gSamBorderLinear, input.uv, int2(x, y)).rgb;
        }
    }
    
    outColor /= 36;
    
    return float4(outColor, 1.0f);
}
//--------------------------------------------------------------------------------------
// Bloom Composite UpSample
//--------------------------------------------------------------------------------------

Texture2D gSrcTexture0 : register(t0);
Texture2D gSrcTexture1 : register(t1);

// pixel shader
float4 PSCompositeUpSample(VSOutput input) : SV_TARGET
{
    // output: tex0 + coefficient * tex1
    return mad(_coefficient, gSrcTexture1.Sample(gSamWrapLinear, input.uv), gSrcTexture0.Sample(gSamWrapLinear, input.uv));
}

//--------------------------------------------------------------------------------------
// Bloom Gaussian Blur
//--------------------------------------------------------------------------------------

#define GAUSSIAN_RADIUS 4

Texture2D gBlurSrcTexture : register(t0);

static float gWeights[9] = { 0.0135f, 0.0476f, 0.1172f, 0.2011f, 0.2408f, 0.2011f, 0.1172f, 0.0476f, 0.0135f };

float4 PSBloomBlur(VSOutput input) : SV_TARGET
{
    float3 outColor = float3(0.0f, 0.0f, 0.0f);
    
    float2 texOffset = _blurTexelSize * _blurTexelSize;
    float3 color = float3(0.0f, 0.0f, 0.0f);
    float weight = 0.0f;
    
    for (int i = -GAUSSIAN_RADIUS; i <= GAUSSIAN_RADIUS; ++i)
    {
        outColor += gWeights[i + GAUSSIAN_RADIUS] * gBlurSrcTexture.Sample(gSamBorderLinear, input.uv + i * texOffset).rgb;
    }
    
    return float4(outColor, 1.0f);
}

#else // witout threshold

//--------------------------------------------------------------------------------------
// Bloom without threshold
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// Downsample
//--------------------------------------------------------------------------------------
Texture2D gDownSampleSrcTexture : register(t0);
SamplerState gSamWrapLinear : register(s0);

cbuffer PerMaterial: register(b2)
{
    float _srcResolution;
}

float4 PSDownsample(VSOutput input) : SV_Target
{
    float2 srcTexelSize = 1.0 / _srcResolution;
    float x = srcTexelSize.x;
    float y = srcTexelSize.y;

    // Take 13 samples around current texel:
    // a - b - c
    // - j - k -
    // d - e - f
    // - l - m -
    // g - h - i
    // === ('e' is the current texel) ===
    float3 a = gDownSampleSrcTexture.Sample(gSamWrapLinear, float2(input.uv.x - 2*x, input.uv.y + 2*y)).rgb;
    float3 b = gDownSampleSrcTexture.Sample(gSamWrapLinear, float2(input.uv.x,       input.uv.y + 2*y)).rgb;
    float3 c = gDownSampleSrcTexture.Sample(gSamWrapLinear, float2(input.uv.x + 2*x, input.uv.y + 2*y)).rgb;

    float3 d = gDownSampleSrcTexture.Sample(gSamWrapLinear, float2(input.uv.x - 2*x, input.uv.y)).rgb;
    float3 e = gDownSampleSrcTexture.Sample(gSamWrapLinear, float2(input.uv.x,       input.uv.y)).rgb;
    float3 f = gDownSampleSrcTexture.Sample(gSamWrapLinear, float2(input.uv.x + 2*x, input.uv.y)).rgb;

    float3 g = gDownSampleSrcTexture.Sample(gSamWrapLinear, float2(input.uv.x - 2*x, input.uv.y - 2*y)).rgb;
    float3 h = gDownSampleSrcTexture.Sample(gSamWrapLinear, float2(input.uv.x,       input.uv.y - 2*y)).rgb;
    float3 i = gDownSampleSrcTexture.Sample(gSamWrapLinear, float2(input.uv.x + 2*x, input.uv.y - 2*y)).rgb;

    float3 j = gDownSampleSrcTexture.Sample(gSamWrapLinear, float2(input.uv.x - x, input.uv.y + y)).rgb;
    float3 k = gDownSampleSrcTexture.Sample(gSamWrapLinear, float2(input.uv.x + x, input.uv.y + y)).rgb;
    float3 l = gDownSampleSrcTexture.Sample(gSamWrapLinear, float2(input.uv.x - x, input.uv.y - y)).rgb;
    float3 m = gDownSampleSrcTexture.Sample(gSamWrapLinear, float2(input.uv.x + x, input.uv.y - y)).rgb;

    float4 downsample = float4(0, 0, 0, 0);

    // Apply weighted distribution:
    // 0.5 + 0.125 + 0.125 + 0.125 + 0.125 = 1
    // a,b,d,e * 0.125
    // b,c,e,f * 0.125
    // d,e,g,h * 0.125
    // e,f,h,i * 0.125
    // j,k,l,m * 0.5
    // This shows 5 square areas that are being sampled. But some of them overlap,
    // so to have an energy preserving downsample we need to make some adjustments.
    // The weights are the distributed, so that the sum of j,k,l,m (e.g.)
    // contribute 0.5 to the final color output. The code below is written
    // to effectively yield this sum. We get:
    // 0.125*5 + 0.03125*4 + 0.0625*4 = 1
    downsample = e*0.125;
    downsample += (a+c+g+i)*0.03125;
    downsample += (b+d+f+h)*0.0625;
    downsample += (j+k+l+m)*0.125;

    return downsample;
}

//--------------------------------------------------------------------------------------
// UpSample
//--------------------------------------------------------------------------------------

Texture2D gUpSampleSrcTexture : register(t0);

cbuffer PerMaterial: register(b2)
{
    float _filterRadius;
}

float4 PSUpsample(VSOutput input) : SV_Target
{
    // The filter kernel is applied with a radius, specified in texture
    // coordinates, so that the radius will vary across mip resolutions.
    float x = _filterRadius;
    float y = _filterRadius;

    // Take 9 samples around current texel:
    // a - b - c
    // d - e - f
    // g - h - i
    // === ('e' is the current texel) ===
    float3 a = gUpSampleSrcTexture.Sample(gSamWrapLinear, float2(input.uv.x - x, input.uv.y + y)).rgb;
    float3 b = gUpSampleSrcTexture.Sample(gSamWrapLinear, float2(input.uv.x,     input.uv.y + y)).rgb;
    float3 c = gUpSampleSrcTexture.Sample(gSamWrapLinear, float2(input.uv.x + x, input.uv.y + y)).rgb;

    float3 d = gUpSampleSrcTexture.Sample(gSamWrapLinear, float2(input.uv.x - x, input.uv.y)).rgb;
    float3 e = gUpSampleSrcTexture.Sample(gSamWrapLinear, float2(input.uv.x,     input.uv.y)).rgb;
    float3 f = gUpSampleSrcTexture.Sample(gSamWrapLinear, float2(input.uv.x + x, input.uv.y)).rgb;

    float3 g = gUpSampleSrcTexture.Sample(gSamWrapLinear, float2(input.uv.x - x, input.uv.y - y)).rgb;
    float3 h = gUpSampleSrcTexture.Sample(gSamWrapLinear, float2(input.uv.x,     input.uv.y - y)).rgb;
    float3 i = gUpSampleSrcTexture.Sample(gSamWrapLinear, float2(input.uv.x + x, input.uv.y - y)).rgb;

    float4 upsample = float4(0, 0, 0, 0);

    // Apply weighted distribution, by using a 3x3 tent filter:
    //  1   | 1 2 1 |
    // -- * | 2 4 2 |
    // 16   | 1 2 1 |
    upsample = e*4.0;
    upsample += (b+d+f+h)*2.0;
    upsample += (a+c+g+i);
    upsample *= 1.0 / 16.0;

    return gUpSampleSrcTexture;
}
#endif