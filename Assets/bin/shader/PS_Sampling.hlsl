SamplerState gSamLinear : register(s0);

Texture2D gRenderTargetTexture;

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
};

float4 main(VertexOut pin) : SV_Target
{
    float4 color = gRenderTargetTexture.Sample(gSamLinear, pin.Texcoord0);

    return color;
}