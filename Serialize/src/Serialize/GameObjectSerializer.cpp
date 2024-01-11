#include <Serialize_pch.h>
#include <serialize\GameObjectSerializer.h>
#include <serialize\ComponentSerializer.h>

#include <rengine\core\component\Transform.h>

namespace utility
{
	GameObjectSerializer::GameObjectSerializer(rengine::GameObject* object)
	: m_pObject(object)
	{

	}

	GameObjectSerializer::~GameObjectSerializer()
	{

	}

	void GameObjectSerializer::Serialize(boost::property_tree::ptree& pt)
	{
		if(m_pObject == nullptr)
			return;

		boost::property_tree::ptree _go_pt;

		string _type = StringHelper::ToString(m_pObject->GetType());

		/*_go_pt.put("Type", _type);*/

		const rttr::type _go_type = rttr::type::get_by_name(_type);

		for (rttr::property _prop : _go_type.get_properties())
		{
			if(!_prop.is_valid())
				continue;

			GetProperty(_go_pt, _prop, m_pObject);
		}
		
		string _uuid = StringHelper::WStringToString(m_pObject->GetUUID());

		pt.push_back(std::make_pair(_uuid, _go_pt));

		for (auto& _item : m_pObject->GetComponents())
		{
			auto _comp = _item.lock();

			ComponentSerializer _compSerializer(_comp.get());

			_compSerializer.Serialize(pt);
		}

		for (auto& _child : m_pObject->GetTransform()->GetChilds())
		{
			GameObjectSerializer _childSerializer(_child.lock()->GetGameObject().get());

			_childSerializer.Serialize(pt);
		}
	}

	rengine::GameObject* GameObjectSerializer::DeSerialize(tstring& path)
	{
		return nullptr;
	}
}
