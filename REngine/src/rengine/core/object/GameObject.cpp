﻿#include <rengine\core\object\GameObject.h>
#include <rengine\core\component\Transform.h>

#include <rttr\registration.h>

RTTR_REGISTRATION
{
	rttr::registration::class_<rengine::GameObject>("GameObject")
	.constructor<uuid>()
	.property("Active Self", &rengine::GameObject::m_bActiveSelf, rttr::detail::private_access())
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::BOOL),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::BOOL)
	)
	.property("Tag", &rengine::GameObject::m_tag, rttr::detail::private_access())
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::ENUM),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::ENUM)
	)
	.property("Layer", &rengine::GameObject::m_layer, rttr::detail::private_access())
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::ENUM),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::ENUM)
	)
	.property("Static", &rengine::GameObject::m_bIsStatic, rttr::detail::private_access())
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::BOOL),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::BOOL)
	)
	;
}

namespace rengine
{
	RENGINE_API shared_ptr<GameObject> GameObject::Instantiate(shared_ptr<Transform> parent)
	{
		auto _newGO = make_shared<GameObject>();

		auto _trans = _newGO->AddComponent<Transform>();

		if(parent != nullptr) parent->AddChild(_trans);

		_newGO->GetTransform() = _trans;

		return _newGO;
	}

	GameObject::GameObject()
		: Object()
	{
	}

	GameObject::GameObject(uuid uuid)
		: Object(uuid)
	{

	}
	GameObject::GameObject(uuid uuid, tstring name)
		: Object(uuid, name)
	{
	}
}