#if !defined(H_STRUCT)
#define H_STRUCT

struct Camera
{
    float3 _world;

    matrix _view;
    matrix _proj;
    matrix _projInv;
    matrix _viewToTexSapce;
    matrix _worldViewProj;

	matrix _viewProj;
    matrix _viewProjInv;
    matrix _viewProjInvTranspose;

    float _near;
    float _far;
};

struct PostProcess
{
	// sslr
	float2 _depthBufferSize; // dimensions of the z-buffer
	float _zThickness; // thickness to ascribe to each pixel in the depth buffer
	float _nearPlaneZ; // the camera's near z plane

	float _stride; // Step in horizontal or vertical pixels between samples. This is a float
	// because integer math is slow on GPUs, but should be set to an integer >= 1.
	float _maxSteps; // Maximum number of iterations. Higher gives better images but may be slow.
	float _maxDistance; // Maximum camera-space distance to trace before returning a miss.
	float _strideZCutoff; // More distant pixels are smaller in screen space. This value tells at what point to
	// start relaxing the stride to give higher quality reflections for objects far from
	// the camera.

	float _numMips; // the number of mip levels in the convolved color buffer
	float _fadeStart; // determines where to start screen edge fading of effect
	float _fadeEnd; // determines where to end screen edge fading of effect

	// tone map
	float g_fHardExposure;

	float4 _pad;  // 64bit
};

// struct ScreenSpaceReflection
// {
// 	float _stride;
// 	// Step in horizontal or vertical pixels between samples. This is a float
// 	// because integer math is slow on GPUs, but should be set to an integer >= 1.
// 	float _maxSteps;    // Maximum number of iterations. Higher gives better images but may be slow.
// 	float _maxDistance; // Maximum camera-space distance to trace before returning a miss.
// 	float _strideZCutoff;
// 	// More distant pixels are smaller in screen space. This value tells at what point to
// 	// start relaxing the stride to give higher quality reflections for objects far from
// 	// the camera.

// 	float _numMips;       // the number of mip levelses in the convolved color buffer
// 	float _fadeStart;     // determines where to start screen edge fading of effect
// 	float _fadeEnd;       // determines where to end screen edge fading of effect
// 	float _sslr_padding0; // padding for alignment

// 	float _zThickness; // thickness to ascribe to each pixel in the depth buffer
// 	float4 _sslr_padding1;
// };

// struct ExponentialHeightFog
// {
// 	float _fogDensity;		  //밀도
// 	float _fogMaxOpacity;     //최대 안개 수치(0~1사이의 값)
// 	float _fogStartDistance;  //카메라로부터의 거리
// 	float _fogCutOffDistance; //카메라로부터 안개가 끝나는 지점의 거리

// 	float _fogHeightFalloff;  //높이 exponential의 수치.
// 	float3 _fogHeight;

// 	float3 _fogScatteringColor;
// 	float _lightScatterFlag; //directional light가 있을 넣어준다. 아니면 -1
// };

// struct RimLight
// {
//     float _clipPoint;
//     float _rimDecrease;
//     float2 _rim_padding0;
// };

// struct Bloom
// {
//     float _intensity;
//     float _threshhold;
//     float2 _bloom_padding0;
// };

// const int SSAO_MAX_KERNEL_COUNT = 64;
// const int SSAO_ROTATION_NOISE = 16;

// struct SSAO
// {
//     float4 _ssaoSampleKernel[SSAO_MAX_KERNEL_COUNT];
//     float4 _ssaoRotationNoise[SSAO_ROTATION_NOISE];
// };

// struct ToneMapping
// {
//     float _exposure;
//     float4 _tone_padding0;
// };
	

// struct PostProcess
// {
// 	ScreenSpaceReflection _screenSpaceReflectionSetting;

// 	ExponentialHeightFog _exponentialHeightFogSettig;

// 	RimLight _rimLightSetting;

// 	Bloom _bloomSetting;

// 	SSAO _ssaoSetting;

// 	ToneMapping _toneMappingSetting;
// };


#endif