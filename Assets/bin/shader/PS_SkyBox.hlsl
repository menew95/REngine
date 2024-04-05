#include "header\H_Input.hlsli"

SamplerState gSamWrapLinearMipPoint : register(s0);

TextureCube gSkyBox;// : register(t0);

float4 main(VSOutput input) : SV_Target
{
	return gSkyBox.Sample(gSamWrapLinearMipPoint, input.posW.xyz);
}