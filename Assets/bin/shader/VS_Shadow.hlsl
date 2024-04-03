#include "header\H_Input.hlsli"
#include "header\H_Const.hlsli"

void main(VSInput input, out float4 posH : SV_Position)
{
    VSOutput _output;


#if !defined(SKIN)
    float4 _world = mul(float4(input.posL, 1.0f), _world);
    float4 _view = mul(_world, _camera._view);
    posH = mul(_view, _camera._proj);
#else   //SKIN
    float3 _posL = float3(0.0f, 0.0f, 0.0f);

        [unroll]
    for (int i = 0; i < BONECNT; i++)
    {
        if (input.bone[i] >= 0)
        {
            _posL        += input.weight[i] * mul(float4(input.posL, 1.0f), boneTransforms[input.bone[i]]).xyz;
        }
    }

    float4 _world = mul(float4(_posL, 1.0f), _world);
    float4 _view = mul(_world, _camera._view);
    posH = mul(_view, _camera._proj);
#endif
}