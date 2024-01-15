#include <Serialize_pch.h>
#include <serialize\ObjectSerializer.h>

#include <rengine\system\ObjectFactory.h>

namespace utility
{
	ObjectSerializer::ObjectSerializer()
	{

	}

	ObjectSerializer::~ObjectSerializer()
	{

	}

	void ObjectSerializer::Serialize(rengine::Object* object, boost::property_tree::ptree& pt)
	{
		if (object == nullptr)
			return;

		boost::property_tree::ptree _obj_pt;

		string _type = StringHelper::ToString(object->GetType());

		const rttr::type _obj_type = rttr::type::get_by_name(_type);

		for (rttr::property _prop : _obj_type.get_properties())
		{
			if (!_prop.is_valid())
				continue;

			GetProperty(_obj_pt, _prop, object);
		}

		string _uuid = StringHelper::WStringToString(object->GetUUID());

		pt.push_back(std::make_pair(_uuid, _obj_pt));
	}

	rengine::Object* ObjectSerializer::DeSerialize(pair<const string, boost::property_tree::ptree>& node)
	{
		uuid _uuid = StringHelper::StringToWString(node.first);

		auto _typeIter = node.second.get_child("m_typeName");

		auto _object = rengine::ObjectFactory::GetInstance()->CreateObject(_typeIter.data(), _uuid);

		rttr::type _objType = rttr::type::get_by_name(_typeIter.data());

		for (auto& _property_node : node.second)
		{
			if(_property_node.first == "m_typeName")
				continue;

			rttr::property _prop = _objType.get_property(_property_node.first);

			if(!_prop.is_valid())
				continue;

			SetProperty(_property_node.second, _prop, _object);
		}

		return nullptr;
	}

	rengine::Object* ObjectSerializer::DeSerialize(tstring& path)
	{
		return nullptr;
	}
}