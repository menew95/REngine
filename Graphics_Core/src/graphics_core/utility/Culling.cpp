#include <graphics_core\utility\Culling.h>

#include <graphics_core\resource\CameraBuffer.h>

namespace graphics
{
	bool CullingHelper::ViewFrustumCullingBoundingBox(const Frustum& frustum, math::Matrix& worldTM, math::Vector3& boundMin, math::Vector3& boundMax)
	{
		math::Vector3 center = (boundMax + boundMin) / 2;
		math::Vector3 extents = (boundMax - boundMin) / 2;

		//월드 축에 정렬한 바운딩 박스를 구한다.
		math::Vector4 localx{ extents.x, 0.f, 0.f, 0.f };
		math::Vector4 localy{ 0.f, extents.y, 0.f, 0.f };
		math::Vector4 localz{ 0.f, 0.f, extents.z, 0.f };

		math::Vector4 scaledxV4 = math::Vector4::Transform(localx, worldTM);
		math::Vector4 scaledyV4 = math::Vector4::Transform(localy, worldTM);
		math::Vector4 scaledzV4 = math::Vector4::Transform(localz, worldTM);

		// 바운딩 박스의 중심을 계산한다.
		math::Vector4 scaledCenterV4 = math::Vector4::Transform(math::Vector4{ center.x, center.y,center.z,1.f }, worldTM);

		math::Vector3 scaledx{ scaledxV4 };
		math::Vector3 scaledy{ scaledyV4 };
		math::Vector3 scaledz{ scaledzV4 };

		float newx = fabs(scaledx.Dot(math::Vector3::Right)) + fabs(scaledy.Dot(math::Vector3::Right)) + fabs(scaledz.Dot(math::Vector3::Right));
		float newy = fabs(scaledx.Dot(math::Vector3::Up)) + fabs(scaledy.Dot(math::Vector3::Up)) + fabs(scaledz.Dot(math::Vector3::Up));
		float newz = fabs(scaledx.Dot(math::Vector3::Forward)) + fabs(scaledy.Dot(math::Vector3::Forward)) + fabs(scaledz.Dot(math::Vector3::Forward));

		math::Vector3 min = { -newx, -newy, -newz };
		math::Vector3 max = { newx, newy, newz };

		math::Vector3 centerPos{ scaledCenterV4.x, scaledCenterV4.y, scaledCenterV4.z };

		bool rb = IsForwardPlane(centerPos, max, frustum._rightFace);
		bool lb = IsForwardPlane(centerPos, max, frustum._leftFace);
		bool tb = IsForwardPlane(centerPos, max, frustum._topFace);
		bool bb = IsForwardPlane(centerPos, max, frustum._bottomFace);
		bool nb = IsForwardPlane(centerPos, max, frustum._nearFace);
		bool fb = IsForwardPlane(centerPos, max, frustum._farFace);

		return (rb && lb && tb && bb && nb && fb);
	}
	bool CullingHelper::ViewFrustumCullingBoundingBox(math::Matrix& worldTM, math::Vector3& extents, math::Vector3& center, const Frustum& frustum, math::Vector3& outWorldTranslatedExtent, math::Vector3& outWorldTranslatedCenterPos)
	{
		//월드 축에 정렬한 바운딩 박스를 구한다.
		math::Vector4 localx{ extents.x, 0.f, 0.f, 0.f };
		math::Vector4 localy{ 0.f, extents.y, 0.f, 0.f };
		math::Vector4 localz{ 0.f, 0.f, extents.z, 0.f };

		math::Vector4 scaledxV4 = math::Vector4::Transform(localx, worldTM);
		math::Vector4 scaledyV4 = math::Vector4::Transform(localy, worldTM);
		math::Vector4 scaledzV4 = math::Vector4::Transform(localz, worldTM);

		// 바운딩 박스의 중심을 계산한다.
		math::Vector4 scaledCenterV4 = math::Vector4::Transform(math::Vector4{ center.x, center.y,center.z,1.f }, worldTM);

		math::Vector3 scaledx{ scaledxV4 };
		math::Vector3 scaledy{ scaledyV4 };
		math::Vector3 scaledz{ scaledzV4 };

		float newx = fabs(scaledx.Dot(math::Vector3::Right)) + fabs(scaledy.Dot(math::Vector3::Right)) + fabs(scaledz.Dot(math::Vector3::Right));
		float newy = fabs(scaledx.Dot(math::Vector3::Up)) + fabs(scaledy.Dot(math::Vector3::Up)) + fabs(scaledz.Dot(math::Vector3::Up));
		float newz = fabs(scaledx.Dot(math::Vector3::Forward)) + fabs(scaledy.Dot(math::Vector3::Forward)) + fabs(scaledz.Dot(math::Vector3::Forward));

		math::Vector3 min = { -newx, -newy, -newz };
		math::Vector3 max = { newx, newy, newz };

		// math::Vector3 centerPos{ worldTM.m[3][0], worldTM.m[3][1], worldTM.m[3][2] };
		math::Vector3 centerPos{ scaledCenterV4.x, scaledCenterV4.y, scaledCenterV4.z };

		outWorldTranslatedExtent = max;
		outWorldTranslatedCenterPos = centerPos;

		bool rb = IsForwardPlane(centerPos, max, frustum._rightFace);
		bool lb = IsForwardPlane(centerPos, max, frustum._leftFace);
		bool tb = IsForwardPlane(centerPos, max, frustum._topFace);
		bool bb = IsForwardPlane(centerPos, max, frustum._bottomFace);
		bool nb = IsForwardPlane(centerPos, max, frustum._nearFace);
		bool fb = IsForwardPlane(centerPos, max, frustum._farFace);

		return (rb && lb && tb && bb && nb && fb);
	}

