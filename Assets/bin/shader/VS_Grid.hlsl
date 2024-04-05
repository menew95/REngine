#include "header\H_Const.hlsli"
#include "header\H_Input.hlsli"

VSOutput main(VSInput input)
{
    VSOutput output;

    // 입력된 정점 위치를 변환하여 출력
    output.posW = float4(input.posL.x, 0.0f, input.posL.y, 0.f) * 100.0f + float4(_camera._world.x, 0.0f, _camera._world.z, 1.0f);
    output.posV = mul(output.posW, _camera._view);
    output.posH = mul(output.posV, _camera._proj);

    return output;
}

// VSOutput main(uint vertexID : SV_VertexID)
// {
//     VSOutput output;

//     // 버텍스 ID를 이용하여 버텍스의 x, y 좌표 계산
//     float x = vertexID % 100; // 가로 크기 (1000)
//     float y = vertexID / 100; // 세로 크기 (1000)

//     // 정규화된 좌표로 변환
//     output.position = mul(float4(x, 0, y, 1.0f), _camera._worldViewProj);

//     return output;
// }