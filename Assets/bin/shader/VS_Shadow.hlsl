#include "header\H_Input.hlsli"
#include "header\H_Const.hlsli"

struct VSShadowOutput
{
    float4 posH : SV_POSITION;
    float2 uv : TEXCOORD0;
};

VSShadowOutput main(VSInput input)
{
    VSShadowOutput output;

#if !defined(SKIN)

    float4 world = mul(float4(input.posL, 1.0f), _world);
    float4 view = mul(_world, _camera._view);
    output.posH = mul(view, _camera._proj);

#else   //SKIN

    float3 posL = float3(0.0f, 0.0f, 0.0f);

        [unroll]
    for (int i = 0; i < BONECNT; i++)
    {
        if (input.bone[i] >= 0)
        {
            posL += input.weight[i] * mul(float4(input.posL, 1.0f), boneTransforms[input.bone[i]]).xyz;
        }
    }

    float4 world = mul(float4(_posL, 1.0f), _world);
    float4 view = mul(_world, _camera._view);
    output.posH = mul(view, _camera._proj);

#endif
    output.uv = input.uv;

    return output;
}