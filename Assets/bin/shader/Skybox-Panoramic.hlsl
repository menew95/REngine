#include "header\H_Math.hlsli"
#include "header\H_Const.hlsli"
#include "header\H_Color.hlsli"

#include "header\H_Input.hlsli"

SamplerState gSamWrapLinearMipPoint : register(s0);

Texture2D gTexture;

//#define _MAPPING_6_FRAMES_LAYOUT

cbuffer PerMaterial : register(b3)
{
    float4 _MainTex_TexelSize : packoffset(c0) = float4(1 / 3072, 1 / 1536, 1, 1);
    float4 _MainTex_HDR : packoffset(c1) = float4(1., 124., 1., 1.);
    float4 _Tint : packoffset(c2) = float4(.5, .5, .5, .5);
    float _Exposure : packoffset(c3.x)  = 1.0;
    float _Rotation : packoffset(c3.y)  = 0;
    #ifndef _MAPPING_6_FRAMES_LAYOUT
    int _MirrorOnBack : packoffset(c3.z)  = 0;
    int _ImageType : packoffset(c3.w)  = 0; // 360 degree = 0  180 degree = 1 
    //int _Layout = 0; // None = 0, Side by Side = 1, Over Under = 2
    #endif
}

#ifndef _MAPPING_6_FRAMES_LAYOUT
inline float2 ToRadialCoords(float3 coords)
{
    float3 normalizedCoords = normalize(coords);
    float latitude = acos(normalizedCoords.y);
    float longitude = atan2(normalizedCoords.z, normalizedCoords.x);
    float2 sphereCoords = float2(longitude, latitude) * float2(0.5/PI, 1.0/PI);
    return float2(0.5,1.0) - sphereCoords;
}
#endif

#ifdef _MAPPING_6_FRAMES_LAYOUT
inline float2 ToCubeCoords(float3 coords, float3 layout, float4 edgeSize, float4 faceXCoordLayouts, float4 faceYCoordLayouts, float4 faceZCoordLayouts)
{
    // Determine the primary axis of the normal
    float3 absn = abs(coords);
    float3 absdir = absn > float3(max(absn.y,absn.z), max(absn.x,absn.z), max(absn.x,absn.y)) ? 1 : 0;
    // Convert the normal to a local face texture coord [-1,+1], note that tcAndLen.z==dot(coords,absdir)
    // and thus its sign tells us whether the normal is pointing positive or negative
    float3 tcAndLen = mul(absdir, float3x3(coords.zyx, coords.xzy, float3(-coords.xy,coords.z)));
    tcAndLen.xy /= tcAndLen.z;
    // Flip-flop faces for proper orientation and normalize to [-0.5,+0.5]
    bool2 positiveAndVCross = float2(tcAndLen.z, layout.x) > 0;
    tcAndLen.xy *= (positiveAndVCross[0] ? absdir.yx : (positiveAndVCross[1] ? float2(absdir[2],0) : float2(0,absdir[2]))) - 0.5;
    // Clamp values which are close to the face edges to avoid bleeding/seams (ie. enforce clamp texture wrap mode)
    tcAndLen.xy = clamp(tcAndLen.xy, edgeSize.xy, edgeSize.zw);
    // Scale and offset texture coord to match the proper square in the texture based on layout.
    float4 coordLayout = mul(float4(absdir,0), float4x4(faceXCoordLayouts, faceYCoordLayouts, faceZCoordLayouts, faceZCoordLayouts));
    tcAndLen.xy = (tcAndLen.xy + (positiveAndVCross[0] ? coordLayout.xy : coordLayout.zw)) * layout.yz;
    return tcAndLen.xy;
}
#endif

float3 RotateAroundYInDegrees (float3 vertex, float degrees)
{
    float alpha = degrees * PI / 180.0;
    float sina, cosa;
    sincos(alpha, sina, cosa);
    float2x2 m = float2x2(cosa, -sina, sina, cosa);
    return float3(mul(vertex.xz, m), vertex.y).xzy;
}

struct appdata_t {
    float4 vertex : POSITION;
};

struct v2f {
    float4 vertex : SV_POSITION;
    float3 texcoord : TEXCOORD0;
#ifdef _MAPPING_6_FRAMES_LAYOUT
    float3 layout : TEXCOORD1;
    float4 edgeSize : TEXCOORD2;
    float4 faceXCoordLayouts : TEXCOORD3;
    float4 faceYCoordLayouts : TEXCOORD4;
    float4 faceZCoordLayouts : TEXCOORD5;
#else
    float2 image180ScaleAndCutoff : TEXCOORD1;
    float4 layout3DScaleAndOffset : TEXCOORD2;
#endif
};

// Tranforms position from object to homogenous space
inline float4 UnityObjectToClipPos(in float3 pos)
{
    // More efficient than computing M*VP matrix product
    return mul(mul(float4(pos, 1.0), _world), _camera._viewProj);
}

inline float4 UnityObjectToClipPos(float4 pos) // overload for float4; avoids "implicit truncation" warning for existing shaders
{
    return UnityObjectToClipPos(pos.xyz);
}

