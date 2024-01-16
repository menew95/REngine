#include <rengine\core\object\GameObject.h>
#include <rengine\core\component\Transform.h>

#include <rengine\System\ObjectFactory.h>

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
	.property("Tag", &rengine::GameObject::GetTag, &rengine::GameObject::SetTag)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::WSTRING),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::WSTRING)
	)
	.property("Layer", &rengine::GameObject::GetLayer, &rengine::GameObject::SetLayer)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::UINT32),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::UINT32)
	)
	.property("Static", &rengine::GameObject::GetIsStatic, &rengine::GameObject::SetIsStatic)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::BOOL),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::BOOL)
	)
	.property("Components", &rengine::GameObject::GetComponents, &rengine::GameObject::SetComponents)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::UUID)
	);
}

namespace rengine
{
	RENGINE_API shared_ptr<GameObject> GameObject::Instantiate(shared_ptr<Transform> parent)
	{
		//auto _newGO = make_shared<GameObject>();
		
		auto _newGO = ObjectFactory::GetInstance()->CreateObject<GameObject>();

		auto _trans = _newGO->AddComponent<Transform>();

		if(parent != nullptr) _trans->SetParent(parent);

		_newGO->SetTransform(_trans);

		return _newGO;
	}

	/*GameObject::GameObject()
		: Object()
	{
	}

	GameObject::GameObject(uuid uuid)
		: Object(uuid)
	{

	}*/

	GameObject::GameObject(uuid uuid)
		: Object(uuid, TEXT("Game Object"), TEXT("GameObject"))
	{
	}

	/*GameObject::GameObject(uuid uuid, tstring name)
		: Object(uuid, name, TEXT("GameObject"))
	{
	}*/
}