#include "header\H_Const.hlsli"
#include "header\H_Input.hlsli"
#include "header\H_GBuffer.hlsli"
#include "header\H_PBR.hlsli"

Texture2D gCameraGBufferTexture0    : register(t0); // albedo + metallic
Texture2D gCameraGBufferTexture1    : register(t1);	// normal + roughness
Texture2D gCameraGBufferTexture2    : register(t2);	// world + v depth
Texture2D gCameraGBufferTexture3    : register(t3); // emission + ao
Texture2D gCameraGBufferTexture4	: register(t4); // flag
//Texture2D gCameraGBufferTexture5	: register(t5); // depth

//TextureCube gPreFilteredMap : register(t5);
//TextureCube gIrradianceMap : register(t6);
//Texture2D gIntegrateBRDFMap : register(t7);

//Texture2DArray gCascadedShadowMap : register(t8);
//Texture2DArray gSpotShadowMap : register(t9);
//TextureCubeArray gPointShadowMap : register(t10);

//Texture2D gReflect : register(t11);

SamplerState samWrapLinear	: register(s0);

SamplerComparisonState samCompShadow : register(s1);

float4 main(VSOutput input) : SV_TARGET
{
	float4 _gbuffer0 = gCameraGBufferTexture0.Sample(samWrapLinear, input.uv);
	float4 _gbuffer1 = gCameraGBufferTexture1.Sample(samWrapLinear, input.uv);
	float4 _gbuffer2 = gCameraGBufferTexture2.Sample(samWrapLinear, input.uv);
	float4 _gbuffer3 = gCameraGBufferTexture3.Sample(samWrapLinear, input.uv);
	float4 _gbuffer4 = gCameraGBufferTexture4.Sample(samWrapLinear, input.uv);


	StandardData _data = StandardDataFronGBuffer(_gbuffer0, _gbuffer1, _gbuffer2, _gbuffer3, _gbuffer4);

    float4 _color = float4(_data._diffuseColor.xyz, 1.f);

	return _color;
}