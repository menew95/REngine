#include "header\H_Const.hlsli"
#include "header\H_Input.hlsli"
#include "header\H_GBuffer.hlsli"
#include "header\H_PBR.hlsli"

Texture2D _CameraGBufferTexture0    : register(t0);
Texture2D _CameraGBufferTexture1    : register(t1);
Texture2D _CameraGBufferTexture2    : register(t2);
Texture2D _CameraGBufferTexture3    : register(t3);
Texture2D _CameraGBufferTexture4	: register(t4);

TextureCube gPreFilteredMap : register(t5);
TextureCube gIrradianceMap : register(t6);
Texture2D gIntegrateBRDFMap : register(t7);

Texture2DArray gCascadedShadowMap : register(t8);
Texture2DArray gSpotShadowMap : register(t9);
TextureCubeArray gPointShadowMap : register(t10);

Texture2D gReflect : register(t11);

SamplerState samWrapLinear	: register(s0);

SamplerComparisonState samShadowSampler : register(s1);

float4 main(VSOutput input) : SV_TARGET
{
	float4 _gbuffer0 = _CameraGBufferTexture0.Sample(samWrapLinear, input.uv);
	float4 _gbuffer1 = _CameraGBufferTexture1.Sample(samWrapLinear, input.uv);
	float4 _gbuffer2 = _CameraGBufferTexture2.Sample(samWrapLinear, input.uv);
	float4 _gbuffer3 = _CameraGBufferTexture3.Sample(samWrapLinear, input.uv);
	float4 _gbuffer4 = _CameraGBufferTexture4.Sample(samWrapLinear, input.uv);


	StandardData _data = StandardDataFronGBuffer(_gbuffer0, _gbuffer1, _gbuffer2, _gbuffer3, _gbuffer4);

    float4 _color;

	return _color;
}