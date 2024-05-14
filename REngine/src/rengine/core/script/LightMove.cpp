#include <rengine\core\script\LightMove.h>

#include <rengine\core\component\Transform.h>
#include <rengine\core\component\Light.h>
#include <rengine\core\object\GameObject.h>

#include <rengine\System\Time.h>

#include <rttr\registration.h>

RTTR_REGISTRATION
{
	rttr::registration::class_<rengine::LightMove>("LightMove")
	.constructor<const uuid&>()
	.property("speed", &rengine::LightMove::GetSpeed, &rengine::LightMove::SetSpeed)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT)
	)
	.property("Pos1", &rengine::LightMove::GetPos1, &rengine::LightMove::SetPos1)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::VECTOR3),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::VECTOR3)
	)
	.property("Pos2", &rengine::LightMove::GetPos2, &rengine::LightMove::SetPos2)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::VECTOR3),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::VECTOR3)
	)
	.property("Pos3", &rengine::LightMove::GetPos3, &rengine::LightMove::SetPos3)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::VECTOR3),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::VECTOR3)
	)
	.property("Pos4", &rengine::LightMove::GetPos4, &rengine::LightMove::SetPos4)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::VECTOR3),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::VECTOR3)
	)
	;
}

namespace rengine
{
	LightMove::LightMove(const uuid& uuid)
		: Component(uuid, TEXT("LightMove"))
	{
	}

	void LightMove::Start()
	{
		m_lightType = m_pGameObject.lock()->GetComponent<Light>()->GetLightType();

	}

	void LightMove::Update()
	{
		if (m_lightType == (uint32)LightType::Directional)
		{
			auto _q = GetTransform()->GetLocalRotation();

			Vector3 _rotate{ 0, 1, 0 };

			const Vector3 radianEulers = _rotate * Deg2Rad * Time::GetDeltaTime() * m_speed;

			_q *= Quaternion::CreateFromYawPitchRoll(radianEulers.y, radianEulers.x, radianEulers.z);

			GetTransform()->SetLocalRotation(_q);
		}
		else if(m_lightType == (uint32)LightType::Point)
		{
			auto _p = GetTransform()->GetWorldPosition();

			if (Vector3::Distance(position[current], _p) < 0.1f)
			{
				current++;

				if (current == 4)
				{
					current = 0;
				}
			}

			Vector3 _dir = position[current] - _p;

			_dir.Normalize();

			_p += _dir * Time::GetDeltaTime() * m_speed;

			GetTransform()->SetWorldPosition(_p);
		}
	}
}