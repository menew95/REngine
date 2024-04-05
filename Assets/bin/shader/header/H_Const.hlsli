#include "header\H_Struct.hlsli"
#include "header\H_Light.hlsli"

#if !defined(H_CONST)
#define H_CONST

cbuffer PerFrame : register(b0)
{
    Camera _camera;
    PostProcess _postProcess;

    Light _light[MAX_LIGHT_CNT];
    int _lightCnt;
};

cbuffer PerObject : register(b1)
{
    float2 _objectID;
    float _renderFlag;
    float _offset; // �ż��� �߰�. PaperBurn �� ���� Offset��.
    matrix _world;
    matrix _worldInvTranspose;
}

cbuffer Debug : register(b5)
{
    // 격자 두께 설정
    float _gridThickness;
    float3 _gridColor;
}
#endif