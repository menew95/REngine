#include "header\H_Input.hlsli"

cbuffer PerMaterial : register(b3)
{
    float _exposure;     // HDR exposure
    float4 _groundColor;
    float _sunSize;
    float _sunSizeConvergence;
    float4 _skyColor;
    float _atmosphereThickness;
}

// RGB wavelengths
// .35 (.62=158), .43 (.68=174), .525 (.75=190)
static const float3 kDefaultScatteringWavelength = float3(.65, .57, .475);
static const float3 kVariableRangeForScatteringWavelength = float3(.15, .15, .15);
#define OUTER_RADIUS 1.025
static const float kOuterRadius = OUTER_RADIUS;
static const float kOuterRadius2 = OUTER_RADIUS*OUTER_RADIUS;
static const float kInnerRadius = 1.0;
static const float kInnerRadius2 = 1.0;
static const float kCameraHeight = 0.0001;
#define kRAYLEIGH (lerp(0.0, 0.0025, pow(_AtmosphereThickness,2.5)))      // Rayleigh constant
#define kMIE 0.0010             // Mie constant
#define kSUN_BRIGHTNESS 20.0    // Sun brightness
#define kMAX_SCATTER 50.0 // Maximum scattering value, to prevent math overflows on Adrenos
static const half kHDSundiskIntensityFactor = 15.0;
static const half kSimpleSundiskIntensityFactor = 27.0;
static const half kSunScale = 400.0 * kSUN_BRIGHTNESS;
static const float kKmESun = kMIE * kSUN_BRIGHTNESS;
static const float kKm4PI = kMIE * 4.0 * 3.14159265;
static const float kScale = 1.0 / (OUTER_RADIUS - 1.0);
static const float kScaleDepth = 0.25;
static const float kScaleOverScaleDepth = (1.0 / (OUTER_RADIUS - 1.0)) / 0.25;
static const float kSamples = 2.0; // THIS IS UNROLLED MANUALLY, DON'T TOUCH
#define MIE_G (-0.990)
#define MIE_G2 0.9801
#define SKY_GROUND_THRESHOLD 0.02

// Calculates the Rayleigh phase function
half getRayleighPhase(half eyeCos2)
{
    return 0.75 + 0.75*eyeCos2;
}

half getRayleighPhase(half3 light, half3 ray)
{
    half eyeCos = dot(light, ray);
    return getRayleighPhase(eyeCos * eyeCos);
}