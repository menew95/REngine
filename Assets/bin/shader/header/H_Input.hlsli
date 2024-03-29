#if !defined(H_INPUT)

#define H_INPUT

#if !defined(BONECNT)
	#define BONECNT 4
#endif

struct VSInput
{
#if defined(SCREEN)
	float3	posL			: POSITION;
	float2	uv				: TEXCOORD;
#else
	float3	posL			: POSITION;
	float4	color			: COLOR;
	float2	uv				: TEXCOORD;
	float3	normal			: NORMAL;
	float3	tangent			: TANGENT;
    uint	bone[BONECNT]   : BONE;
    float	weight[BONECNT] : WEIGHT;
#endif
};

struct VSOutput
{
	float4 posH		: SV_POSITION;
	float4 posW		: POSITION1;
	float4 posV		: POSITION2;
	float4 color	: COLOR;
	float3 normal	: NORMAL;
	float2 uv		: TEXCOORD1;

    float3 tangentToWorld[3] : TEXCOORD2;

#if defined(INSTANCING)
	float4 albedoColor : COLOR1;
	float4 emissiveColor : COLOR2;
	float4 pbr : COLOR3;
#endif
};

struct PSOutput
{
	float4 Albedo	: SV_TARGET0;
	float4 Normal	: SV_TARGET1;
	float4 World	: SV_TARGET2;
	float4 Emissive	: SV_TARGET3;
	float4 Flags	: SV_TARGET4;
};

#if defined(HARDWARE)
struct VSInstanceInput
{
	float4x4	_world		: INSTANCE_WORLD;
	float4x4	_worldInv	: INSTANCE_WORLDINV;

	// material property
	float4		_albedo		: INSTANCE_ALBEDO;
	float4		_emissive	: INSTANCE_EMISSIVE;
	float4		_pbr		: INSTANCE_PBR;
};
#endif

#endif // H_INPUT