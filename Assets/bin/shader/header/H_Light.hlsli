#ifndef H_LIGHT
#define H_LIGHT

#include "header\H_PBR.hlsli"
#include "header\H_Gbuffer.hlsli"

static const uint Directional = 1u;
static const uint Point = 2u;
static const uint Spot = 3u;
static const uint AreaRect = 4u;

#define MAX_LIGHT_CNT 30

struct Light // 1 + 1 + 1 + 1 + 1 + 1 + 4 * 6 = 30 ... 30 * 30 = 900
{
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
    int _pad;

    float2 _uv0[6];
    float2 _uv1[6];

    matrix _shadowMatrix[6]; 
};

struct CascadeShadow //18
{
    matrix _shadowTransform[4]; // 4 * 4 
    float4 _cascadeOffset; 
    float4 _cascadeScale;
};

float4 ComputePBRDirectionalLight
	(in Light DL
    , in StandardData data
    , in float3 specularColor
	, in float3 albedoColor
	//, in float3 normal
    , in float3 eyeVec
    //, in float roughness
    //, in float metallic
    )
{
    float3 light = normalize(-DL._direction);
    float NdotL = saturate(dot(data._normalWorld, light));

    float3 halfVec = normalize(eyeVec + light);
    
    float NdotH = saturate(dot(data._normalWorld, halfVec));
    float NdotV = saturate(dot(data._normalWorld, eyeVec));

    float LdotH = saturate(dot(light, halfVec));
    
    float3 retColor = Unreal_PBR(data._metallicRoughness.g, data._metallicRoughness.r, specularColor, albedoColor, NdotV, NdotL, LdotH, NdotH);
    //float3 retColor = CookTorrance_GGX(roughness, metallic, specularColor, albedoColor, NdotV, NdotL, LdotH, NdotH);
       
    float3 radiance = DL._intensity * DL._color * NdotL;
  
    float4 litColor = float4(retColor * radiance, 1.f);

    return litColor;
}


float4  ComputePBRSpotLight
	(in Light SL
    , in StandardData data
    , in float3 specularColor
	, in float3 albedoColor
	//, in float3 normal
    , in float3 eyeVec
    //, in float roughness
    //, in float metallic
    //, in float3 position
    )
{
    float4 litColor = { 0.f, 0.f, 0.f, 0.f };
    
    float3 PtoL = SL._position - data._worldPosition;
    float dist = length(PtoL);
    float3 light = normalize(PtoL);
    float3 lightDirection = normalize(SL._direction);
    
    float3 halfVec = normalize(eyeVec + light);
    float NdotL = saturate(dot(data._normalWorld, light));

    float NdotH = saturate(dot(data._normalWorld, halfVec));
    float NdotV = saturate(dot(data._normalWorld, eyeVec));
    float LdotH = saturate(dot(light, halfVec));

    float3 retColor = Unreal_PBR(data._metallicRoughness.g, data._metallicRoughness.r, specularColor, albedoColor, NdotV, NdotL, LdotH, NdotH);
       
    float3 radiance = CalcIluminance(SL._intensity);

    //distance Attenuation
    float sqrDist = max(pow(dist, 2), EPSILON);
    float sqrRange = max(pow(SL._attenuationRadius, 2), EPSILON);

    //unity URP?
    float distanceAttenuation = 1.0f / sqrDist;
    float rangeAttenuation =  pow(saturate(1.0f - pow(sqrDist / sqrRange, 2)), 2.f);
    
    //Angle Att
    float innerCos = cos(SL._innerAngle * 0.5f);
    float outerCos = cos(SL._angle * 0.5f);

    float angleRangeInv = 1.f / max(innerCos - outerCos, EPSILON);
    float angleRangeInv2 = -outerCos * angleRangeInv;

    float angleAttenuation = pow(saturate(dot(-lightDirection, +light) * angleRangeInv + angleRangeInv2), 2);

    float attenuationFactor = angleAttenuation * rangeAttenuation * distanceAttenuation;
    
    litColor = float4(retColor, 1.f);
    litColor.xyz *= SL._color * NdotL * radiance * attenuationFactor;
    
    return litColor;
}

