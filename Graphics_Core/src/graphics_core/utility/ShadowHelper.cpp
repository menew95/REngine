﻿#include <graphics_core\object\PerFrame.h>
#include <graphics_core\utility\ShadowHelper.h>

static const math::Vector3 g_vFLTMAX = { FLT_MAX, FLT_MAX, FLT_MAX };
static const math::Vector3 g_vFLTMIN = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

namespace graphics
{
	void ShadowHelper::CalculateCascadeShadowSlices(PerCamera& perFrame, float near, float far, float fovAngleY, float aspectRatio, float* cascadeOffset
		, CascadeShadowSlice cascadeShadowInfos[4], int shadowMapTextureSize)
	{
		constexpr int cascadeCnt = 4;

		//zero to 1.f
		float cascadeSplits[cascadeCnt + 1] = { 0.f, };
		for (int idx = 1; idx < cascadeCnt + 1; idx++)
		{
			cascadeSplits[idx] = cascadeOffset[idx - 1];
		}

		math::Vector3 frustumCornerPoint[8] =
		{
			 {-1.f, +1.f, +0.f} //near
			,{-1.f, -1.f, +0.f} //near
			,{+1.f, -1.f, +0.f} //near
			,{+1.f, +1.f, +0.f} //near
			,{-1.f, +1.f, +1.f} //far
			,{-1.f, -1.f, +1.f}	//far
			,{+1.f, -1.f, +1.f} //far
			,{+1.f, +1.f, +1.f} //far
		};

		auto viewProjInv = perFrame._camera._viewProjInv;

		for (int pointIdx = 0; pointIdx < 8; ++pointIdx)
		{
			frustumCornerPoint[pointIdx] = math::Vector3::Transform(frustumCornerPoint[pointIdx], viewProjInv);
		}

		math::Vector3 cornerVector[4];

		for (int cornerIdx = 0; cornerIdx < 4; ++cornerIdx)
		{
			cornerVector[cornerIdx] = frustumCornerPoint[cornerIdx + 4] - frustumCornerPoint[cornerIdx];
		}

		//프러스텀을 자른다
		math::Vector3 cascadeBoundingBox[cascadeCnt][8];

		for (int cascadeIdx = 0; cascadeIdx < cascadeCnt; ++cascadeIdx)
		{
			for (int boundingBoxIdx = 0; boundingBoxIdx < 4; ++boundingBoxIdx)
			{
				math::Vector3 nearDist = cornerVector[boundingBoxIdx] * cascadeSplits[cascadeIdx];
				math::Vector3 farDist = cornerVector[boundingBoxIdx] * cascadeSplits[cascadeIdx + 1];

				cascadeBoundingBox[cascadeIdx][boundingBoxIdx] = frustumCornerPoint[boundingBoxIdx] + nearDist;
				cascadeBoundingBox[cascadeIdx][boundingBoxIdx + 4] = frustumCornerPoint[boundingBoxIdx] + farDist;
			}
		}

		//중점구하기
		for (int cascadeIdx = 0; cascadeIdx < cascadeCnt; cascadeIdx++)
		{
			for (int boundingBoxPointIdx = 0; boundingBoxPointIdx < 8; ++boundingBoxPointIdx)
			{
				cascadeShadowInfos[cascadeIdx]._frustumCenter += cascadeBoundingBox[cascadeIdx][boundingBoxPointIdx];
			}

			cascadeShadowInfos[cascadeIdx]._frustumCenter /= 8.f;
		}


		for (int cascadeIdx = 0; cascadeIdx < cascadeCnt; cascadeIdx++)
		{
			for (int boundingBoxPointIdx = 0; boundingBoxPointIdx < 8; boundingBoxPointIdx++)
			{
				float dist = (cascadeBoundingBox[cascadeIdx][boundingBoxPointIdx] - cascadeShadowInfos[cascadeIdx]._frustumCenter).Length();
				cascadeShadowInfos[cascadeIdx]._frustumRadius = std::max<float>(cascadeShadowInfos[cascadeIdx]._frustumRadius, dist);
			}

			//오차줄이기용 코드인것같다.
			//cascadeShadowInfos[cascadeIdx]._frustumRadius = std::floor(cascadeShadowInfos[cascadeIdx]._frustumRadius * 16.f) / 16.f;
		}


		//texel size만큼	일정하게 분포시킨다.
		for (int cascadeIdx = 0; cascadeIdx < cascadeCnt; cascadeIdx++)
		{
			float unitPerTexel = (cascadeShadowInfos[cascadeIdx]._frustumRadius * 2) / float(shadowMapTextureSize);

			cascadeShadowInfos[cascadeIdx]._frustumRadius /= unitPerTexel;
			cascadeShadowInfos[cascadeIdx]._frustumRadius = floor(cascadeShadowInfos[cascadeIdx]._frustumRadius);
			cascadeShadowInfos[cascadeIdx]._frustumRadius *= unitPerTexel;
		}
	}

