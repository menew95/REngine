#include <rengine\core\scene\scene.h>

#include <rengine\core\object\GameObject.h>

namespace rengine
{
	Scene::Scene()
	: Object()
	{
		auto _camGO = GameObject::Instantiate();
		_camGO->SetName(L"Main Camera");

		auto _lightGO = GameObject::Instantiate();
		_lightGO->SetName(L"Directional Light");

		m_rootGameObjects.push_back(_camGO);
		m_rootGameObjects.push_back(_lightGO);
	}
	Scene::Scene(uuid uuid)
		: Object(uuid)
	{

	}
	Scene::Scene(uuid uuid, tstring name)
		: Object(uuid, name)
	{

	}
	Scene::Scene(uuid uuid, tstring name, tstring type)
		: Object(uuid, name, type)
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
}