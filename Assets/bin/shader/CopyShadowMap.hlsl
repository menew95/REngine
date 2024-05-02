#include "header\H_Input.hlsli"

Texture2D ShadowDepthTexture;
SamplerState ShadowDepthSampler;

float4 Texture2DSampleLevel(Texture2D Tex, SamplerState Sampler, float2 UV, float Mip)
{
	return Tex.SampleLevel(Sampler, UV, Mip);
}

float4 TextureCubeSampleLevel(TextureCube Tex, SamplerState Sampler, float3 UV, float Mip)
{
	return Tex.SampleLevel(Sampler, UV, Mip);
}

void Copy2DDepthPS(
	VSOutput Input,
	out float OutDepth : SV_DEPTH,
	out float4 OutColor : SV_Target0
	)
{
	OutColor = 0;
	OutDepth = Texture2DSampleLevel(ShadowDepthTexture, ShadowDepthSampler, Input.uv, 0).x;
}

struct FCubeCopyGSToPS
{
	float4 Position : SV_POSITION;
    noperspective float2 UV : TEXCOORD0;

	nointerpolation uint FaceIndex : TEXCOORD1;

	/** Controls which of the cube map faces to rasterize the primitive to, only the value from the first vertex is used. */
	nointerpolation uint RTIndex : SV_RenderTargetArrayIndex;
};
 
/** Allocate space for cloning to all 6 faces. */
[maxvertexcount(18)]
void CopyCubeDepthGS(triangle VSOutput Input[3], inout TriangleStream<FCubeCopyGSToPS> OutStream)
{
	[unroll]
	// Clone the triangle to each face
    for (uint CubeFaceIndex = 0; CubeFaceIndex < 6; CubeFaceIndex++)
    {
		FCubeCopyGSToPS Output;
		Output.RTIndex = CubeFaceIndex;
		Output.FaceIndex = CubeFaceIndex;

		[unroll]
		for (uint VertexIndex = 0; VertexIndex < 3; VertexIndex++)
		{
			Output.Position = Input[VertexIndex].posH;
            Output.UV = Input[VertexIndex].uv;
			OutStream.Append(Output);
		}
		OutStream.RestartStrip();
	}
}

float3 GetCubemapVector(float2 ScaledUVs, uint CubeFace)
{
	float3 CubeCoordinates;

	if (CubeFace == 0)
	{
		CubeCoordinates = float3(1, -ScaledUVs.y, -ScaledUVs.x);
	}
	else if (CubeFace == 1)
	{
		CubeCoordinates = float3(-1, -ScaledUVs.y, ScaledUVs.x);
	}
	else if (CubeFace == 2)
	{
		CubeCoordinates = float3(ScaledUVs.x, 1, ScaledUVs.y);
	}
	else if (CubeFace == 3)
	{
		CubeCoordinates = float3(ScaledUVs.x, -1, -ScaledUVs.y);
	}
	else if (CubeFace == 4)
	{
		CubeCoordinates = float3(ScaledUVs.x, -ScaledUVs.y, 1);
	}
	else
	{
		CubeCoordinates = float3(-ScaledUVs.x, -ScaledUVs.y, -1);
	}

	return CubeCoordinates;
}

TextureCube ShadowDepthCubeTexture;

void CopyCubeDepthPS(
	FCubeCopyGSToPS Input,
	out float OutDepth : SV_DEPTH,
	out float4 OutColor : SV_Target0
	)
{ 
	float2 ScaledUVs = Input.UV * 2 - 1;
	float3 CubeCoordinates = GetCubemapVector(ScaledUVs, Input.FaceIndex);

	OutColor = 0;
	OutDepth = TextureCubeSampleLevel(ShadowDepthCubeTexture, ShadowDepthSampler, CubeCoordinates, 0).x;
}