#include <rengine\core\script\LightMove.h>

#include <rengine\core\component\Transform.h>

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
	;
}

namespace rengine
{
	LightMove::LightMove(const uuid& uuid)
		: Component(uuid, TEXT("LightMove"))
	{
	}

	void LightMove::Update()
	{
		auto _q = GetTransform()->GetLocalRotation();

		Vector3 _rotate{1, 0, 0};

		const Vector3 radianEulers = _rotate * Deg2Rad * Time::GetDeltaTime() * m_speed;

		_q *= Quaternion::CreateFromYawPitchRoll(radianEulers.y, radianEulers.x, radianEulers.z);

		GetTransform()->SetLocalRotation(_q);
	}
}