	bool CullingHelper::ViewFrustumCullingBoundingBox(const Frustum& frustum, const math::Vector3& outWorldTranslatedExtent, const math::Vector3& outWorldTranslatedCenterPos)
	{
		math::Vector3 max = outWorldTranslatedExtent;

		// math::Vector3 centerP
		math::Vector3 centerPos = outWorldTranslatedCenterPos;

		bool lb = IsForwardPlane(centerPos, max, frustum._leftFace);
		bool rb = IsForwardPlane(centerPos, max, frustum._rightFace);
		bool tb = IsForwardPlane(centerPos, max, frustum._topFace);
		bool bb = IsForwardPlane(centerPos, max, frustum._bottomFace);
		bool nb = IsForwardPlane(centerPos, max, frustum._nearFace);
		bool fb = IsForwardPlane(centerPos, max, frustum._farFace);

		return (rb && lb && tb && bb && nb && fb);
	}

	bool CullingHelper::ViewFrustumCullingBoundingVolume(const math::Vector3& worldPos, float radius, const Frustum& frustum)
	{
		bool rb = IsForwardPlaneBoundingVolume(worldPos, radius, frustum._rightFace);
		bool lb = IsForwardPlaneBoundingVolume(worldPos, radius, frustum._leftFace);
		bool tb = IsForwardPlaneBoundingVolume(worldPos, radius, frustum._topFace);
		bool bb = IsForwardPlaneBoundingVolume(worldPos, radius, frustum._bottomFace);
		bool nb = IsForwardPlaneBoundingVolume(worldPos, radius, frustum._nearFace);
		bool fb = IsForwardPlaneBoundingVolume(worldPos, radius, frustum._farFace);

		return (rb && lb && tb && bb && nb && fb);
	}

	void CullingHelper::CreateFrustumFromCamera(const CameraInfo& camera, Frustum& outFrustum)
	{
		auto camMat = camera._view.Invert();

		outFrustum._camRight = math::Vector3{ camMat.m[0][0], camMat.m[0][1], camMat.m[0][2] };
		outFrustum._camUp = math::Vector3{ camMat.m[1][0], camMat.m[1][1], camMat.m[1][2] };
		outFrustum._camLook = math::Vector3{ camMat.m[2][0], camMat.m[2][1], camMat.m[2][2] };

		outFrustum._camLook.Normalize();
		outFrustum._camRight.Normalize();
		outFrustum._camUp.Normalize();

		outFrustum._nearFace = { outFrustum._camLook , camera._cameraWorldPos + outFrustum._camLook * camera._near };
		outFrustum._farFace = { -outFrustum._camLook , camera._cameraWorldPos + outFrustum._camLook * camera._far};

		math::Vector3 FarVec = outFrustum._camLook * camera._far;
		float halfHorizon = camera._far * tanf(camera._fov * 0.5f);//halfVertical * camera._aspectRatio;
		float halfVertical = halfHorizon * camera._aspectRatio;//camera._far * tanf(camera._fov * 0.5f);

		math::Vector3 rightNormal = FarVec + outFrustum._camRight * halfHorizon;
		math::Vector3 leftNormal = FarVec - outFrustum._camRight * halfHorizon;

		rightNormal.Normalize();
		leftNormal.Normalize();

		outFrustum._rightFace = { rightNormal.Cross(outFrustum._camUp) , camera._cameraWorldPos };
		outFrustum._leftFace = { outFrustum._camUp.Cross(leftNormal) , camera._cameraWorldPos };

		math::Vector3 upNormal = FarVec + outFrustum._camUp * halfVertical;
		math::Vector3 downNormal = FarVec - outFrustum._camUp * halfVertical;

		upNormal.Normalize();
		downNormal.Normalize();

		//outFrustum._topFace = {  upNormal.Cross(outFrustum._camRight) , camera._cameraPosition };
		//outFrustum._bottomFace = { outFrustum._camRight.Cross(downNormal) , camera._cameraPosition };

		outFrustum._topFace = { outFrustum._camRight.Cross(upNormal) , camera._cameraWorldPos };
		outFrustum._bottomFace = { downNormal.Cross(outFrustum._camRight) , camera._cameraWorldPos };

		outFrustum._nearFace._normal.Normalize();
		outFrustum._farFace._normal.Normalize();
		outFrustum._rightFace._normal.Normalize();
		outFrustum._leftFace._normal.Normalize();
		outFrustum._topFace._normal.Normalize();
		outFrustum._bottomFace._normal.Normalize();
	}

	bool CullingHelper::IsForwardPlane(math::Vector3& centerPos, math::Vector3& boundingBox, const Plane& plane)
	{
		//bounding box의 positive extent와 plane normal간의 내적. 
		float r = boundingBox.x * fabs(plane._normal.x) + boundingBox.y * fabs(plane._normal.y) + boundingBox.z * fabs(plane._normal.z);

		//signed distance
		float distPlane = plane._normal.Dot(plane._position);
		float s = plane._normal.Dot(centerPos) - distPlane;

		return -r <= s;
	}
	bool CullingHelper::IsForwardPlaneBoundingVolume(const math::Vector3& centerPos, float radius, const Plane& plane)
	{
		math::Vector3 pos = centerPos - plane._position;

		//거리구하기
		float dist = pos.Dot(plane._normal);

		//-radius 보다 길이가 크면 plain 내부에 있는 것임.
		return dist > -radius;
	}
}