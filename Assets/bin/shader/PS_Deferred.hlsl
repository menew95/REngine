#include "header\H_Input.hlsli"
#include "header\H_Const.hlsli"
#include "header\H_Standard.hlsli"
#include "header\H_GBuffer.hlsli"


PSOutput main(VSOutput input)
{
	PSOutput output;

	// float3 _albedoSample = Albedo(input.uv); 

	// float3 normalTangentSample = NormalInTangentSpace(input.uv);
	// float3 _normalWorld = TangentNormalToWorldNormal(input.tangentToWorld, normalTangentSample);

	// float2 _metallicRoughnessSample = MetallicRough(input.uv);

	// float _aoSample = AmbeintOcclution(input.uv);

	// float3 _emissionSample = Emission(input.uv);

	// _out.Albedo = float4(_albedoSample, _metallicRoughnessSample.r);

	// _out.Normal = float4((_normalWorld * 0.5 + 0.5), _metallicRoughnessSample.g);

	// _out.World = float4(input.posW.xyz, _aoSample);

	// _out.Emissive = float4(float3(_emissionSample.rgb), input.posV.z);

	// _out.Flags = float4(0.f, 0.f, 0.f, 0.f);

	StandardData data;
    data._diffuseColor = Albedo(input.uv); 
	data._emissiveColor = Emission(input.uv);
	data._metallicRoughness = MetallicRough(input.uv);

	float3 normalTangentSample = NormalInTangentSpace(input.uv);
	data._normalWorld = TangentNormalToWorldNormal(input.tangentToWorld, normalTangentSample);
	data._posVDepth = input.posV.z;

	data._worldPosition = input.posW.xyz;
	data._ambientOcclussion = AmbeintOcclution(input.uv);

	data._flags = float4(_objectID.xy, _renderFlag, 0.f);

	StandardDataToGBuffer(data, output.Albedo, output.Emissive, output.Normal, output.World, output.Flags);

	return output;
}