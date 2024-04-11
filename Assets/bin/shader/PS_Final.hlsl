#include "header\H_Input.hlsli"

Texture2D gDebugTexture;
Texture2D gRenderTargetDepth;
Texture2D gDebugDepth;

SamplerState gSamLinear : register(s0);

float4 main(VSOutput input) : SV_Target
{
    return gDebugTexture.Sample(gSamLinear, input.uv);
    // float frameDepth = gRenderTargetDepth.Sample(gSamLinear, input.uv).r;
    // float4 debugColor = gDebugTexture.Sample(gSamLinear, input.uv);
    // float debugDepth = gDebugDepth.Sample(gSamLinear, input.uv).r;

    // if (debugDepth < frameDepth)
    // {
    //     return debugColor;
    // }
    // else
    // {
    //     clip(-1);
    // }

    // return float4(0.f, 0.f, 0.f, 0.f);
}