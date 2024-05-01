/**

    @file      Culling.h
    @brief     culling helper
    @details   frustum culling helper
    @author    LWT
    @date      30.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\math.h>

namespace graphics
{
	struct CameraInfo;

	struct Plane
	{
		Plane() :
			_normal(0.f, 1.f, 0.f)
			, _position(0.f)
		{

		}

		Plane(math::Vector3 normal, math::Vector3 pos) :
			_normal(normal)
			, _position(pos)
		{

		}

		math::Vector3 _normal;

		math::Vector3 _position;
	};

	struct FrustumInfo
	{
		float _near;
		float _far;
		float _aspect;
		float _fov;
		Vector3 _position;

		Matrix _view;
	};

	struct Frustum
	{
		Frustum() :
			_topFace()
			, _bottomFace()
			, _rightFace()
			, _leftFace()
			, _farFace()
			, _nearFace()
		{
		}

		Plane _topFace;

		Plane _bottomFace;

		Plane _rightFace;

		Plane _leftFace;

		Plane _farFace;

		Plane _nearFace;

		math::Vector3 _camLook;

		math::Vector3 _camUp;

		math::Vector3 _camRight;
	};

	class CullingHelper
	{
	public:
		static bool ViewFrustumCullingBoundingBox(
			const Frustum& frustum,
			const math::Matrix& worldTM,
			const math::Vector3& boundMin,
			const math::Vector3& boundMax
		);

		static bool ViewFrustumCullingBoundingBox(
			math::Matrix& worldTM,
			math::Vector3& extents,
			math::Vector3& center,
			const Frustum& frustum,
			math::Vector3& outWorldTranslatedExtent,
			math::Vector3& outWorldTranslatedCenterPos
		);

		static bool ViewFrustumCullingBoundingBox(
			const Frustum& frustum,
			const math::Vector3& outWorldTranslatedExtent,
			const math::Vector3& outWorldTranslatedCenterPos
		);

		static bool ViewFrustumCullingBoundingVolume(
			const math::Vector3& worldPos,
			float radius,
			const Frustum& frustum
		);

		static void CreateFrustumFromCamera(const CameraInfo& camera, Frustum& outFrustum);

		static Frustum CreateFrustum(const FrustumInfo& info);

	private:
		static bool IsForwardPlane(
			math::Vector3& centerPos,
			math::Vector3& boundingBox,
			const Plane& plane
		);
		static bool IsForwardPlaneBoundingVolume(
			const math::Vector3& centerPos,
			float radius,
			const Plane& plane
		);

	};

}