	void ShadowHelper::CalculateCascadeShadowSlices2(
		const CameraInfo& cameraInfo, float* cascadeOffset,
		CascadeShadowSlice cascadeShadowInfos[4], int shadowMapTextureSize, math::Vector3 lightDirection)
	{
		constexpr int cascadeCnt = 4;

		//zero to 1.f
		float cascadeSplits[cascadeCnt + 1] = { 0.f, };
		for (int idx = 1; idx < cascadeCnt + 1; idx++)
		{
			cascadeSplits[idx] = cascadeOffset[idx - 1];
		}

		math::Vector4 frustumCornerPoint[8] =
		{
			 {-1.f, +1.f, +0.f, 1.f} //near
			,{-1.f, -1.f, +0.f, 1.f} //near
			,{+1.f, -1.f, +0.f, 1.f} //near
			,{+1.f, +1.f, +0.f, 1.f} //near
			,{-1.f, +1.f, +1.f, 1.f} //far
			,{-1.f, -1.f, +1.f, 1.f} //far
			,{+1.f, -1.f, +1.f, 1.f} //far
			,{+1.f, +1.f, +1.f, 1.f} //far
		};

		auto viewProjInv = cameraInfo._viewProjInv;

		for (int pointIdx = 0; pointIdx < 8; ++pointIdx)
		{
			frustumCornerPoint[pointIdx] = math::Vector4::Transform(frustumCornerPoint[pointIdx], viewProjInv);
			frustumCornerPoint[pointIdx] /= frustumCornerPoint[pointIdx].w;
		}

		//near -> far로 가는 벡터가 만들어짐.
		math::Vector3 cornerVector[4];

		for (int cornerIdx = 0; cornerIdx < 4; ++cornerIdx)
		{
			math::Vector3 test1 = math::Vector3{ frustumCornerPoint[cornerIdx].x, frustumCornerPoint[cornerIdx].y, frustumCornerPoint[cornerIdx].z };
			math::Vector3 test2 = math::Vector3{ frustumCornerPoint[cornerIdx + 4].x, frustumCornerPoint[cornerIdx + 4].y, frustumCornerPoint[cornerIdx + 4].z };
			cornerVector[cornerIdx] = test2 - test1;
		}

		//프러스텀을 자른다
		math::Vector3 cascadeBoundingBox[cascadeCnt][8];

		for (int cascadeIdx = 0; cascadeIdx < cascadeCnt; ++cascadeIdx)
		{
			cascadeShadowInfos[cascadeIdx]._orthoMin = g_vFLTMAX;
			cascadeShadowInfos[cascadeIdx]._orthoMax = g_vFLTMIN;

			for (int boundingBoxIdx = 0; boundingBoxIdx < 4; ++boundingBoxIdx)
			{
				//분할면의 위치로 가는 벡터
				math::Vector3 nearDist = cornerVector[boundingBoxIdx] * cascadeSplits[cascadeIdx];
				math::Vector3 farDist = cornerVector[boundingBoxIdx] * cascadeSplits[cascadeIdx + 1];

				//near 평면에서 벡터만큼 가서, 바운딩박스의 point를 구한다.
				cascadeBoundingBox[cascadeIdx][boundingBoxIdx] = frustumCornerPoint[boundingBoxIdx] + nearDist;
				cascadeBoundingBox[cascadeIdx][boundingBoxIdx + 4] = frustumCornerPoint[boundingBoxIdx] + farDist;
			}
		}

		//중점구하기

		for (int cascadeIdx = 0; cascadeIdx < cascadeCnt; cascadeIdx++)
		{
			for (int boundingBoxPointIdx = 0; boundingBoxPointIdx < 8; boundingBoxPointIdx++)
			{
				float dist = (cascadeBoundingBox[cascadeIdx][boundingBoxPointIdx] - cascadeShadowInfos[cascadeIdx]._frustumCenter).Length();
				cascadeShadowInfos[cascadeIdx]._frustumRadius = std::max<float>(cascadeShadowInfos[cascadeIdx]._frustumRadius, dist);
			}
		}

		for (int cascadeIdx = 0; cascadeIdx < cascadeCnt; cascadeIdx++)
		{
			for (int boundingBoxPointIdx = 0; boundingBoxPointIdx < 8; ++boundingBoxPointIdx)
			{
				//바운딩 박스로부터 중점을 구한다.
				cascadeShadowInfos[cascadeIdx]._frustumCenter += cascadeBoundingBox[cascadeIdx][boundingBoxPointIdx];
			}

			cascadeShadowInfos[cascadeIdx]._frustumCenter /= 8.f;
			// - (light * (250 + shadowInfo._frustumRadius));
			math::Vector3 shadowCamPos = cascadeShadowInfos[cascadeIdx]._frustumCenter - (lightDirection * (250 + cascadeShadowInfos[cascadeIdx]._frustumRadius));
			auto view = math::Matrix::CreateLookAt(shadowCamPos, cascadeShadowInfos[cascadeIdx]._frustumCenter, math::Vector3(0.f, 1.f, 0.f));

			for (int boundingBoxIdx = 0; boundingBoxIdx < 4; ++boundingBoxIdx)
			{
				//바운딩박스의 포지션을 라이트 좌표계로 옮긴다. 여기서 lightMat은 light의 뷰매트릭스(월드인버스)이다.
				math::Vector3 lightSpacePoint1 = math::Vector3::Transform(cascadeBoundingBox[cascadeIdx][boundingBoxIdx], view);
				math::Vector3 lightSpacePoint2 = math::Vector3::Transform(cascadeBoundingBox[cascadeIdx][boundingBoxIdx + 4], view);

				//light 좌표계에서의 x, y의 min, max를 구한다. 
				cascadeShadowInfos[cascadeIdx]._orthoMin = math::Vector3::Min(cascadeShadowInfos[cascadeIdx]._orthoMin, lightSpacePoint1);
				cascadeShadowInfos[cascadeIdx]._orthoMin = math::Vector3::Min(cascadeShadowInfos[cascadeIdx]._orthoMin, lightSpacePoint2);

				cascadeShadowInfos[cascadeIdx]._orthoMax = math::Vector3::Max(cascadeShadowInfos[cascadeIdx]._orthoMax, lightSpacePoint1);
				cascadeShadowInfos[cascadeIdx]._orthoMax = math::Vector3::Max(cascadeShadowInfos[cascadeIdx]._orthoMax, lightSpacePoint2);
			}
		}


		math::Vector3 vWorldUnitsPerTexel;

		// This code removes the shimmering effect along the edges of shadows due to
		// the light changing to fit the camera.
		{
			for (int cascadeIdx = 0; cascadeIdx < cascadeCnt; cascadeIdx++)
			{
				int m_iPCFBlurSize = 3;

				// We calculate a looser bound based on the size of the PCF blur.  This ensures us that we're 
				// sampling within the correct map.
				float fScaleDuetoBlureAMT = ((float)(m_iPCFBlurSize * 2 + 1)
					/ (float)shadowMapTextureSize);
				math::Vector3 vScaleDuetoBlureAMT = { fScaleDuetoBlureAMT, fScaleDuetoBlureAMT,  0.f };

				float fNormalizeByBufferSize = (1.0f / (float)shadowMapTextureSize);
				math::Vector3 vNormalizeByBufferSize = { fNormalizeByBufferSize, fNormalizeByBufferSize,  0.f };

				// We calculate the offsets as a percentage of the bound.
				math::Vector3 vBoarderOffset = cascadeShadowInfos[cascadeIdx]._orthoMax - cascadeShadowInfos[cascadeIdx]._orthoMin;
				vBoarderOffset *= 0.5f;
				vBoarderOffset *= vScaleDuetoBlureAMT;
				cascadeShadowInfos[cascadeIdx]._orthoMax += vBoarderOffset;
				cascadeShadowInfos[cascadeIdx]._orthoMin -= vBoarderOffset;

				// The world units per texel are used to snap  the orthographic projection
				// to texel sized increments.  
				// Because we're fitting tighly to the cascades, the shimmering shadow edges will still be present when the 
				// camera rotates.  However, when zooming in or strafing the shadow edge will not shimmer.
				vWorldUnitsPerTexel = cascadeShadowInfos[cascadeIdx]._orthoMax - cascadeShadowInfos[cascadeIdx]._orthoMin;
				vWorldUnitsPerTexel *= vNormalizeByBufferSize;

				cascadeShadowInfos[cascadeIdx]._orthoMin = cascadeShadowInfos[cascadeIdx]._orthoMin / vWorldUnitsPerTexel;
				cascadeShadowInfos[cascadeIdx]._orthoMin = math::Vector3{ ceil(cascadeShadowInfos[cascadeIdx]._orthoMin.x), ceil(cascadeShadowInfos[cascadeIdx]._orthoMin.y), ceil(cascadeShadowInfos[cascadeIdx]._orthoMin.z) };
				cascadeShadowInfos[cascadeIdx]._orthoMin *= vWorldUnitsPerTexel;

				cascadeShadowInfos[cascadeIdx]._orthoMax = cascadeShadowInfos[cascadeIdx]._orthoMax / vWorldUnitsPerTexel;
				cascadeShadowInfos[cascadeIdx]._orthoMax = math::Vector3{ ceil(cascadeShadowInfos[cascadeIdx]._orthoMax.x), ceil(cascadeShadowInfos[cascadeIdx]._orthoMax.y), ceil(cascadeShadowInfos[cascadeIdx]._orthoMax.z) };
				cascadeShadowInfos[cascadeIdx]._orthoMax *= vWorldUnitsPerTexel;
			}
		}




		////texel size만큼	일정하게 분포시킨다.
		//for (int cascadeIdx = 0; cascadeIdx < cascadeCnt; cascadeIdx++)
		//{
		//	float unitPerTexel = (cascadeShadowInfos[cascadeIdx]._frustumRadius * 2) / float(shadowMapTextureSize);

		//	cascadeShadowInfos[cascadeIdx]._frustumRadius /= unitPerTexel;
		//	cascadeShadowInfos[cascadeIdx]._frustumRadius = floor(cascadeShadowInfos[cascadeIdx]._frustumRadius);
		//	cascadeShadowInfos[cascadeIdx]._frustumRadius *= unitPerTexel;
		//}
	}

