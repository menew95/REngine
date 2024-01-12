#include <Serialize_pch.h>
#include <serialize\ComponentSerializer.h>

namespace utility
{
	ComponentSerializer::ComponentSerializer(rengine::Component* object)
	: m_pObject(object)
	{

	}

	ComponentSerializer::~ComponentSerializer()
	{

	}

	void ComponentSerializer::Serialize(boost::property_tree::ptree& pt)
	{
		boost::property_tree::ptree _comp_pt;
		
		if(m_pObject == nullptr)
			return;

		string _type = StringHelper::ToString(m_pObject->GetType());

		/*_comp_pt.put("Type", _type);*/

		const rttr::type component_type = rttr::type::get_by_name(_type);

		for (rttr::property _prop : component_type.get_properties())
		{
			if (!_prop.is_valid())
				continue;

			GetProperty(_comp_pt, _prop, m_pObject);
		}

		string _uuid = StringHelper::WStringToString(m_pObject->GetUUID());

		pt.push_back(std::make_pair(_uuid, _comp_pt));
	}

	rengine::Component* ComponentSerializer::DeSerialize(tstring& path)
	{
		return nullptr;
	}
}