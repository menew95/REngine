#include "header\H_Input.hlsli"

struct VSIBLOutput
{
	float4 posW		: SV_POSITION;
	float2 uv		: TEXCOORD0;
};

VSIBLOutput main(VSInput input)
{
	VSIBLOutput output;
	
	output.posW = float4(input.posL * 2, 1.0f);
	output.uv = input.uv;

	return output;
}