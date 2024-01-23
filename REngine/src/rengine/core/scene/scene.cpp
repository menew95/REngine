#include <rengine\core\scene\Scene.h>

#include <rengine\core\object\GameObject.h>
#include <rengine\core\component\Transform.h>

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
	Scene::Scene(uuid uuid)
		: Object(uuid, TEXT("Scene"), TEXT("Scene"))
	{

	}

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

	void Scene::AddRootGameObject(std::shared_ptr<GameObject> go)
	{
		m_rootGameObjects.push_back(go);

		/*for (auto& trans : go->GetTransform()->GetChilds())
		{
			auto _transform = trans.lock();

			if(_transform == nullptr)
				continue;

			auto _childObj = _transform->GetGameObject().lock();

			if(_childObj == nullptr)
				continue;

			_childObj->SetScene(shared_from_this());
		}*/
	}
}