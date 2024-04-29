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
		// 게임 오브젝트에 등록된 스마트 포인터부터 먼저 삭제
		m_gameObjects.clear();

		// 씬에 등록된 게임 루트 오브젝트들을 삭제
		for (auto& _go : m_rootGameObjects)
		{
			DestroyImmediate(_go);

			_go.reset();
		}

		m_rootGameObjects.clear();
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

	void Scene::AddGameObject(const std::shared_ptr<GameObject>& go)
	{
		// 게임 오브젝트 리스트에 등록된 게임 오브젝트가 아니라면 추가
		auto _iter = std::ranges::find_if(std::begin(m_gameObjects), std::end(m_gameObjects), 
				[&](auto& obj)
				{
					return obj->GetUUID() == go->GetUUID();
				}
			);

		if(_iter == std::end(m_gameObjects))
			m_gameObjects.push_back(go);

		/*if (go->GetTransform() != nullptr && go->GetTransform()->GetParent() == nullptr)
		{
			AddRootGameObject(go);
		}*/
	}

	void Scene::RemoveGameObject(const std::shared_ptr<GameObject>& go)
	{
		auto _iter = std::ranges::find_if(m_gameObjects.begin(), m_gameObjects.end()
			, [&go](auto& item)
			{
				return item->GetUUID() == go->GetUUID();
			}
		);

		// 오브젝트 팩토리에서 삭제할때 게임 오브젝트에 씬이 할당 되어있는데 찾지 못함
		assert(_iter != m_gameObjects.end());

		m_gameObjects.erase(_iter);
			
		RemoveRootGameObject(go);
	}

	void Scene::AddRootGameObject(std::shared_ptr<GameObject> go)
	{
		// 루트 게임 오브젝트 리스트에 등록된 게임 오브젝트가 아니라면 추가
		auto _iter = std::ranges::find_if(std::begin(m_rootGameObjects), std::end(m_rootGameObjects),
			[&](auto& obj)
			{
				return obj->GetUUID() == go->GetUUID();
			}
		);

		if (_iter == std::end(m_rootGameObjects))
			m_rootGameObjects.push_back(go);
	}

	void Scene::RemoveRootGameObject(std::shared_ptr<GameObject> go)
	{
		auto _iter = std::ranges::find_if(m_rootGameObjects.begin(), m_rootGameObjects.end()
			, [&go](auto& item)
			{
				return item->GetUUID() == go->GetUUID();
			}
		);

		if(_iter != m_rootGameObjects.end())
			m_rootGameObjects.erase(_iter);
	}
}