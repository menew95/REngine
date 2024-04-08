#include "header\H_Standard.hlsli"

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
struct PSShadowInput
{
    float4 posH : SV_POSITION;
    float2 uv : TEXCOORD0;
    uint RTIndex : SV_RenderTargetArrayIndex;
};

void main(PSShadowInput input)
{
    float4 albedo = AlbedoMap.Sample(samWrapLinear, input.uv);
    bool isAlpha = (_albedoColor.w < 0.8f);
    bool isAlpha2 = (albedo.w < 0.8f);
    
    if (isAlpha && isAlpha2)
    {
          clip(-1.f);
    }

    return;
}