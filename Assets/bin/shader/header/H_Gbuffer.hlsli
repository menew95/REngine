#if !defined(H_GBUFFER)
#define H_GBUFFER

struct StandardData
{
    float3 _diffuseColor;
    float3 _emissiveColor;
    float2 _metallicRoughness;
    
    float3 _normalWorld;
    float _posVDepth;
    
    float3 _worldPosition;
    float _ambientOcclussion;

    uint _flags;

};

void StandardDataToGBuffer(StandardData data, out float4 GBuffer0, out float4 GBuffer1, out float4 GBuffer2, out float4 GBuffer3, out uint GBuffer4)
{
    GBuffer0.xyz = data._diffuseColor;
    GBuffer0.w = data._metallicRoughness.r;
    
    GBuffer1.xyz = data._emissiveColor;
    GBuffer1.w = data._metallicRoughness.g;

    GBuffer2.xyz = data._normalWorld * 0.5f + 0.5f;
    GBuffer2.w = data._posVDepth;
    
    GBuffer3.xyz = data._worldPosition;
    GBuffer3.w = data._ambientOcclussion;
    
    GBuffer4 = data._flags;
}

StandardData StandardDataFronGBuffer(float4 GBuffer0, float4 GBuffer1, float4 GBuffer2, float4 GBuffer3, uint GBuffer4)
{
    StandardData _data;

    _data._diffuseColor = GBuffer0.xyz;
    _data._metallicRoughness.r = GBuffer0.w;

    _data._emissiveColor = GBuffer1.xyz;
    _data._metallicRoughness.g = GBuffer1.w;

    _data._normalWorld = normalize(GBuffer2.xyz * 2.0f - 1.0f);
    _data._posVDepth = GBuffer2.w;

    _data._worldPosition = GBuffer3.xyz;
    _data._ambientOcclussion = GBuffer3.w;

    _data._flags = GBuffer4;

    return _data;
}

#endif