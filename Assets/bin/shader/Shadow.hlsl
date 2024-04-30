#include "header\H_Const.hlsli"
#include "header\H_Input.hlsli"

StructuredBuffer<Light> gLightTexture : register(t12);

cbuffer PerMaterial : register(b2)
{
    int _current_light_idx;
}

struct VSShadowOutput
{
    float4 posH : SV_POSITION;
    float2 uv : TEXCOORD0;

};
struct GSOutput
{
    float4 posH : SV_POSITION;
    float2 uv : TEXCOORD0;
    uint RTIndex : SV_RenderTargetArrayIndex;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

VSShadowOutput VSMain(VSInput input)
{
    VSShadowOutput output;

#if !defined(SKIN)

    float4 posW = mul(float4(input.posL, 1.0f), _world);
    float4 posV = mul(posW, _camera._view);
    output.posH = mul(posV, _camera._proj);

#else   //SKIN

    float3 posL = float3(0.0f, 0.0f, 0.0f);

        [unroll]
    for (int i = 0; i < BONECNT; i++)
    {
        if (input.bone[i] >= 0)
        {
            posL += input.weight[i] * mul(float4(input.posL, 1.0f), _boneTransforms[input.bone[i]]).xyz;
        }
    }

    float4 posW = mul(float4(posL, 1.0f), _world);
    float4 posV = mul(posW, _camera._view);
    output.posH = mul(posV, _camera._proj);

#endif
    output.uv = input.uv;

    return output;
}

//--------------------------------------------------------------------------------------
// Geometry Shader
//--------------------------------------------------------------------------------------
[maxvertexcount(12)] // 4(shadow slice) * 3
void CascadeGSMain(triangle VSShadowOutput input[3], inout TriangleStream<GSOutput> stream)
{
    for (int cascade = 0; cascade < 4; ++cascade)
    {
        //Generate cascade slice
        GSOutput output;
        output.RTIndex = cascade;

        for (int i = 0; i < 3; ++i)
        {
            output.posH = mul(input[i].posH, _shadow._shadowTransform[cascade]);
            output.uv = input[i].uv;

            stream.Append(output);
        }

        stream.RestartStrip();
    }
}

[maxvertexcount(3)] // 1(shadow slice) * 3
void SpotGSMain(triangle VSOutput input[3], inout TriangleStream<GSOutput> stream)
{
    for (int i = 0; i < 3; ++i)
    {
        GSOutput output;

        output.RTIndex = 0;

        output.posH = mul(input[i].posH, gLightTexture[_current_light_idx]._shadowMatrix[0]);
        output.uv = input[i].uv;
        
        stream.Append(output);
    }

    stream.RestartStrip();
}

[maxvertexcount(18)] // 6(shadow slice) * 3
void PointGSMain(triangle VSOutput input[3], inout TriangleStream<GSOutput> stream)
{
    for (int directionIdx = 0; directionIdx < 6 ;directionIdx++)
    {
        GSOutput output;

        output.RTIndex = directionIdx;

        for (int i = 0; i < 3; ++i)
        {
            output.posH = mul(input[i].posH, gLightTexture[_current_light_idx]._shadowMatrix[directionIdx]);
            output.uv = input[i].uv;

            stream.Append(output);
        }

        stream.RestartStrip();
    }
}