#include "header\H_Input.hlsli"
#include "header\H_Const.hlsli"

VSOutput main(VSInput input)
{
    VSOutput _output;

    _output.posW = float4(input.posL, 1.0f);
    _output.posV = mul(_output.posW, _camera._view);
    _output.posH = mul(_output.posV, _camera._worldViewProj);

    // _output.posV = mul(_output.posW, _camera._view);
    // _output.posH = mul(_output.posV, _camera._proj);

    return _output;
}