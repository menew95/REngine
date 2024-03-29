#include "header\H_Input.hlsli"

SamplerState samWrapLinearMipPoint : register(s0);

TextureCube g_SkyBox;// : register(t0);

float4 main(VSOutput input) : SV_Target
{
	return g_SkyBox.Sample(samWrapLinearMipPoint, input.posW.xyz);
}