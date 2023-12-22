#include <rengine\core\component\Component.h>

#include <rengine\core\object\GameObject.h>

#include <rttr\registration.h>

RTTR_REGISTRATION
{
	rttr::registration::class_<rengine::Component>("Component")
	.constructor<std::shared_ptr<rengine::GameObject>&>()
	.constructor<std::shared_ptr<rengine::GameObject>&, uuid>()
	.property("m_bEnable", &rengine::Component::GetEnable, &rengine::Component::SetEnable)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::BOOL),
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::BOOL)
	);
}

namespace rengine
{
	Component::Component(std::shared_ptr<GameObject>& gameObj)
	: Object()
	, m_pGameObject(gameObj)
	{
	}

	Component::Component(std::shared_ptr<GameObject>& gameObj, uuid uuid)
	: Object(uuid)
	, m_pGameObject(gameObj)
	{
	}

	Component::~Component()
	{
	}

	void Component::SetEnable(bool value)
	{
		if (/*m_pGameObject->GetActiveInHierarchy() &&*/ m_bEnable != value)
		{
			m_bEnable = value;
		}
	}
}