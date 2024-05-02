#include "header\H_Const.hlsli"
#include "header\H_Input.hlsli"

cbuffer PerLight : register(b2)
{
    //int _current_light_idx;
    uint _type; 
    float3 _direction;
    
    float3 _position;
    float _attenuationRadius;
    
    float3 _color;
    float _intensity;
  
    float _angle;
    float _innerAngle;
    float _fallOffExponential;
    float _width;

    float3 _up;
    float _height;

    int _staticShadowMapIdx;
    int _dynamicShadowMapIdx;
    uint _shadowState;
    int _pad0;

    matrix _shadowMatrix[6]; 

    float2 _uv0[6];
    float2 _uv1[6];
}

struct VSShadowOutput
{
    float4 posW : SV_POSITION;
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

    output.posW = mul(float4(input.posL, 1.0f), _world);

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

    output.posW = mul(float4(posL, 1.0f), _world);

#endif
    output.uv = input.uv;

#if defined(SPOT_LIGHT)
    output.posW = mul(output.posW, _shadowMatrix[0])
#endif

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
            output.posH = mul(input[i].posW, _shadow._shadowTransform[cascade]);
            output.uv = input[i].uv;

            stream.Append(output);
        }

        stream.RestartStrip();
    }
}

[maxvertexcount(3)] // 1(shadow slice) * 3
void SpotGSMain(triangle VSShadowOutput input[3], inout TriangleStream<GSOutput> stream)
{
    for (int i = 0; i < 3; ++i)
    {
        GSOutput output;

        output.RTIndex = 0;

        output.posH = mul(input[i].posW, _shadowMatrix[0]);
        output.uv = input[i].uv;

        stream.Append(output);
    }

    stream.RestartStrip();
}

[maxvertexcount(18)] // 6(shadow slice) * 3
void PointGSMain(triangle VSShadowOutput input[3], inout TriangleStream<GSOutput> stream)
{
    for (int directionIdx = 0; directionIdx < 6 ;directionIdx++)
    {
        GSOutput output;

        output.RTIndex = directionIdx;

        for (int i = 0; i < 3; ++i)
        {
            output.posH = mul(input[i].posW, _shadowMatrix[directionIdx]);
            output.uv = input[i].uv;

            stream.Append(output);
        }

        stream.RestartStrip();
    }
}