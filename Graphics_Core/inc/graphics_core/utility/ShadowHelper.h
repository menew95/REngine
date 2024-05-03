/**

    @file      ShadowHelper.h
    @brief     
    @details   ~
    @author    Cool Guy
    @date      2.05.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\math.h>

namespace graphics
{
	struct PerFrame;

	struct CascadeShadowSlice
	{
		Vector3 _frustumCenter;

		float _frustumRadius = 0;

		Vector3 _orthoMin;

		Vector3 _orthoMax;
	};

	struct CascadeShadowInfo
	{
		Matrix shadowMatrix[4];

		float _cascadeSliceOffset[4];

		float _cascadeScale[4];
	};

	class ShadowHelper
	{
	public:
		//void CalculateCascadeShadowSlices(ConstantBufferPerFrame& perFrameBuffer, float near, float far, float range);

		//일단은 무조건 네개만 넣으세요..
		static void CalculateCascadeShadowSlices(PerCamera& perCameraBuffer, float near, float far, float FovAngleY, float AspectRatio, float* cascadeOffset, CascadeShadowSlice cascadeShadowInfos[4], int
			shadowMapTextureSize);

		static void CalculateCascadeShadowSlices2(const CameraInfo& cameraInfo, float* cascadeOffset, CascadeShadowSlice cascadeShadowInfos[4], int
			shadowMapTextureSize, Vector3 lightDirection);

		static void CalcuateViewProjectionMatrixFromCascadeSlice(const CascadeShadowSlice& shadowInfo, const Vector3& lightDir, Matrix& outMatrix);

		static void CalcuateViewProjectionMatrixFromCascadeSlice2(const CascadeShadowSlice& shadowInfo, const Vector3& lightDir, Matrix& outMatrix);
	};
}