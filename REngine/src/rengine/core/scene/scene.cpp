#include <rengine\core\scene\scene.h>

#include <rengine\core\object\GameObject.h>

namespace rengine
{
	Scene::Scene()
	{

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
		for (auto& _go : m_RootGameObjects)
		{
			_go.reset();
		}
	}

	std::shared_ptr<GameObject> Scene::FindGameObject(tstring& objectName)
	{
		auto _iter = find_if(m_RootGameObjects.begin(), m_RootGameObjects.end(), [&objectName](auto& go)
			{
				return go->GetName() == objectName;
			});

		if (_iter != m_RootGameObjects.end())
		{
			return *_iter;
		}

		return nullptr;
	}
}