	void ShadowHelper::CalcuateViewProjectionMatrixFromCascadeSlice(const CascadeShadowSlice& shadowInfo, const math::Vector3& lightDir, math::Matrix& outMatrix)
	{
		math::Vector3 light = lightDir;
		light.Normalize();

		//쉐도우 카메라의 포지션과 proj matrix near, far 하드코딩되어있음. 문제가 지금 많은 코드이다.. 쉐도우 버그의 주범임.
		//VSM과 Blend Cascade Slice를 추가하기전까지는 아마 고정된 값으로 해야할 것 같습니다.

		math::Vector3 shadowCamPos = shadowInfo._frustumCenter - (light * 200);

		auto view = math::Matrix::CreateLookAt(shadowCamPos, shadowInfo._frustumCenter, math::Vector3(0.f, 1.f, 0.f));
		auto proj = math::Matrix::CreateOrthographicOffCenter(-shadowInfo._frustumRadius, shadowInfo._frustumRadius, -shadowInfo._frustumRadius, shadowInfo._frustumRadius, 1, 500);

		outMatrix = view * proj;
	}

	void ShadowHelper::CalcuateViewProjectionMatrixFromCascadeSlice2(const CascadeShadowSlice& shadowInfo,
		const math::Vector3& lightDir, math::Matrix& outMatrix)
	{
		math::Vector3 light = lightDir;
		light.Normalize();

		//쉐도우 카메라의 포지션과 proj matrix near, far 하드코딩되어있음. 문제가 지금 많은 코드이다.. 쉐도우 버그의 주범임.
		//VSM과 Blend Cascade Slice를 추가하기전까지는 아마 고정된 값으로 해야할 것 같습니다.


		float x = shadowInfo._orthoMax.x - shadowInfo._orthoMin.x;
		float y = shadowInfo._orthoMax.y - shadowInfo._orthoMin.y;
		if (x <= 0.0001f || y <= 0.0001f)
			return;
		//x /= 2.0f;

		//y /= 2.0f;

		//fabs(x);
		//fabs(y);
		math::Vector3 shadowCamPos = shadowInfo._frustumCenter - (light * (250 + shadowInfo._frustumRadius));

		auto view = math::Matrix::CreateLookAt(shadowCamPos, shadowInfo._frustumCenter, math::Vector3(0.f, 1.f, 0.f));
		auto proj = math::Matrix::CreateOrthographicOffCenter(shadowInfo._orthoMin.x, shadowInfo._orthoMax.x, shadowInfo._orthoMin.y, shadowInfo._orthoMax.y, 1, 400 + shadowInfo._frustumRadius);

		//auto proj = math::Matrix::CreateOrthographicOffCenter(-x, +x, -y, +y, 0, 500);

		outMatrix = view * proj;

	}
}