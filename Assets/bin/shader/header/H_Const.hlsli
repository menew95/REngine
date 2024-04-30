#include "header\H_Struct.hlsli"
#include "header\H_Light.hlsli"

#if !defined(H_CONST)
#define H_CONST

cbuffer PerFrame : register(b0)
{
    Camera _camera;
    PostProcess _postProcess;

    uint _lightCnt = 0;

    // direction light;
    float4 _WorldSpaceLightPos0;
    float4 _LightColor0;

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

#define BONE_MAX_CNT 128

cbuffer PerSkin : register(b3)
{
    matrix _boneTransforms[BONE_MAX_CNT];
}

cbuffer Debug : register(b5)
{
    // 격자 두께 설정
    float _gridThickness;
    float3 _gridColor;
}
#endif