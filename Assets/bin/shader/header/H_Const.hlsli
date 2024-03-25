#include "header\H_Struct.hlsli"

#if !defined(H_CONST)
#define H_CONST

cbuffer PerFrame : register(b0)
{
    Camera _camera;
    PostProcess _postProcess;
};

cbuffer PerObject : register(b1)
{
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