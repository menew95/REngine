#pragma once

#include <common\math.h>

namespace boost
{
	namespace serialization
	{
		class access;
	}

}

struct VertexAttribute
{
	math::Vector3 _position = math::Vector3::Zero;
	math::Vector4 _color = math::Vector4::One;
	math::Vector2 _uv = math::Vector2::Zero;
	math::Vector3 _normal = math::Vector3::Zero;
	math::Vector3 _tangent = math::Vector3::Zero;
	uint32 _bone[4] = { UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX };
	float _weight[4] = { 0.f, 0.f, 0.f, 0.f };

	friend class boost::serialization::access;

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& _position;
		ar& _color;
		ar& _uv;
		ar& _normal;
		ar& _tangent;
		ar& _bone;
		ar& _weight;
	}
};