#include "header\H_Const.hlsli"
#include "header\H_Input.hlsli"

float4 main(VSOutput input) : SV_TARGET
{
    float4 output;
    
    // 픽셀의 월드 좌표를 정규화
    float4 uv = abs(frac(input.posW)); 

    // 가로 및 세로 격자 라인을 생성    
    float horizontalLine = smoothstep(0.0f, _gridThickness, uv.z);
    float verticalLine = smoothstep(0.0f, _gridThickness, uv.x);

    clip(horizontalLine < 1.0f || verticalLine < 1.0f ? 1.0 : -1.0f);

    output = float4(_gridColor, 1.0f);

    return float4(_gridColor, 1.0f);
}