#include <Serialize_pch.h>
#include <serialize\SceneSerializer.h>
#include <serialize\ObjectSerializer.h>
#include <serialize\GameObjectSerializer.h>

#include <rengine\system\ObjectFactory.h>
#include <rengine\core\component\Component.h>
#include <rengine\core\scene\Scene.h>

namespace utility
{
	void SceneSerializer::Serialize(rengine::Object* object, boost::property_tree::ptree& pt)
	{
		pt.clear();

		if (object == nullptr)
			return;

		rengine::Scene* _scene = reinterpret_cast<rengine::Scene*>(object);

		for (auto _go : _scene->GetRootGameObjects())
		{
			GameObjectSerializer::Serialize(_go.get(), pt);
		}
	}

	std::shared_ptr<rengine::Object> SceneSerializer::DeSerialize(tstring& path, const rengine::MetaInfo& metaInfo)
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

		auto _scene = std::static_pointer_cast<rengine::Scene>(rengine::ObjectFactory::GetInstance()->CreateObject("Scene", metaInfo._guid));

		_scene->SetPath(path);

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
		}

		size_t _idx = 0;

		for (auto& _node : pt)
		{
			if (_node.first == "uuid")
				continue;

			auto _typeIter = _node.second.get_child("m_typeName");

			if (_typeIter.data() == "Transform")
			{
				auto _trans_comp = std::static_pointer_cast<rengine::Component>(_list[_idx]);

				auto _go = _trans_comp->GetGameObject().lock();

				// 딱히 루트게임 오브젝트 임을 알릴 필요가 있는가? 트랜스폼에서 부모를 등록할때 삭제가 될듯 싶은데 ...
				if(_node.second.get_child("m_parent").data() == "")
					_scene->AddRootGameObject(_go);

				_scene->AddGameObject(_go);
			}

			ObjectSerializer::DeSerialize(_node, _list[_idx++]);

		}

		return _scene;
	}
}