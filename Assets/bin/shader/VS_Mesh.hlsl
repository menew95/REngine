#include "header\H_Input.hlsli"
#include "header\H_Const.hlsli"
#include "header\H_Utility.hlsli"

#if !defined(INSTANCING)
VSOutput main(VSInput input)
{
    VSOutput output;

#if defined(SCREEN)
    output.posH = float4(input.posL.xy, 1.0f, 1.0f);
    output.normal = float4(1.0f, 1.0f, 1.0f, 1.0f);
#else   // SCREEN

#if !defined(SKIN)
    output.posW = mul(float4(input.posL, 1.0f), _world);
    output.posV = mul(output.posW, _camera._view);
    output.posH = mul(output.posV, _camera._proj);

    output.normal = normalize(mul(input.normal, (float3x3)_worldInvTranspose));

    float3 tangentWorld = normalize(mul(input.tangent, (float3x3) _world));
    float3x3 tangentToWorld = CreateTangentToWorldPerVertex(output.normal, tangentWorld.xyz);

    output.tangentToWorld[0] = tangentToWorld[0];
    output.tangentToWorld[1] = tangentToWorld[1];
    output.tangentToWorld[2] = tangentToWorld[2];

#else   //SKIN
    float3 posL = float3(0.0f, 0.0f, 0.0f);
    float3 normalL = float3(0.0f, 0.0f, 0.0f);

    float3 tangentL = normalize(mul(input.tangent, (float3x3)_world));

        [unroll]
    for (int i = 0; i < BONECNT; i++)
    {
        if (input.bone[i] >= 0)
        {
            posL        += input.weight[i] * mul(float4(input.posL, 1.0f), boneTransforms[input.bone[i]]).xyz;
            normalL     += input.weight[i] * mul(input.normal, (float3x3)boneTransforms[input.bone[i]]).xyz;
            tangentL    += input.weight[i] * mul(input.tangent, (float3x3)boneTransforms[input.bone[i]]).xyz;
        }
    }

    output.posW = float4(_posL, 1.0f);
    output.posV = mul(output.posW, _camera._view);
    output.posH = mul(output.posV, _camera._proj);

    output.normal = normalize(_normalL);

    float4 tangentWorld = normalize(mul(_tangentL, (float3x3)_world));
    float3x3 tangentToWorld = CreateTangentToWorldPerVertex(output.normal, tangentWorld.xyz);

    output.tangentToWorld[0] = tangentToWorld[0];
    output.tangentToWorld[1] = tangentToWorld[1];
    output.tangentToWorld[2] = tangentToWorld[2];
#endif  //SKIN

    output.color = input.color;
#endif  // SCREEN

    output.uv = input.uv;

    return output;
}
#else




#endif