#include <Serialize_pch.h>
#include <serialize\SceneSerializer.h>
#include <serialize\ObjectSerializer.h>

#include <rengine\system\ObjectFactory.h>

namespace utility
{
	void SceneSerializer::Serialize(rengine::Object* object, boost::property_tree::ptree& pt)
	{

	}

	std::shared_ptr<rengine::Object> SceneSerializer::DeSerialize(tstring& path)
	{
		std::ifstream file(path);

		if (!file.good())
			return nullptr;

		boost::property_tree::ptree pt;

		try
		{
			boost::property_tree::read_json(file, pt);

			file.close();
		}
		catch (const boost::property_tree::json_parser_error& e)
		{
			auto error = e.what();

			return nullptr;
		}

		uuid _uuid = StringHelper::StringToWString(pt.get<string>("uuid"));

		auto _scene = rengine::ObjectFactory::GetInstance()->CreateObject("Scene", _uuid);

		assert(_scene != nullptr);

		vector<shared_ptr<rengine::Object>> _list;

		for (auto& _node : pt)
		{
			if(_node.first == "uuid")
				continue;

			auto _typeIter = _node.second.get_child("m_typeName");

			rttr::type _objType = rttr::type::get_by_name(_typeIter.data());

			uuid _uuid = StringHelper::StringToWString(_node.first);

			_list.emplace_back(rengine::ObjectFactory::GetInstance()->CreateObject(_typeIter.data(), _uuid));

			/*ObjectSerializer _objectSerializer;

			_objectSerializer.DeSerialize(_node);*/
		}

		size_t _idx = 0;

		for (auto& _node : pt)
		{
			if (_node.first == "uuid")
				continue;

			ObjectSerializer _objectSerializer;

			_objectSerializer.DeSerialize(_node, _list[_idx++]);
		}

		return _scene;
	}
}