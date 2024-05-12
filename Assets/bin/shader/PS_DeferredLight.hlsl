#include "header\H_Const.hlsli"
#include "header\H_Input.hlsli"
#include "header\H_GBuffer.hlsli"
#include "header\H_Light.hlsli"
#include "header\H_ShadowHelper.hlsli"

Texture2D gCameraGBufferTexture0    : register(t0); // albedo + metallic
Texture2D gCameraGBufferTexture1    : register(t1);	// normal + roughness
Texture2D gCameraGBufferTexture2    : register(t2);	// world + v depth
Texture2D gCameraGBufferTexture3    : register(t3); // emission + ao
Texture2D gCameraGBufferTexture4	: register(t4); // flag

Texture2D gSSAO : register(t5);
TextureCube gPreFilteredMap : register(t6);
TextureCube gIrradianceMap : register(t7);
Texture2D gIntegrateBRDFMap : register(t8);

Texture2D gShadowAtlas : register(t9);

//Texture2DArray gCascadedShadowMap : register(t9);
//Texture2DArray gSpotShadowMap : register(t10);
//TextureCubeArray gPointShadowMap : register(t11);

StructuredBuffer<Light> gLightTexture : register(t12);

// 샘플링하는 함수 예시
Light GetLight(uint index) {
    return gLightTexture[index];
}

SamplerState gSamWrapLinear	: register(s0);
SamplerComparisonState gSamCompShadow : register(s1);

float CalcCascadedShadowFactor(
    in SamplerComparisonState samShadow
    , in Texture2D shadowMapAtlas
    , Light light
    , float3 worldPosition
    , in float4x4 view
    , in float near
    , in float far
    )
{
    const int CASCADE_MAX_COUNT = 4;

    float viewpos = mul(float4(worldPosition, 1.f), view).z;
    //viewpos /= (far - near);

    int cascadeSlice;

    for (cascadeSlice = 0; cascadeSlice < 4; cascadeSlice++)
    {
        if (viewpos < _shadow._cascadeOffset[cascadeSlice + 1].x)
        {
            break;
        }
    }

    float2 shadowResolution = light._uv1[0] - light._uv0[0];

    float shadowFactor = CalcShadowFactorFromShadowAtlas(samShadow
    , shadowMapAtlas
    , shadowResolution
    , light._uv0[cascadeSlice]
    , light._uv1[cascadeSlice]
    , _shadowAtlasResolution
    , light._shadowMatrix[cascadeSlice]
    , worldPosition
    );

    return shadowFactor;
}

float SpotShadowFactor(
    in SamplerComparisonState samShadow
    , in Texture2D shadowMapAtlas
    , Light light
    , float3 worldPosition
    )
{
    float2 shadowResolution = light._uv1[0] - light._uv0[0];

    float shadowFactor = CalcShadowFactorFromShadowAtlas(samShadow
    , shadowMapAtlas
    , shadowResolution
    , light._uv0[0]
    , light._uv1[0]
    , _shadowAtlasResolution
    , light._shadowMatrix[0]
    , worldPosition
    );

    return shadowFactor;
}

//vec3 doesn’t have to be normalized,
//Translates from world space vector to a coordinate inside our 6xsize shadow map
void GetSampleCoordinate(in float3 dir, out float2 uv, out int faceIdx)
{
    // X => right left
    if (abs(dir.x) >= abs(dir.y) && abs(dir.x) >= abs(dir.z))
    {
        if (dir.x > 0) //Positive X
        {
            faceIdx = 0;
            dir.z = -dir.z;
            uv = normalize(dir.zy);
        }
        else
        {
            faceIdx = 1; //Negative X
        }
    }
    // y => top bottom
    else if(abs(dir.y) >= abs(dir.x) && abs(dir.y) >= abs(dir.z))
    {
        if (dir.x > 0) //Positive Y
        {
            faceIdx = 2;
            dir.z = -dir.z;
            uv = normalize(dir.xz);
        }
        else
        {
            faceIdx = 3; //Negative Y
            uv = normalize(dir.xz);
        }
    }
    // z => front back
    else if(abs(dir.z) >= abs(dir.x) && abs(dir.z) >= abs(dir.y))
    {
        if (dir.x > 0) //Positive Z
        {
            faceIdx = 4;
            uv = normalize(dir.xy);
        }
        else
        {
            faceIdx = 5; //Negative Z
            dir.x = -dir.x;
            uv = normalize(dir.xy);
        }
    }
}

