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
	struct alignas(16) PerFrame
	{
        uint _lightCnt = 0;
        Vector3 _pad0;

        // direction light;
        Vector4 _WorldSpaceLightPos0;
        Vector4 _LightColor0;
	};

	struct alignas(16) PerCamera
	{
		CameraInfo _camera;

		CascadedInfo _shadow;

        Vector4 _time; // (t/20, t, t*2, t*3)
        Vector4 _sinTime; // sin(t/8), sin(t/4), sin(t/2), sin(t)
        Vector4 _cosTime; // cos(t/8), cos(t/4), cos(t/2), cos(t)

        // dont use smoothdt
        Vector4 _deltaTime; // dt, 1/dt, smoothdt, 1/smoothdt

        // x = width
       // y = height
       // z = 1 + 1.0/width
       // w = 1 + 1.0/height
        Vector4 _screen;

        /*
            DX:
            EZ  = (n * f) / (f - z * (f - n))
            LZ  = (eyeZ - n) / (f - n) = z / (f - z * (f - n))
            LZ2 = eyeZ / f = n / (f - z * (f - n))
        */
        // Values used to linearize the Z buffer (http://www.humus.name/temp/Linearize%20depth.txt)
        // x = 1-far/near
        // y = far/near
        // z = x/far
        // w = y/far
        Vector4 _ZBufferParams;
	};
}