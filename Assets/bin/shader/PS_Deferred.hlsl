#include "header\H_Input.hlsli"
#include "header\H_Const.hlsli"
#include "header\H_Standard.hlsli"
#include "header\H_GBuffer.hlsli"


PSOutput main(VSOutput input)
{
	PSOutput output;

	StandardData data;
    data._diffuseColor = Albedo(input.uv); 
	data._emissiveColor = Emission(input.uv);
	data._metallicRoughness = MetallicRough(input.uv);

	float3 normalTangentSample = NormalInTangentSpace(input.uv);
	data._normalWorld = TangentNormalToWorldNormal(input.tangentToWorld, normalTangentSample);
	data._posVDepth = input.posV.z;

	data._worldPosition = input.posW.xyz;
	data._ambientOcclussion = AmbeintOcclution(input.uv);

	data._flags = float4(_objectID.xy, _renderFlag, _renderFlag2);

	StandardDataToGBuffer(data, output.Albedo, output.Emissive, output.Normal, output.World, output.Flags);

	return output;
}