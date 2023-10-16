#include <rengine\core\object\GameObject.h>

namespace rengine
{
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