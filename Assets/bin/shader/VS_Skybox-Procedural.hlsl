

struct VSInput
{
	float3	posL			: POSITION;
	float4	color			: COLOR;
	float2	uv				: TEXCOORD;
	float3	normal			: NORMAL;
	float3	tangent			: TANGENT;
    uint	bone[BONECNT]   : BONE;
    float	weight[BONECNT] : WEIGHT;
};

struct VSOutput
{
    float4  pos             : SV_POSITION;
#if SKYBOX_SUNDISK == SKYBOX_SUNDISK_HQ
    // for HQ sun disk, we need vertex itself to calculate ray-dir per-pixel
    float3  vertex          : TEXCOORD0;
#elif SKYBOX_SUNDISK == SKYBOX_SUNDISK_SIMPLE
    half3   rayDir          : TEXCOORD0;
#else
    // as we dont need sun disk we need just rayDir.y (sky/ground threshold)
    half    skyGroundFactor : TEXCOORD0;
#endif
    // calculate sky colors in vprog
    half3   groundColor     : TEXCOORD1;
    half3   skyColor        : TEXCOORD2;
#if SKYBOX_SUNDISK != SKYBOX_SUNDISK_NONE
    half3   sunColor        : TEXCOORD3;
#endif
    UNITY_VERTEX_OUTPUT_STEREO
};

