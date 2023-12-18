#include <rengine\core\object\GameObject.h>
#include <rengine\core\component\Transform.h>

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