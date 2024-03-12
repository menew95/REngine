#include "header\H_Const.hlsli"
#include "header\H_Input.hlsli"

float4 main(VSOutput input) : SV_TARGET
{
    float4 _output;
    
    // 픽셀의 월드 좌표를 정규화
    float4 uv = abs(frac(input.posW)); 

    // 가로 및 세로 격자 라인을 생성    
    float _horizontalLine = smoothstep(0.0f, _gridThickness, uv.z);
    float _verticalLine = smoothstep(0.0f, _gridThickness, uv.x);

    clip(_horizontalLine < 1.0f || _verticalLine < 1.0f ? 1.0 : -1.0f);

    _output = float4(_gridColor, 1.0f);

    return float4(_gridColor, 1.0f);
}

// struct VSOutput
// {
//     float4 position : SV_POSITION;
// };

// struct PSOutput
// {
//     float4 color : SV_TARGET;
// };

// PSOutput main(VSOutput input)
// {
//     PSOutput output;

//     // 픽셀의 UV 좌표 계산
//     float2 uv = abs(frac(input.position.xy));

//     // 격자 두께 내의 픽셀에 대해서만 색상 설정
//     if (uv.x < gridThickness || uv.y < gridThickness)
//     {
//         output.color = float4(0.7f, 0.7f, 0.7f, 1.0f); // 격자 색상
//     }
//     else
//     {
//         output.color = float4(0.0f, 0.0f, 0.0f, 0.0f); // 투명
//     }

//     return output;
// }