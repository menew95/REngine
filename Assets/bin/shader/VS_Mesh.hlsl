#include "header\H_Input.hlsli"
#include "header\H_Const.hlsli"
#include "header\H_Utility.hlsli"

#if !defined(INSTANCING)
VSOutput main(VSInput input)
{
    VSOutput _output;

#if defined(SCREEN)
    _output.posH = float4(input.posL.xy, 1.0f, 1.0f);
    _output.normal = float4(1.0f, 1.0f, 1.0f, 1.0f);
#else   // SCREEN

#if !defined(SKIN)
    _output.posW = mul(float4(input.posL, 1.0f), _world);
    _output.posV = mul(_output.posW, _camera._view);
    _output.posH = mul(_output.posV, _camera._proj);

    _output.normal = normalize(mul(input.normal, (float3x3)_worldInvTranspose));

    float3 _tangentWorld = normalize(mul(input.tangent, (float3x3) _world));
    float3x3 _tangentToWorld = CreateTangentToWorldPerVertex(_output.normal, _tangentWorld.xyz);

    _output.tangentToWorld[0] = _tangentToWorld[0];
    _output.tangentToWorld[1] = _tangentToWorld[1];
    _output.tangentToWorld[2] = _tangentToWorld[2];

#else   //SKIN
    float3 _posL = float3(0.0f, 0.0f, 0.0f);
    float3 _normalL = float3(0.0f, 0.0f, 0.0f);

    float3 _tangentL = normalize(mul(input.tangent, (float3x3)_world));

        [unroll]
    for (int i = 0; i < BONECNT; i++)
    {
        if (input.bone[i] >= 0)
        {
            _posL        += input.weight[i] * mul(float4(input.posL, 1.0f), boneTransforms[input.bone[i]]).xyz;
            _normalL     += input.weight[i] * mul(input.normal, (float3x3)boneTransforms[input.bone[i]]).xyz;
            _tangentL    += input.weight[i] * mul(input.tangent, (float3x3)boneTransforms[input.bone[i]]).xyz;
        }
    }

    _output.posW = float4(_posL, 1.0f);
    _output.posV = mul(_output.posW, _camera._view);
    _output.posH = mul(_output.posV, _camera._proj);

    _output.normal = normalize(_normalL);

    float4 _tangentWorld = normalize(mul(_tangentL, (float3x3)_world));
    float3x3 _tangentToWorld = CreateTangentToWorldPerVertex(_output.normal, _tangentWorld.xyz);

    _output.tangentToWorld[0] = _tangentToWorld[0];
    _output.tangentToWorld[1] = _tangentToWorld[1];
    _output.tangentToWorld[2] = _tangentToWorld[2];
#endif  //SKIN

    _output.color = input.color;
#endif  // SCREEN

    _output.uv = input.uv;

    return _output;
}
#else




#endif