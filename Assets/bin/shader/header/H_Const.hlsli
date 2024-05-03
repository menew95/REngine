#include "header\H_Struct.hlsli"
#include "header\H_Light.hlsli"

#if !defined(H_CONST)
#define H_CONST

cbuffer PerFrame : register(b0)
{
    uint _lightCnt = 0;
    float3 _pad;

    // direction light;
    float4 _WorldSpaceLightPos0;
    float4 _LightColor0;
}

cbuffer PerCamera : register(b1)
{
    Camera _camera;

    CascadeShadow _shadow;

    float4 _time; // (t/20, t, t*2, t*3)
    float4 _sinTime; // sin(t/8), sin(t/4), sin(t/2), sin(t)
    float4 _cosTime; // cos(t/8), cos(t/4), cos(t/2), cos(t)
    float4 _deltaTime; // dt, 1/dt, smoothdt, 1/smoothdt

     // x = width
    // y = height
    // z = 1 + 1.0/width
    // w = 1 + 1.0/height
    float4 _screen;

    /*
        DX:
        EZ  = (n * f) / (f - z * (f - n))
        LZ  = (eyeZ - n) / (f - n) = z / (f - z * (f - n))
        LZ2 = eyeZ / f = n / (f - z * (f - n))
    */
    // Values used to linearize the Z buffer (http://www.humus.name/temp/Linearize%20depth.txt)
    // x = 1-far/near
    // y = far/near
    // z = x/far
    // w = y/far
    float4 _ZBufferParams;
}

cbuffer PerObject : register(b2)
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