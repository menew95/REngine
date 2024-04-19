#include "header\H_Input.hlsli"

#define COLORSPACE_GAMMA

#include "header\H_Color.hlsli"

SamplerState gSamWrapLinearMipPoint : register(s0);

TextureCube gTexture;// : register(t0);

cbuffer PerMaterial: register(b2)
{
	float4 _tex_HDR = float4(1.0f, 1.0f, 0.0f, 0.0f);
    float4 _tint = float4(.5f, .5f, .5f, .5f);
    float _exposure = 1.0f;
    float _rotation = 0.f;
}

float4 main(VSOutput input) : SV_Target
{
	float4 tex = gTexture.Sample(gSamWrapLinearMipPoint, input.posW.xyz);
	float3 c = DecodeHDR (tex, _tex_HDR);
    
	c = c * _tint.rgb * colorSpaceDouble.rgb;
    c *= _exposure;

	return float4(c, 1.f);
}