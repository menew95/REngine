#include "header\H_Const.hlsli"
#include "header\H_Input.hlsli"

cbuffer PerLight : register(b2)
{
    //int _current_light_idx;
    uint _type; 
    float3 _direction;
    
    float3 _position;
    float _attenuationRadius;
    
    float3 _color;
    float _intensity;
  
    float _angle;
    float _innerAngle;
    float _fallOffExponential;
    float _width;

    float3 _up;
    float _height;

    int _staticShadowMapIdx;
    int _dynamicShadowMapIdx;
    uint _shadowState;
    int _pad0;

    matrix _shadowMatrix[6]; 

    float2 _uv0[6];
    float2 _uv1[6];
}

struct VSShadowOutput
{
    float4 posW : SV_POSITION;
    float2 uv : TEXCOORD0;
};

struct GSOutput
{
    float4 posH : SV_POSITION;
    float2 uv : TEXCOORD0;
    uint RTIndex : SV_RenderTargetArrayIndex;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

void VSMain(VSInput input
    , out VSShadowOutput output
#if USING_VERTEX_SHADER_LAYER 
    , out uint LayerIndex : SV_RenderTargetArrayIndex
	, out float ClipDistance : SV_ClipDistance0
#endif
)
{
#if !defined(SKIN)

    output.posW = mul(float4(input.posL, 1.0f), _world);

#else   //SKIN

    float3 posL = float3(0.0f, 0.0f, 0.0f);

        [unroll]
    for (int i = 0; i < BONECNT; i++)
    {
        if (input.bone[i] >= 0)
        {
            posL += input.weight[i] * mul(float4(input.posL, 1.0f), _boneTransforms[input.bone[i]]).xyz;
        }
    }

    output.posW = mul(float4(posL, 1.0f), _world);

#endif
    output.uv = input.uv;

#if defined(SPOT_LIGHT)
    output.posW = mul(output.posW, _shadowMatrix[0])
#endif

#if USING_VERTEX_SHADER_LAYER
    LayerIndex = Input.InstanceId / InstanceCount;
	OutPosition = mul(WorldPos, _shadowMatrix[LayerIndex]);
	ClipDistance = (MeshVisibleToFace[LayerIndex].x > 0) ? 1.0 : -1.0;
#endif
}

//--------------------------------------------------------------------------------------
// Geometry Shader
//--------------------------------------------------------------------------------------
[maxvertexcount(12)] // 4(shadow slice) * 3
void CascadeGSMain(triangle VSShadowOutput input[3], inout TriangleStream<GSOutput> stream)
{
    for (int cascade = 0; cascade < 4; ++cascade)
    {
        float4 ClipSpacePositions[3];

		[unroll]
		for (int VertexIndex = 0; VertexIndex < 3; VertexIndex++)
		{
			// Calculate the clip space position for each cube face
			ClipSpacePositions[VertexIndex] = mul(input[VertexIndex].posW, _shadow._shadowTransform[cascade]);
		}

		float4 FrustumTests0 = saturate(ClipSpacePositions[0].xyxy * float4(-1, -1, 1, 1) - ClipSpacePositions[0].w);		
		float4 FrustumTests1 = saturate(ClipSpacePositions[1].xyxy * float4(-1, -1, 1, 1) - ClipSpacePositions[1].w);		
		float4 FrustumTests2 = saturate(ClipSpacePositions[2].xyxy * float4(-1, -1, 1, 1) - ClipSpacePositions[2].w);		
		float4 FrustumTests = FrustumTests0 * FrustumTests1 * FrustumTests2;

        [branch]	
		// Frustum culling, saves GPU time with high poly meshes
		if (!any(FrustumTests != 0))
        {
            GSOutput output;

            output.RTIndex = cascade;

			[unroll]
			for (int VertexIndex = 0; VertexIndex < 3; VertexIndex++)
			{
				output.posH = ClipSpacePositions[VertexIndex];
                output.uv = input[VertexIndex].uv;

				stream.Append(output);
			}

			stream.RestartStrip();
        }

        // //Generate cascade slice
        // GSOutput output;
        // output.RTIndex = cascade;

        // for (int i = 0; i < 3; ++i)
        // {
        //     output.posH = mul(input[i].posW, _shadow._shadowTransform[cascade]);
        //     output.uv = input[i].uv;

        //     stream.Append(output);
        // }

        // stream.RestartStrip();
    }
}

// 사용하지 않음
[maxvertexcount(3)] // 1(shadow slice) * 3
void SpotGSMain(triangle VSShadowOutput input[3], inout TriangleStream<GSOutput> stream)
{
    for (int i = 0; i < 3; ++i)
    {
        GSOutput output;

        output.RTIndex = 0;

        output.posH = mul(input[i].posW, _shadowMatrix[0]);
        output.uv = input[i].uv;

        stream.Append(output);
    }

    stream.RestartStrip();
}

[maxvertexcount(18)] // 6(shadow slice) * 3
void PointGSMain(triangle VSShadowOutput input[3], inout TriangleStream<GSOutput> stream)
{
    for (int faceIdx = 0; faceIdx < 6 ;faceIdx++)
    {
        float4 ClipSpacePositions[3];
		[unroll]
		for (int VertexIndex = 0; VertexIndex < 3; VertexIndex++)
		{
			// Calculate the clip space position for each cube face
			ClipSpacePositions[VertexIndex] = mul(input[VertexIndex].posW, _shadowMatrix[faceIdx]);
		}

		float4 FrustumTests0 = saturate(ClipSpacePositions[0].xyxy * float4(-1, -1, 1, 1) - ClipSpacePositions[0].w);		
		float4 FrustumTests1 = saturate(ClipSpacePositions[1].xyxy * float4(-1, -1, 1, 1) - ClipSpacePositions[1].w);		
		float4 FrustumTests2 = saturate(ClipSpacePositions[2].xyxy * float4(-1, -1, 1, 1) - ClipSpacePositions[2].w);		
		float4 FrustumTests = FrustumTests0 * FrustumTests1 * FrustumTests2;

        [branch]	
		// Frustum culling, saves GPU time with high poly meshes
		if (!any(FrustumTests != 0))		
		{				
            GSOutput output;

            output.RTIndex = faceIdx;

			[unroll]
			for (int VertexIndex = 0; VertexIndex < 3; VertexIndex++)
			{
				output.posH = ClipSpacePositions[VertexIndex];
                output.uv = input[VertexIndex].uv;

				stream.Append(output);
			}

			stream.RestartStrip();
		}
    }
}