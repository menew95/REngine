#include "header\H_Input.hlsli"
#include "header\H_Const.hlsli"

VSOutput main(VSInput input)
{
    VSOutput output;

    output.posW = float4(input.posL, 1.0f) * 1000.f;
    output.posV = mul(output.posW, _camera._view);
    output.posH = mul(float4(input.posL, 1.0f), _camera._worldViewProj);
    
    // output.posV = mul(output.posW, _camera._view);
    // output.posH = mul(output.posV, _camera._proj);

    return output;
}