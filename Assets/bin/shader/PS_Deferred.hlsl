#include "header\H_Input.hlsli"
#include "header\H_Const.hlsli"
#include "header\H_Standard.hlsli"
#include "header\H_GBuffer.hlsli"


PSOutput main(VSOutput input)
{
	PSOutput _out;

	// float3 _albedoSample = Albedo(input.uv); 

	// float3 _normalTangentSample = NormalInTangentSpace(input.uv);
	// float3 _normalWorld = TangentNormalToWorldNormal(input.tangentToWorld, _normalTangentSample);

	// float2 _metallicRoughnessSample = MetallicRough(input.uv);

	// float _aoSample = AmbeintOcclution(input.uv);

	// float3 _emissionSample = Emission(input.uv);

	// _out.Albedo = float4(_albedoSample, _metallicRoughnessSample.r);

	// _out.Normal = float4((_normalWorld * 0.5 + 0.5), _metallicRoughnessSample.g);

	// _out.World = float4(input.posW.xyz, _aoSample);

	// _out.Emissive = float4(float3(_emissionSample.rgb), input.posV.z);

	// _out.Flags = float4(0.f, 0.f, 0.f, 0.f);

	StandardData _data;
    _data._diffuseColor = Albedo(input.uv); 
	_data._emissiveColor = Emission(input.uv);
	_data._metallicRoughness = MetallicRough(input.uv);

	float3 _normalTangentSample = NormalInTangentSpace(input.uv);
	_data._normalWorld = TangentNormalToWorldNormal(input.tangentToWorld, _normalTangentSample);
	_data._posVDepth = input.posV.z;

	_data._worldPosition = input.posW.xyz;
	_data._ambientOcclussion = AmbeintOcclution(input.uv);

	_data._flags = float4(0.f, 0.f, 0.f, 0.f);

	StandardDataToGBuffer(_data, _out.Albedo, _out.Emissive, _out.Normal, _out.World, _out.Flags);

	return _out;
}