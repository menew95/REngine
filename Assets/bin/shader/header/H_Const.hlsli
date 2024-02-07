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

cbuffer PerMaterial : register(b2)
{
    Material _material;
}
#endif