VSOutput main(VSInput input)
{
    VSOutput OUT;
        UNITY_SETUP_INSTANCE_ID(v);
        UNITY_INITIALIZE_VERTEX_OUTPUT_STEREO(OUT);
        OUT.pos = UnityObjectToClipPos(v.vertex);

        float3 kSkyTintInGammaSpace = COLOR_2_GAMMA(_SkyTint); // convert tint from Linear back to Gamma
        float3 kScatteringWavelength = lerp (
            kDefaultScatteringWavelength-kVariableRangeForScatteringWavelength,
            kDefaultScatteringWavelength+kVariableRangeForScatteringWavelength,
            half3(1,1,1) - kSkyTintInGammaSpace); // using Tint in sRGB gamma allows for more visually linear interpolation and to keep (.5) at (128, gray in sRGB) point
        float3 kInvWavelength = 1.0 / pow(kScatteringWavelength, 4);

        float kKrESun = kRAYLEIGH * kSUN_BRIGHTNESS;
        float kKr4PI = kRAYLEIGH * 4.0 * 3.14159265;

        float3 cameraPos = float3(0,kInnerRadius + kCameraHeight,0);    // The camera's current position

        // Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)
        float3 eyeRay = normalize(mul((float3x3)unity_ObjectToWorld, v.vertex.xyz));

        float far = 0.0;
        half3 cIn, cOut;

        if(eyeRay.y >= 0.0)
        {
            // Sky
            // Calculate the length of the "atmosphere"
            far = sqrt(kOuterRadius2 + kInnerRadius2 * eyeRay.y * eyeRay.y - kInnerRadius2) - kInnerRadius * eyeRay.y;

            float3 pos = cameraPos + far * eyeRay;

            // Calculate the ray's starting position, then calculate its scattering offset
            float height = kInnerRadius + kCameraHeight;
            float depth = exp(kScaleOverScaleDepth * (-kCameraHeight));
            float startAngle = dot(eyeRay, cameraPos) / height;
            float startOffset = depth*scale(startAngle);


            // Initialize the scattering loop variables
            float sampleLength = far / kSamples;
            float scaledLength = sampleLength * kScale;
            float3 sampleRay = eyeRay * sampleLength;
            float3 samplePoint = cameraPos + sampleRay * 0.5;

            // Now loop through the sample rays
            float3 frontColor = float3(0.0, 0.0, 0.0);
            // Weird workaround: WP8 and desktop FL_9_3 do not like the for loop here
            // (but an almost identical loop is perfectly fine in the ground calculations below)
            // Just unrolling this manually seems to make everything fine again.
//          for(int i=0; i<int(kSamples); i++)
            {
                float height = length(samplePoint);
                float depth = exp(kScaleOverScaleDepth * (kInnerRadius - height));
                float lightAngle = dot(_WorldSpaceLightPos0.xyz, samplePoint) / height;
                float cameraAngle = dot(eyeRay, samplePoint) / height;
                float scatter = (startOffset + depth*(scale(lightAngle) - scale(cameraAngle)));
                float3 attenuate = exp(-clamp(scatter, 0.0, kMAX_SCATTER) * (kInvWavelength * kKr4PI + kKm4PI));

                frontColor += attenuate * (depth * scaledLength);
                samplePoint += sampleRay;
            }
            {
                float height = length(samplePoint);
                float depth = exp(kScaleOverScaleDepth * (kInnerRadius - height));
                float lightAngle = dot(_WorldSpaceLightPos0.xyz, samplePoint) / height;
                float cameraAngle = dot(eyeRay, samplePoint) / height;
                float scatter = (startOffset + depth*(scale(lightAngle) - scale(cameraAngle)));
                float3 attenuate = exp(-clamp(scatter, 0.0, kMAX_SCATTER) * (kInvWavelength * kKr4PI + kKm4PI));

                frontColor += attenuate * (depth * scaledLength);
                samplePoint += sampleRay;
            }



            // Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader
            cIn = frontColor * (kInvWavelength * kKrESun);
            cOut = frontColor * kKmESun;
        }
        else
        {
            // Ground
            far = (-kCameraHeight) / (min(-0.001, eyeRay.y));

            float3 pos = cameraPos + far * eyeRay;

            // Calculate the ray's starting position, then calculate its scattering offset
            float depth = exp((-kCameraHeight) * (1.0/kScaleDepth));
            float cameraAngle = dot(-eyeRay, pos);
            float lightAngle = dot(_WorldSpaceLightPos0.xyz, pos);
            float cameraScale = scale(cameraAngle);
            float lightScale = scale(lightAngle);
            float cameraOffset = depth*cameraScale;
            float temp = (lightScale + cameraScale);

            // Initialize the scattering loop variables
            float sampleLength = far / kSamples;
            float scaledLength = sampleLength * kScale;
            float3 sampleRay = eyeRay * sampleLength;
            float3 samplePoint = cameraPos + sampleRay * 0.5;

            // Now loop through the sample rays
            float3 frontColor = float3(0.0, 0.0, 0.0);
            float3 attenuate;
//          for(int i=0; i<int(kSamples); i++) // Loop removed because we kept hitting SM2.0 temp variable limits. Doesn't affect the image too much.
            {
                float height = length(samplePoint);
                float depth = exp(kScaleOverScaleDepth * (kInnerRadius - height));
                float scatter = depth*temp - cameraOffset;
                attenuate = exp(-clamp(scatter, 0.0, kMAX_SCATTER) * (kInvWavelength * kKr4PI + kKm4PI));
                frontColor += attenuate * (depth * scaledLength);
                samplePoint += sampleRay;
            }

            cIn = frontColor * (kInvWavelength * kKrESun + kKmESun);
            cOut = clamp(attenuate, 0.0, 1.0);
        }

    #if SKYBOX_SUNDISK == SKYBOX_SUNDISK_HQ
        OUT.vertex          = -eyeRay;
    #elif SKYBOX_SUNDISK == SKYBOX_SUNDISK_SIMPLE
        OUT.rayDir          = half3(-eyeRay);
    #else
        OUT.skyGroundFactor = -eyeRay.y / SKY_GROUND_THRESHOLD;
    #endif

        // if we want to calculate color in vprog:
        // 1. in case of linear: multiply by _Exposure in here (even in case of lerp it will be common multiplier, so we can skip mul in fshader)
        // 2. in case of gamma and SKYBOX_COLOR_IN_TARGET_COLOR_SPACE: do sqrt right away instead of doing that in fshader

        OUT.groundColor = _Exposure * (cIn + COLOR_2_LINEAR(_GroundColor) * cOut);
        OUT.skyColor    = _Exposure * (cIn * getRayleighPhase(_WorldSpaceLightPos0.xyz, -eyeRay));

    #if SKYBOX_SUNDISK != SKYBOX_SUNDISK_NONE
        // The sun should have a stable intensity in its course in the sky. Moreover it should match the highlight of a purely specular material.
        // This matching was done using the standard shader BRDF1 on the 5/31/2017
        // Finally we want the sun to be always bright even in LDR thus the normalization of the lightColor for low intensity.
        half lightColorIntensity = clamp(length(_LightColor0.xyz), 0.25, 1);
        #if SKYBOX_SUNDISK == SKYBOX_SUNDISK_SIMPLE
            OUT.sunColor    = kSimpleSundiskIntensityFactor * saturate(cOut * kSunScale) * _LightColor0.xyz / lightColorIntensity;
        #else // SKYBOX_SUNDISK_HQ
            OUT.sunColor    = kHDSundiskIntensityFactor * saturate(cOut) * _LightColor0.xyz / lightColorIntensity;
        #endif

    #endif

    #if defined(UNITY_COLORSPACE_GAMMA) && SKYBOX_COLOR_IN_TARGET_COLOR_SPACE
        OUT.groundColor = sqrt(OUT.groundColor);
        OUT.skyColor    = sqrt(OUT.skyColor);
        #if SKYBOX_SUNDISK != SKYBOX_SUNDISK_NONE
            OUT.sunColor= sqrt(OUT.sunColor);
        #endif
    #endif

        return OUT;
}