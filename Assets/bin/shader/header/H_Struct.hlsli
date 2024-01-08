#if !defined(H_STRUCT)
#define H_STRUCT

struct Camera
{
    float3 _world           : packoffset(c0.xyz);

    matrix _view            : packoffset(c1);
    matrix _proj            : packoffset(c5);
    matrix _projInv         : packoffset(c9);
    matrix _viewToTexSapce  : packoffset(c13);
    matrix _worldViewProj   : packoffset(c17);

    float _near             : packoffset(c18.x);
    float _far              : packoffset(c18.y);
};

struct Material
{
	float4 _albedo;
	float4 _emissive;
	float _roughness;
	float _metallic;
	float2 _tiling;
};

struct Light
{
	uint Type           : packoffset(c0.x);

	float Range         : packoffset(c0.y);		// Point, Spot
	float FallOff       : packoffset(c0.z);		// Point, Spot

	float3 Position     : packoffset(c1.xyz);	// Point, Spot
	float SpotAngle     : packoffset(c1.w);	    // Spot

	float3 Direction    : packoffset(c2.xyz);	// Directional, Spot
	float FallOffAngle  : packoffset(c2.w);	    // Spot

	float3 Color        : packoffset(c3.xyz);
	float Power         : packoffset(c3.w);
	matrix ShadowTransform;
};

struct CascadedLight
{
	// float3 _direction;
	// float _pad1;

	// float3 _color;
	// float _power;

	float _cascadeEndClipSpace[4] : packoffset(c0.xyzw);
	matrix _lightTransform[4];
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