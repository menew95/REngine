/**

    @file      PerFrame.h
    @brief     
    @details   ~
    @author    Cool Guy
    @date      19.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <common\math.h>

#include <graphics_core\resource\CameraBuffer.h>
#include <graphics_core\LightManager.h>

namespace graphics
{
	struct alignas(16) PostProcess
	{
		// sslr
		Vector2 _depthBufferSize; // dimensions of the z-buffer
		float _zThickness = 0.2f; // thickness to ascribe to each pixel in the depth buffer
		float _nearPlaneZ; // the camera's near z plane

		float _stride = 1.0f; // Step in horizontal or vertical pixels between samples. This is a float
		// because integer math is slow on GPUs, but should be set to an integer >= 1.
		float _maxSteps = 1000.f; // Maximum number of iterations. Higher gives better images but may be slow.
		float _maxDistance = 300.f; // Maximum camera-space distance to trace before returning a miss.
		float _strideZCutoff; // More distant pixels are smaller in screen space. This value tells at what point to
		// start relaxing the stride to give higher quality reflections for objects far from
		// the camera.

		float _numMips = 7.f; // the number of mip levels in the convolved color buffer
		float _fadeStart = 0.09f; // determines where to start screen edge fading of effect
		float _fadeEnd = 1.f; // determines where to end screen edge fading of effect

		// tone map
		float g_fHardExposure = 1.0f;

		Vector4 _pad;  // 64bit
	};

	struct alignas(16) PerFrame
	{
		CameraInfo _camera;
		//PostProcess _postProcess;

		uint _lightCnt = 0;
		Vector3 _pad;

		Vector4 _LightPos;
		Vector4 _LightColor;

		CascadedInfo _shadow;
	};

	struct alignas(16) PerCamera
	{
		CameraInfo _camera;

		CascadedInfo _shadow;
	};
}