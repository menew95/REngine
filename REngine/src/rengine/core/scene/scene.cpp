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

	}

	std::shared_ptr<GameObject> Scene::FindGameObject(tstring& objectName)
	{
		auto _iter = find_if(m_GameObjects.begin(), m_GameObjects.end(), [&objectName](auto& go)
			{
				return go->GetName() == objectName;
			});

		if (_iter != m_GameObjects.end())
		{
			return *_iter;
		}

		return nullptr;
	}
}