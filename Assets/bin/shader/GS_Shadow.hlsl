#include "header\H_Const.hlsli"

cbuffer PerLight : register(b3)
{
    Light _lightInfo;
}

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------

struct VSOutput
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

//Cascade Shadow
//--------------------------------------------------------------------------------------
// Geometry Shader
//--------------------------------------------------------------------------------------
[maxvertexcount(12)] // 4(shadow slice) * 3
void GSMain(triangle VSOutput input[3], inout TriangleStream<GSOutput> stream)
{
    GSOutput output;

    for (int cascade = 0; cascade < 4; ++cascade)
    {
        //Generate cascade slice
        output.RTIndex = cascade;

        for (int i = 0; i < 3; ++i)
        {
            output.posH = input[i].posH;
            output.posH = mul(output.posH, _shadow._shadowTransform[cascade]);
            output.uv = input[i].uv;
            //output.PosH /= output.PosH.w; //orth이므로 w는 어짜피 1이다.
            stream.Append(output);
        }

        stream.RestartStrip();
    }
}

//Spot Shadow
//--------------------------------------------------------------------------------------
// Geometry Shader
//--------------------------------------------------------------------------------------
[maxvertexcount(3)] // 1(shadow slice) * 3
void SpotGSMain(triangle VSOutput input[3], inout TriangleStream<GSOutput> stream)
{


    for (int i = 0; i < 3; ++i)
    {
        GSOutput output;
        // Generate cascade slice
        if(g_isStatic > 0)
            output.RTIndex = _light[g_idx].staticShadowMapIdx;
        else
            output.RTIndex = _light[g_idx].dynamicShadowMapIdx;

        output.PosH = input[i].PosH;
        output.PosH = mul(output.PosH, _light.ShadowMatrix[0]);
        output.Texcoord0 = input[i].Texcoord0;
        //output.PosH /= output.PosH.w;
        //output.PosH.w = 1.f;
        stream.Append(output);
    }

    stream.RestartStrip();
}

// PointLight Shadow
//--------------------------------------------------------------------------------------
//  Geometry Shader
//--------------------------------------------------------------------------------------
[maxvertexcount(18)] // 6(shadow slice) * 3
void PointGSMain(triangle VSOutput input[3], inout TriangleStream<GSOutput> stream)
{

    for (int directionIdx = 0; directionIdx < 6 ;directionIdx++)
        {

            for (int i = 0; i < 3; ++i)
            {
                GSOutput output;

                // 현재의 라이트맵 포지션 + 그릴 평면
                if (g_isStatic > 0)
                    output.RTIndex = g_Light[g_idx].staticShadowMapIdx * 6 + directionIdx;
                else
                    output.RTIndex = g_Light[g_idx].dynamicShadowMapIdx * 6 + directionIdx;

                output.PosH = input[i].PosH;
                output.PosH = mul(output.PosH, g_Light[g_idx].ShadowMatrix[directionIdx]);
                output.Texcoord0 = input[i].Texcoord0;

                stream.Append(output);
        }
          stream.RestartStrip();
    }

}