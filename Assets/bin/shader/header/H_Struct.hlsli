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

#endif