v2f vert (VSInput v)
{
    v2f o;
    
    float3 rotated = RotateAroundYInDegrees(v.posL.xyz, _Rotation);
    o.vertex = mul(float4(v.posL, 1.0f), _camera._worldViewProj);
    o.texcoord = v.posL.xyz;

#ifdef _MAPPING_6_FRAMES_LAYOUT
    // layout and edgeSize are solely based on texture dimensions and can thus be precalculated in the vertex shader.
    float sourceAspect = float(_MainTex_TexelSize.x) / float(_MainTex_TexelSize.y);
    // Use the halfway point between the 1:6 and 3:4 aspect ratios of the strip and cross layouts to
    // guess at the correct format.
    bool3 aspectTest =
        sourceAspect >
        float3(1.0, 1.0f / 6.0f + (3.0f / 4.0f - 1.0f / 6.0f) / 2.0f, 6.0f / 1.0f + (4.0f / 3.0f - 6.0f / 1.0f) / 2.0f);
    // For a given face layout, the coordinates of the 6 cube faces are fixed: build a compact representation of the
    // coordinates of the center of each face where the first float4 represents the coordinates of the X axis faces,
    // the second the Y, and the third the Z. The first two float componenents (xy) of each float4 represent the face
    // coordinates on the positive axis side of the cube, and the second (zw) the negative.
    // layout.x is a boolean flagging the vertical cross layout (for special handling of flip-flops later)
    // layout.yz contains the inverse of the layout dimensions (ie. the scale factor required to convert from
    // normalized face coords to full texture coordinates)
    if (aspectTest[0]) // horizontal
    {
        if (aspectTest[2])
        { // horizontal strip
            o.faceXCoordLayouts = float4(0.5,0.5,1.5,0.5);
            o.faceYCoordLayouts = float4(2.5,0.5,3.5,0.5);
            o.faceZCoordLayouts = float4(4.5,0.5,5.5,0.5);
            o.layout = float3(-1,1.0/6.0,1.0/1.0);
        }
        else
        { // horizontal cross
            o.faceXCoordLayouts = float4(2.5,1.5,0.5,1.5);
            o.faceYCoordLayouts = float4(1.5,2.5,1.5,0.5);
            o.faceZCoordLayouts = float4(1.5,1.5,3.5,1.5);
            o.layout = float3(-1,1.0/4.0,1.0/3.0);
        }
    }
    else
    {
        if (aspectTest[1])
        { // vertical cross
            o.faceXCoordLayouts = float4(2.5,2.5,0.5,2.5);
            o.faceYCoordLayouts = float4(1.5,3.5,1.5,1.5);
            o.faceZCoordLayouts = float4(1.5,2.5,1.5,0.5);
            o.layout = float3(1,1.0/3.0,1.0/4.0);
        }
        else
        { // vertical strip
            o.faceXCoordLayouts = float4(0.5,5.5,0.5,4.5);
            o.faceYCoordLayouts = float4(0.5,3.5,0.5,2.5);
            o.faceZCoordLayouts = float4(0.5,1.5,0.5,0.5);
            o.layout = float3(-1,1.0/1.0,1.0/6.0);
        }
    }
    // edgeSize specifies the minimum (xy) and maximum (zw) normalized face texture coordinates that will be used for
    // sampling in the texture. Setting these to the effective size of a half pixel horizontally and vertically
    // effectively enforces clamp mode texture wrapping for each individual face.
    o.edgeSize.xy = _MainTex_TexelSize.xy * 0.5 / o.layout.yz - 0.5;
    o.edgeSize.zw = -o.edgeSize.xy;
#else // !_MAPPING_6_FRAMES_LAYOUT
    // Calculate constant horizontal scale and cutoff for 180 (vs 360) image type
    if (_ImageType == 0)  // 360 degree
        o.image180ScaleAndCutoff = float2(1.0, 1.0);
    else  // 180 degree
        o.image180ScaleAndCutoff = float2(2.0, _MirrorOnBack ? 1.0 : 0.5);

    o.layout3DScaleAndOffset = float4(0,0,1,1);
#endif
    return o;
}

float4 frag (v2f i) : SV_Target
{
// #ifdef _MAPPING_6_FRAMES_LAYOUT
//     float2 tc = ToCubeCoords(i.texcoord, i.layout, i.edgeSize, i.faceXCoordLayouts, i.faceYCoordLayouts, i.faceZCoordLayouts);
// #else
//     float2 tc = ToRadialCoords(i.texcoord);
//     if (tc.x > i.image180ScaleAndCutoff[1])
//         return float4(0,0,0,1);
//     tc.x = fmod(tc.x*i.image180ScaleAndCutoff[0], 1);
//     tc = (tc + i.layout3DScaleAndOffset.xy) * i.layout3DScaleAndOffset.zw;
// #endif

    float2 tc = ToRadialCoords(i.texcoord);
    
    tc.y = 1 - tc.y;

    if (tc.x > i.image180ScaleAndCutoff[1])
        return float4(0,0,0,1);
    tc.x = fmod(tc.x*i.image180ScaleAndCutoff[0], 1);
    tc = (tc + i.layout3DScaleAndOffset.xy) * i.layout3DScaleAndOffset.zw;

    float4 tex = gTexture.Sample(gSamWrapLinearMipPoint, tc);
    //float3 c = DecodeHDR (tex, _MainTex_HDR);
    float3 c = DecodeRGBE (tex, _MainTex_HDR.x, _MainTex_HDR.y);
    
    c = tex.xyz * _Tint.rgb * colorSpaceDouble.rgb;
    c *= _Exposure;
    
    return float4(c, 1.0f);
}