float4 ComputePBRPointLight
	(in Light PL
    , in StandardData data
    , in float3 specularColor
	, in float3 albedoColor
	//, in float3 normal
    , in float3 eyeVec
    //, in float roughness
    //, in float metallic
    //, in float3 position
    )
{
    float3 lightVector = PL._position - data._worldPosition;
    
    float dist = length(lightVector);
    float3 light = normalize(lightVector);
    float3 halfVec = normalize(eyeVec + light);

    float NdotL = saturate(dot(data._normalWorld, light));
    float NdotH = saturate(dot(data._normalWorld, halfVec));
    float NdotV = saturate(dot(data._normalWorld, eyeVec));
    float LdotH = saturate(dot(light, halfVec));

    float3 retColor = Unreal_PBR(data._metallicRoughness.g, data._metallicRoughness.r, specularColor, albedoColor, NdotV, NdotL, LdotH, NdotH);
    
    // //why divide 4 : light radiance iluminance divide PI
    float3 radiance = (CalcIluminance(PL._intensity) / 4) ;

    //distance Attenuation
    float sqrDist = max(pow(dist, 2), EPSILON);
    float sqrRange = max(pow(PL._attenuationRadius, 2), EPSILON);

    //unity URP?
    float distanceAttenuation = 1.0f / sqrDist;
    float rangeAttenuation =  pow(saturate(1.0f - pow(sqrDist / sqrRange, 2)), 2.f);
    
    float attenuationFactor = rangeAttenuation * distanceAttenuation;

    //Unreal att
    //float attenuationFactor =  1.0f / (1.0f + sqrDist / sqrRange);

    float4 litColor = float4(retColor, 1.f);
    litColor.xyz *= PL._color * NdotL  * attenuationFactor *  radiance;
    //litColor.xyz *=  attenuationFactor * radiance;
    
    return litColor;
}

 float rightPyramidSolidAngle ( float dist , float halfWidth , float half_height )
{
    float a = halfWidth ;
    float b = half_height ;
    float h = dist ;
    return 4 * asin (a * b / sqrt (( a * a + h * h) * (b * b + h * h) ));
}

// float3 IntegrateEdge(float3 v1, float3 v2, float3 N) {
//     float x = dot(v1, v2);
//     float y = abs(x);
//     float a = 0.8543985 + (0.4965155 + 0.0145206*y)*y;
//     float b = 3.4175940 + (4.1616724 + y)*y;
//     float v = a / b;
//     float thetasintheta = (x > 0.0) ? v : 0.5* inversesqrt(max(1.0 - x*x, 1e-7)) - v;
//     return dot(cross(v1, v2)*thetasintheta, N);
// }

 float rectangleSolidAngle ( float3 worldPos ,
 float3 p0 , float3 p1 ,
 float3 p2 , float3 p3 )
 {
    float3 v0 = p0 - worldPos ;
    float3 v1 = p1 - worldPos ;
    float3 v2 = p2 - worldPos ;
    float3 v3 = p3 - worldPos ;
    
    float3 n0 = normalize ( cross (v0 , v1 ));
    float3 n1 = normalize ( cross (v1 , v2 ));
    float3 n2 = normalize ( cross (v2 , v3 ));
    float3 n3 = normalize ( cross (v3 , v0 ));
    
    float g0 = acos ( clamp(dot (-n0 , n1 ), -1.f, 1.f));
    float g1 = acos ( clamp(dot  (-n1 , n2 ), -1.f, 1.f));
    float g2 = acos ( clamp(dot  (-n2 , n3 ), -1.f, 1.f));
    float g3 = acos ( clamp(dot  (-n3 , n0 ), -1.f, 1.f));
    
// float g0 = 1.f;
// float g1 = 1.f;
// float g2 = 1.f;
// float g3 = 1.f;
    
    return g0 + g1 + g2 + g3 - 2 * PI ;
 }

float3 rayPlaneIntersect (in float3 rayOrigin , in float3 rayDirection ,
in float3 planeOrigin , in float3 planeNormal )
{

    float distance = dot ( planeNormal , planeOrigin - rayOrigin ) / dot ( planeNormal , rayDirection );

    return rayOrigin + rayDirection * distance ;
}
 // Return the closest point to a rectangular shape defined by two vectors
 // left and up
 float3 closestPointRect (in float3 pos , in float3 planeOrigin , in float3 left , in float3 up , in
float halfWidth , in float half_height )
{
    float3 dir = pos - planeOrigin ;
    // - Project in 2D plane ( forward is the light direction away from
    // the plane )
    // - Clamp inside the rectangle
    // - Calculate new world position
    float2 dist2D = float2 (dot ( dir , left ) , dot ( dir , up ));
    float rectHalfSize = float2 ( halfWidth , half_height ) ;
    dist2D = clamp ( dist2D , - rectHalfSize , rectHalfSize );
    return planeOrigin + dist2D .x * left + dist2D .y * up ;
}


