#include "header\H_Const.hlsli"
#include "header\H_Input.hlsli"
#include "header\H_GBuffer.hlsli"
#include "header\H_Light.hlsli"

Texture2D gCameraGBufferTexture0    : register(t0); // albedo + metallic
Texture2D gCameraGBufferTexture1    : register(t1);	// normal + roughness
Texture2D gCameraGBufferTexture2    : register(t2);	// world + v depth
Texture2D gCameraGBufferTexture3    : register(t3); // emission + ao
Texture2D gCameraGBufferTexture4	: register(t4); // flag
//Texture2D gCameraGBufferTexture5	: register(t5); // depth

Texture2D gSSAO : register(t5);
TextureCube gPreFilteredMap : register(t6);
TextureCube gIrradianceMap : register(t7);
Texture2D gIntegrateBRDFMap : register(t8);
Texture2DArray gCascadedShadowMap : register(t9);
Texture2DArray gSpotShadowMap : register(t10);
TextureCubeArray gPointShadowMap : register(t11);

//Texture2D gReflect : register(t11);

SamplerState gSamWrapLinear	: register(s0);
SamplerComparisonState gSamCompShadow : register(s1);

// float3 fresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
// {
//     return F0 + (max(float3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
// } 

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

	/*
	//Diffuse IBL
    _color.xyz += c_diff * diffuseIrradiance * 0.2f;

    //Specular IBL
    const float MAX_REFLECTION_LOD = 4.0; // 0~3, 4개의 밉맵 존재합니다.
    float mipLevel = roughness * MAX_REFLECTION_LOD;
    float3 R = reflect(-eyeVec, normal.xyz);

    float3 prefilteredColor = g_PreFilteredMap.SampleLevel(g_samTriLinear, normalize(R).xyz, mipLevel).rgb;
    float NdotV = dot(normal.xyz, eyeVec);
    float2 BRDF = g_BRDFLookUpTable.Sample(g_samLinearClamp, float2(max(NdotV, 0.f), roughness)).rg;

    float3 _specularIBL = prefilteredColor * (c_spec * BRDF.x + BRDF.y);

	*/
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

	float4 color = float4(0.f, 0.f, 0.f, 0.f);

	[unroll]
    for (uint lightIdx = 0; lightIdx < _lightCnt; lightIdx++)
    {
            if (_light[lightIdx].Type == Directional)
        {
            //float shadows = 1.0f;

            //float clipSpaceDepth = posW.w;
            //int cascadeSlice = 0;
            // shadow
            // 2048 -> hard code shader map size
            //shadows *= CalcShadowFactor(g_samShadow, g_ShadowMap, posW.xyz, 2048.f, cascadeSlice);
            // shadows = 1.0f;
            //Output += CalcShadowColor(ComputePBRDirectionalLight(g_Light[lightIdx], c_spec, c_diff, normal.xyz, eyeVec, roughness, metallic) * shadows, shadows);

			color += ComputePBRDirectionalLight(_light[lightIdx], data, c_spec, c_diff, eyeVec);
        }
        else if (_light[lightIdx].Type == Point)
        {
            //float shadows = 1.0f;

            //int idx = asint(lightIdx);

            //shadows *= CalcShadowFactorFromPointShadowMap(g_samShadow, g_PointShadowMap, posW.xyz, 1024.f, idx);

            //Output +=  CalcShadowColor(ComputePBRPointLight(g_Light[lightIdx], c_spec, c_diff, normal.xyz, eyeVec, roughness, metallic, posW.xyz) * shadows, shadows);
            // shadows = 1.0f;
			
			color += ComputePBRPointLight(_light[lightIdx], data, c_spec, c_diff, eyeVec);
        }
        else if (_light[lightIdx].Type  == Spot)
        {
            //float shadows = 1.0f;
            //int idx = asint(lightIdx);
            // shadow
            // 2048 -> hard code shader map size
            //shadows *= CalcShadowFactorFromSpotShadowMap(g_samShadow, g_SpotShadowMap, posW.xyz, 1024.f, idx);
            //Output +=  CalcShadowColor(ComputePBRSpotLight(g_Light[lightIdx], c_spec, c_diff, normal.xyz, eyeVec, roughness, metallic, posW.xyz) * shadows, shadows) ;
			
			color += ComputePBRSpotLight(_light[lightIdx], data, c_spec, c_diff, eyeVec);
        }
        else if (_light[lightIdx].Type == AreaRect)
        {
            //float shadows = 1.0f;

            //shadows *= CalcShadowFactorFromAreaShadowMap(g_samShadow, g_SpotShadowMap, posW.xyz, 1024.f, lightIdx);

            //Output +=  CalcShadowColor(ComputePBRAreaRectLight(g_Light[lightIdx], c_spec, c_diff, normal.xyz, eyeVec, roughness, metallic, posW.xyz) * shadows, shadows);
			
			color += ComputePBRAreaRectLight(_light[lightIdx], data, c_spec, c_diff, eyeVec);
        }
    }

	float4 _color = float4(0.f, 0.f, 0.f, 0.f);

	return _color;
}