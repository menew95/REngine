#include "header\H_Input.hlsli"

Texture2D gDebugTexture;
Texture2D gRenderTargetDepth;
Texture2D gDebugDepth;

SamplerState gSamLinear : register(s0);

float4 main(VSOutput input) : SV_Target
{
    return gDebugTexture.Sample(gSamLinear, input.uv);
}