float GetRectAttenuationFactor(float lightWidth, float light_height, float3 up, float3 dir, float3 lightPos, float3 worldPos, float3 worldNormal, float angle)
{
    float halfWidth = lightWidth * 0.5;
    float half_height = light_height * 0.5;

    float3 lightRight = normalize(cross(up, dir));

    float clampCosAngle = 0.001 + saturate ( dot ( worldNormal , dir ));
    // clamp d0 to the positive hemisphere of surface normal
    float3 d0 = normalize ( -dir + worldNormal * clampCosAngle );
    // clamp d1 to the negative hemisphere of light plane normal
    float3 d1 = normalize ( worldNormal - dir * clampCosAngle );
    float3 dh = normalize ( d0 + d1 );
    
    float ph = rayPlaneIntersect ( worldPos , dh , lightPos , dir ) ;
    ph = closestPointRect (ph ,lightPos ,lightRight ,up ,halfWidth ,half_height );

    float3 p0 = lightPos + lightRight * +halfWidth + up * +half_height ;
    float3 p1 = lightPos + lightRight * +halfWidth + up * -half_height ;
    float3 p2 = lightPos + lightRight * -halfWidth + up * -half_height ;
    float3 p3 = lightPos + lightRight * -halfWidth + up * +half_height ;


    float solidAngle = rectangleSolidAngle ( worldPos , p0 , p1 , p2 , p3 );

    float illuminance =  solidAngle * 0.2 * (
     saturate ( dot( normalize ( p0 - worldPos ) , worldNormal ) )+
     saturate ( dot( normalize ( p1 - worldPos ) , worldNormal ) )+
     saturate ( dot( normalize ( p2 - worldPos ) , worldNormal ) )+
     saturate ( dot( normalize ( p3 - worldPos ) , worldNormal ) )+
     saturate ( dot( normalize ( lightPos - worldPos ) , worldNormal )));

    // float illuminance = 0;
    // float3 unormLightVector = ph - worldPos ;
    // float sqrDist = dot ( unormLightVector , unormLightVector ) ;
    // float L = normalize ( unormLightVector );
    // illuminance = solidAngle * saturate(dot(worldNormal, L));

    return illuminance;
}

float4 ComputePBRAreaRectLight
	(in Light ARL
    , in StandardData data
    , in float3 specularColor
	, in float3 albedoColor
	//, in float3 normal
    , in float3 eyeVec
    /*, in float roughness
    , in float metallic
    , in float3 position*/
    )
{
    float width = ARL._width;
    float _height = ARL._height;

    float4 litColor = { 0.f, 0.f, 0.f, 0.f };

    if(dot(normalize(data._worldPosition - ARL._position), ARL._direction) <= 0.001f)
        return litColor;

    float3 PtoL = ARL._position - data._worldPosition;
    float dist = length(PtoL);
    float3 light = normalize(PtoL);
    float3 lightDirection = normalize(ARL._direction);
    
    float3 halfVec = normalize(eyeVec + light);
    float NdotL = saturate(dot(data._normalWorld, light));

    float NdotH = saturate(dot(data._normalWorld, halfVec));
    float NdotV = saturate(dot(data._normalWorld, eyeVec));
    float LdotH = saturate(dot(light, halfVec));

    float3 retColor = Unreal_PBR(data._metallicRoughness.g, data._metallicRoughness.r, specularColor, albedoColor, NdotV, NdotL, LdotH, NdotH);
       
    float3 radiance = CalcIluminance(ARL._intensity);

    //distance Attenuation
    float sqrDist = max(pow(dist, 2), EPSILON);
    float sqrRange = max(pow(ARL._attenuationRadius, 2), EPSILON);

    //unity URP?
    float distanceAttenuation = 1.0f / sqrDist;
    float rangeAttenuation =  pow(saturate(1.0f - pow(sqrDist / sqrRange, 2)), 2.f);
    
    //Angle Att
    float angleAttenuation = GetRectAttenuationFactor(width, _height, ARL._up, ARL._direction, ARL._position, data._worldPosition, data._normalWorld, ARL._angle);

    float attenuationFactor = angleAttenuation* rangeAttenuation * distanceAttenuation;
    
    litColor = float4(retColor, 1.f);
    litColor.xyz *= ARL._color * NdotL * radiance * attenuationFactor;
    
    return litColor;
}

float2 ViewPosToUV(float2 posXY)
{
    float2 uv = posXY + 1.f;

    uv /= 2.0f;
    uv.y = 1.f - uv.y;

    return uv;
}

#endif