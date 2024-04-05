struct VSInput
{
	float3 posL		: POSITION;
	float2 uv		: TEXCOORD0;
};

struct VSOutput
{
	float4 posW		: SV_POSITION;
	float2 uv		: TEXCOORD0;
};

VSOutput main(VSInput input)
{
	VSOutput output;
	
	output.posW = float4(input.posL, 1.0f);
	output.uv = input.uv;

	return output;
}