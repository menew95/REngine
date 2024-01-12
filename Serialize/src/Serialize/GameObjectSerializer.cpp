#include <Serialize_pch.h>
#include <serialize\GameObjectSerializer.h>
#include <serialize\ObjectSerializer.h>
#include <serialize\ComponentSerializer.h>

#include <rengine\core\component\Transform.h>

namespace utility
{
	GameObjectSerializer::GameObjectSerializer()
	{

	}

	GameObjectSerializer::~GameObjectSerializer()
	{

	}

	void GameObjectSerializer::Serialize(rengine::GameObject* object, boost::property_tree::ptree& pt)
	{
		if(object == nullptr)
			return;

		ObjectSerializer _goSerializer;

		_goSerializer.Serialize(object, pt);

		for (auto& _item : object->GetComponents())
		{
			auto _comp = _item.lock();

			_goSerializer.Serialize(_comp.get(), pt);

			/*ComponentSerializer _compSerializer(_comp.get());

			_compSerializer.Serialize(pt);*/
		}

		for (auto& _child : object->GetTransform()->GetChilds())
		{
			/*_goSerializer.*/Serialize(_child.lock()->GetGameObject().get(), pt);
		}
	}

	rengine::GameObject* GameObjectSerializer::DeSerialize(tstring& path)
	{
		return nullptr;
	}
}