float PointShadowFactor(
    in SamplerComparisonState samShadow
    , in Texture2D shadowMapAtlas
    , Light light
    , float3 worldPosition
    )
{
    float2 shadowResolution = light._uv1[0] - light._uv0[0];

    float3 dir = worldPosition - light._position;
    
    int faceIdx = 0;
    float2 uv = float2(0, 0);

    GetSampleCoordinate(dir, uv, faceIdx);

    //GetSampleCoordinate(dir);

    float shadowFactor = CalcShadowFactorFromShadowAtlas(samShadow
    , shadowMapAtlas
    , shadowResolution
    , light._uv0[faceIdx]
    , light._uv1[faceIdx]
    , _shadowAtlasResolution
    , light._shadowMatrix[faceIdx]
    , worldPosition
    );

    return shadowFactor;
}

float3 FresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
	return F0 + (max((1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float3 CalcIBL(float3 V, float3 N, float3 albedo, float3 F0, float roughness, float metallic, float ao)
{
	float3 R = reflect(-V, N);

	const float MAX_REFLECTION_LOD = 5.0;
	float3 prefilteredColor = gPreFilteredMap.SampleLevel(gSamWrapLinear, R, roughness * 5).rgb;

	float3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
	float3 kS = F;
	float3 kD = 1.0 - kS;
	kD *= 1.0 - metallic;

	float3 irradiance = gIrradianceMap.Sample(gSamWrapLinear, N).rgb;
	float3 diffuse = irradiance * albedo;

	float2 envBRDF = gIntegrateBRDFMap.Sample(gSamWrapLinear, float2(max(dot(N, V), 0.0), roughness)).rg;
	float3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

	float3 ambient = (kD * diffuse + specular) * ao;

	return ambient;
}

float3 IBL(
    in StandardData data
    , in float3 specularColor
	, in float3 albedoColor
    , in float3 eyeVec
)
{
	float3 diffuseIrradiance = gIrradianceMap.Sample(gSamWrapLinear, data._normalWorld).xyz;

    //Diffuse IBL
    float3 diffuseIBL =  albedoColor * diffuseIrradiance;

    //Specular IBL
    const float MAX_REFLECTION_LOD = 4.0; // 0~3, 4개의 밉맵 존재합니다.
    float mipLevel = data._metallicRoughness.g * MAX_REFLECTION_LOD;
    float3 R = reflect(-eyeVec, data._normalWorld.xyz);

    float3 prefilteredColor = gPreFilteredMap.SampleLevel(gSamWrapLinear, normalize(R).xyz, mipLevel).rgb;
    float NdotV = dot(data._normalWorld.xyz, eyeVec);
    float2 BRDF = gIntegrateBRDFMap.Sample(gSamWrapLinear, float2(max(NdotV, 0.f), data._metallicRoughness.g)).rg;

    float3 specularIBL = prefilteredColor * (specularColor * BRDF.x + BRDF.y);

    return (diffuseIBL + specularIBL) * data._ambientOcclussion;
}

float4 main(VSOutput input) : SV_TARGET
{
	float4 gbuffer0 = gCameraGBufferTexture0.Sample(gSamWrapLinear, input.uv);
	float4 gbuffer1 = gCameraGBufferTexture1.Sample(gSamWrapLinear, input.uv);
	float4 gbuffer2 = gCameraGBufferTexture2.Sample(gSamWrapLinear, input.uv);
	float4 gbuffer3 = gCameraGBufferTexture3.Sample(gSamWrapLinear, input.uv);
	float4 gbuffer4 = gCameraGBufferTexture4.Sample(gSamWrapLinear, input.uv);

	StandardData data = StandardDataFronGBuffer(gbuffer0, gbuffer1, gbuffer2, gbuffer3, gbuffer4);

	static const float kSpecularCoefficient = 0.08;

    // blend base Color Width Metallic
    // 메탈릭 수치에 따라
    float specular = kSpecularCoefficient * data._metallicRoughness.r;

    const float3 c_diff = lerp(data._diffuseColor.xyz, float3(0, 0, 0), data._metallicRoughness.r);
    const float3 c_spec = lerp((float3)specular, data._diffuseColor.xyz, data._metallicRoughness.r);

	float3 eyeVec = normalize(_camera._world.xyz - data._worldPosition.xyz);

	float4 color = float4(0.f, 0.f, 0.f, 1.f);

	//[unroll(MAX_LIGHT_CNT)]
    for (uint lightIdx = 0; lightIdx < _lightCnt; lightIdx++)
    {
        Light light = GetLight(lightIdx);

        if (light._type == Directional)
        {
            float shadows = 1.0f;

            float clipSpaceDepth = 1.0f;

            //shadows *= CalcCascadedShadowFactor(gSamCompShadow, gShadowAtlas, light, data._worldPosition, _camera._view, _camera._near, _camera._far);

            color += shadows * ComputePBRDirectionalLight(light, data, c_spec, c_diff, eyeVec);
            // Output += CalcShadowColor(ComputePBRDirectionalLight(g_Light[lightIdx], c_spec, c_diff, normal.xyz, eyeVec, roughness, metallic) * shadows, shadows);
			//color += ComputePBRDirectionalLight(light, data, c_spec, c_diff, eyeVec);
        }
        else if (light._type == Point)
        {
            //float shadows = 1.0f;

            //int idx = asint(lightIdx);

            //shadows *= CalcShadowFactorFromPointShadowMap(g_samShadow, g_PointShadowMap, posW.xyz, 1024.f, idx);

            //Output +=  CalcShadowColor(ComputePBRPointLight(g_Light[lightIdx], c_spec, c_diff, normal.xyz, eyeVec, roughness, metallic, posW.xyz) * shadows, shadows);
            // shadows = 1.0f;
			
			color += ComputePBRPointLight(light, data, c_spec, c_diff, eyeVec);
        }
        else if (light._type  == Spot)
        {
            //float shadows = 1.0f;
            //int idx = asint(lightIdx);
            // shadow
            // 2048 -> hard code shader map size
            //shadows *= CalcShadowFactorFromSpotShadowMap(g_samShadow, g_SpotShadowMap, posW.xyz, 1024.f, idx);
            //Output +=  CalcShadowColor(ComputePBRSpotLight(g_Light[lightIdx], c_spec, c_diff, normal.xyz, eyeVec, roughness, metallic, posW.xyz) * shadows, shadows) ;
			float shadows = 1.0f;

            float clipSpaceDepth = 1.0f;

            shadows *= SpotShadowFactor(gSamCompShadow, gShadowAtlas, light, data._worldPosition);
            
			color += ComputePBRSpotLight(light, data, c_spec, c_diff, eyeVec) * shadows;
        }
        else if (light._type == AreaRect)
        {
            //float shadows = 1.0f;

            //shadows *= CalcShadowFactorFromAreaShadowMap(g_samShadow, g_SpotShadowMap, posW.xyz, 1024.f, lightIdx);

            //Output +=  CalcShadowColor(ComputePBRAreaRectLight(g_Light[lightIdx], c_spec, c_diff, normal.xyz, eyeVec, roughness, metallic, posW.xyz) * shadows, shadows);
			
			color += ComputePBRAreaRectLight(light, data, c_spec, c_diff, eyeVec);
        }
    }

    color.xyz += IBL(data, c_spec, c_diff, eyeVec);
    
    // Output.xyz += specularIBL * 0.2f;

    // // AO
    // Output.xyz *= pow(metallicRoughnessAOSpecular.z * g_SSAO.Load(int3(Input.Position.xy, 0)).r, g_AOPower);
    // //이후 Emissive
    // Output.xyz += emissiveColor.xyz * (emissiveColor.w * 255);

    // //Set Alpha
    // Output.w = albedo.w;
    
	return color;
}