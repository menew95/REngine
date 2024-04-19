// #include "header\H_Input.hlsli"

// cbuffer PerMaterial : register(b2)
// {
//     float _exposure;     // HDR exposure
//     float4 _groundColor;
//     float _sunSize;
//     float _sunSizeConvergence;
//     float4 _skyColor;
//     float _atmosphereThickness;
// }

// // RGB wavelengths
// // .35 (.62=158), .43 (.68=174), .525 (.75=190)
// static const float3 kDefaultScatteringWavelength = float3(.65, .57, .475);
// static const float3 kVariableRangeForScatteringWavelength = float3(.15, .15, .15);
// #define OUTER_RADIUS 1.025
// static const float kOuterRadius = OUTER_RADIUS;
// static const float kOuterRadius2 = OUTER_RADIUS*OUTER_RADIUS;
// static const float kInnerRadius = 1.0;
// static const float kInnerRadius2 = 1.0;
// static const float kCameraHeight = 0.0001;
// #define kRAYLEIGH (lerp(0.0, 0.0025, pow(_AtmosphereThickness,2.5)))      // Rayleigh constant
// #define kMIE 0.0010             // Mie constant
// #define kSUN_BRIGHTNESS 20.0    // Sun brightness
// #define kMAX_SCATTER 50.0 // Maximum scattering value, to prevent math overflows on Adrenos
// static const half kHDSundiskIntensityFactor = 15.0;
// static const half kSimpleSundiskIntensityFactor = 27.0;
// static const half kSunScale = 400.0 * kSUN_BRIGHTNESS;
// static const float kKmESun = kMIE * kSUN_BRIGHTNESS;
// static const float kKm4PI = kMIE * 4.0 * 3.14159265;
// static const float kScale = 1.0 / (OUTER_RADIUS - 1.0);
// static const float kScaleDepth = 0.25;
// static const float kScaleOverScaleDepth = (1.0 / (OUTER_RADIUS - 1.0)) / 0.25;
// static const float kSamples = 2.0; // THIS IS UNROLLED MANUALLY, DON'T TOUCH
// #define MIE_G (-0.990)
// #define MIE_G2 0.9801
// #define SKY_GROUND_THRESHOLD 0.02

// // Calculates the Mie phase function
// half getMiePhase(half eyeCos, half eyeCos2)
// {
//     half temp = 1.0 + MIE_G2 - 2.0 * MIE_G * eyeCos;
//     temp = pow(temp, pow(_sunSize,0.65) * 10);
//     temp = max(temp,1.0e-4); // prevent division by zero, esp. in half precision
//     temp = 1.5 * ((1.0 - MIE_G2) / (2.0 + MIE_G2)) * (1.0 + eyeCos2) / temp;

//     #if defined(COLORSPACE_GAMMA) && SKYBOX_COLOR_IN_TARGET_COLOR_SPACE
//         temp = pow(temp, .454545);
//     #endif

//     return temp;
// }
// // Calculates the sun shape
// half calcSunAttenuation(half3 lightPos, half3 ray)
// {
// #if SKYBOX_SUNDISK == SKYBOX_SUNDISK_SIMPLE
//     half3 delta = lightPos - ray;
//     half dist = length(delta);
//     half spot = 1.0 - smoothstep(0.0, _SunSize, dist);
//     return spot * spot;
// #else // SKYBOX_SUNDISK_HQ
//     half focusedEyeCos = pow(saturate(dot(lightPos, ray)), _SunSizeConvergence);
//     return getMiePhase(-focusedEyeCos, focusedEyeCos * focusedEyeCos);
// #endif
// }
// half4 frag (v2f IN) : SV_Target
// {
//     half3 col = half3(0.0, 0.0, 0.0);
// // if y > 1 [eyeRay.y < -SKY_GROUND_THRESHOLD] - ground
// // if y >= 0 and < 1 [eyeRay.y <= 0 and > -SKY_GROUND_THRESHOLD] - horizon
// // if y < 0 [eyeRay.y > 0] - sky
// #if SKYBOX_SUNDISK == SKYBOX_SUNDISK_HQ
//     half3 ray = normalize(IN.vertex.xyz);
//     half y = ray.y / SKY_GROUND_THRESHOLD;
// #elif SKYBOX_SUNDISK == SKYBOX_SUNDISK_SIMPLE
//     half3 ray = IN.rayDir.xyz;
//     half y = ray.y / SKY_GROUND_THRESHOLD;
// #else
//     half y = IN.skyGroundFactor;
// #endif
//     // if we did precalculate color in vprog: just do lerp between them
//     col = lerp(IN.skyColor, IN.groundColor, saturate(y));
// #if SKYBOX_SUNDISK != SKYBOX_SUNDISK_NONE
//     if(y < 0.0)
//     {
//         col += IN.sunColor * calcSunAttenuation(_WorldSpaceLightPos0.xyz, -ray);
//     }
// #endif
// #if defined(UNITY_COLORSPACE_GAMMA) && !SKYBOX_COLOR_IN_TARGET_COLOR_SPACE
//     col = LINEAR_2_OUTPUT(col);
// #endif
//     return half4(col,1.0);
// }