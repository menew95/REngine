#include "header\H_Struct.hlsli"
#include "header\H_Light.hlsli"

#if !defined(H_CONST)
#define H_CONST

cbuffer PerFrame : register(b0)
{
    Camera _camera;
    PostProcess _postProcess;

    uint _lightCnt = 0;

    CascadeShadow _shadow;
}

cbuffer PerObject : register(b1)
{
    float2 _objectID;
    float _renderFlag;
    float _renderFlag2;
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