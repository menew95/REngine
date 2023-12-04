#include <rengine\core\component\Component.h>

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

	Component::Component(std::shared_ptr<GameObject>& gameObj, uuid uuid, tstring name)
	: Object(uuid, name)
	, m_pGameObject(gameObj)
	{
	}

	Component::~Component()
	{
	}
}