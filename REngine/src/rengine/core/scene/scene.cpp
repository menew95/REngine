#include <rengine\core\scene\Scene.h>

#include <rengine\core\object\GameObject.h>

#include <rengine\core\component\Camera.h>

#include <rttr\registration.h>

RTTR_REGISTRATION
{
	rttr::registration::class_<rengine::Scene>("Scene")
	.constructor<tstring>()
	/*.property_readonly("m_typeName", &rengine::Object::GetType)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::WSTRING)
	)
	.property("m_uuid", &rengine::Object::GetUUID, &rengine::Object::SetUUID)
	.property("m_objectName", &rengine::Object::GetName, &rengine::Object::SetName)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::WSTRING)
	)*/
	;
}

namespace rengine
{
	/*Scene::Scene()
	: Object()
	{
		auto _camGO = GameObject::Instantiate();
		_camGO->SetName(L"Main Camera");

		_camGO->AddComponent<Camera>();

		auto _lightGO = GameObject::Instantiate();
		_lightGO->SetName(L"Directional Light");

		m_rootGameObjects.push_back(_camGO);
		m_rootGameObjects.push_back(_lightGO);
	}*/

	Scene::Scene(uuid uuid)
		: Object(uuid, TEXT("Scene"), TEXT("Scene"))
	{

	}

	/*Scene::Scene(uuid uuid, tstring name)
		: Object(uuid, name, TEXT("Scene"))
	{
		auto _camGO = GameObject::Instantiate();
		_camGO->SetName(L"Main Camera");

		_camGO->AddComponent<Camera>();

		auto _lightGO = GameObject::Instantiate();
		_lightGO->SetName(L"Directional Light");

		m_rootGameObjects.push_back(_camGO);
		m_rootGameObjects.push_back(_lightGO);
	}*/

	/*Scene::Scene(uuid uuid, tstring name, tstring type)
		: Object(uuid, name, type)
	{

	}*/

	Scene::~Scene()
	{
		for (auto& _go : m_rootGameObjects)
		{
			_go.reset();
		}
	}

	std::shared_ptr<GameObject> Scene::FindGameObject(tstring& objectName)
	{
		auto _iter = find_if(m_rootGameObjects.begin(), m_rootGameObjects.end(), [&objectName](auto& go)
			{
				return go->GetName() == objectName;
			});

		if (_iter != m_rootGameObjects.end())
		{
			return *_iter;
		}

		return nullptr;
	}
}