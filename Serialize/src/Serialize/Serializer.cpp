#include <Serialize_pch.h>
#include <serialize\Serializer.h>

#include <rttr\registration.h>
#include <rttr\type.h>

#include <filesystem>

#include <serialize\SerializerHelper.h>
#include <serialize\GameObjectSerializer.h>
#include <serialize\ObjectSerializer.h>
#include <serialize\SceneSerializer.h>
#include <serialize\ResourceSerializer.h>

#include <common\math.h>

#include <rengine\core\object\MetaInfo.h>

#include <rengine\core\scene\scene.h>
#include <rengine\core\object\GameObject.h>
#include <rengine\core\component\Component.h>

namespace fs = std::filesystem;

namespace utility
{
	Serializer::Serializer()
	{
	}

	Serializer::~Serializer()
	{
	}

	rengine::MetaInfo ReadMetaFile(const tstring& path, boost::property_tree::ptree& pt)
	{
		// path.meta
		tstring _metaPath = path + TEXT(".meta");

		std::ifstream file(_metaPath);

		if (!file.good())
			return rengine::MetaInfo();

		try
		{
			boost::property_tree::read_json(file, pt);

			file.close();
		}
		catch (const boost::property_tree::json_parser_error& e)
		{
			auto error = e.what();

			return rengine::MetaInfo();
		}

		rengine::MetaInfo _metaInfo;
		_metaInfo._guid = StringHelper::StringToWString(pt.get<string>("guid"));
		_metaInfo._type = static_cast<rengine::SerializableType>(pt.get<uint32>("serializable type"));

		return _metaInfo;
	}

	SERIALIZE_API bool Serializer::CreateMetaInfo(const tstring& path, rengine::Object* object)
	{
		boost::property_tree::ptree _pt;

		fs::path _path(path);

		_pt.put("guid", object->GetUUIDStr());

		if (!_path.has_extension())
			return false;

		std::string _extension = _path.extension().string();

		std::transform(_extension.begin(), _extension.end(), _extension.begin(), ::tolower);

		if (_extension == ".scene")
		{
			_pt.put("serializable type", (uint32)rengine::SerializableType::SCENE);
		}
		else if (_extension == ".prefab")
		{
			_pt.put("serializable type", (uint32)rengine::SerializableType::PREFABS);
		}
		else
		{
			_pt.put("serializable type", (uint32)rengine::SerializableType::RESOURCE);
			
			if (_extension == ".fbx")
			{
				
			}
			else if (_extension == ".mat")
			{

			}
			else if (_extension == ".mesh")
			{

			}
			else if (_extension == ".anim")
			{

			}
			else if (_extension == ".png" || _extension == ".bmp" || _extension == ".jpeg" || _extension == ".jpg"
				|| _extension == ".dds" || _extension == ".tga" || _extension == ".hdr")
			{
				ResourceSerializer::Serialize(object, _pt);
			}
		}

		std::ofstream _file(StringHelper::WStringToString(path + TEXT(".meta")));

		if (!_file.is_open())
			return false;

		boost::property_tree::write_json(_file, _pt);

		_file.close();

		return true;
	}

	//SERIALIZE_API bool Serializer::CreateMetaInfoModel(const tstring& path, rengine::Model* model)
	//{
	//	// 마땅히 방법이 없어서 일단 만든다.
	//	boost::property_tree::ptree _pt;

	//	fs::path _path(path);

	//	_pt.put("guid", "");

	//	_pt.put("serializable type", (uint32)rengine::SerializableType::RESOURCE);

	//	boost::property_tree::ptree _node_pt;
	//	boost::property_tree::ptree _mesh_pt;
	//	boost::property_tree::ptree _material_pt;
	//	boost::property_tree::ptree _anim_pt;

	//	for (auto* _obj : assets)
	//	{
	//		boost::property_tree::ptree _child;

	//		_child.put("", _obj->GetUUIDStr());

	//		if (_obj->GetType() == TEXT("Mesh")) _mesh_pt.push_back(std::make_pair("", _child));
	//		else if (_obj->GetType() == TEXT("Material")) _material_pt.push_back(std::make_pair("", _child));
	//		else if (_obj->GetType() == TEXT("AnimationClip")) _anim_pt.push_back(std::make_pair("", _child));
	//	}

	//	for (auto& _node : model._nodes)
	//	{
	//		boost::property_tree::ptree _child;

	//		//_child.put("name", _node._name);

	//		string _world = "world";

	//		serializeConfig<Matrix>(_node._world, _world, _child);

	//		_child.put("hasParent", _node._hasParent);
	//		_child.put("parent", _node._parent);

	//		_child.put("hasMesh", _node._hasMesh);
	//		_child.put("isSkinned", _node._isBone);
	//		_child.put("mesh", _node._mesh);

	//		_node_pt.push_back(std::make_pair(_node._name, _child));
	//	}

	//	_pt.add_child("node", _node_pt);
	//	_pt.add_child("mesh", _mesh_pt);
	//	_pt.add_child("material", _material_pt);
	//	_pt.add_child("animation", _anim_pt);

	//	std::ofstream _file(StringHelper::WStringToString(path + TEXT(".meta")));

	//	if (!_file.is_open())
	//		return false;

	//	boost::property_tree::write_json(_file, _pt);

	//	_file.close();

	//	return true;
	//}

	SERIALIZE_API rengine::MetaInfo Serializer::SerializeMetaInfo(const tstring& path)
	{
		boost::property_tree::ptree _pt;

		return ReadMetaFile(path, _pt);
	}

	bool Serializer::Serialize(const tstring& path, rengine::Object* object)
	{
		boost::property_tree::ptree _pt;

		rengine::MetaInfo _metaInfo = ReadMetaFile(path, _pt);

		switch (_metaInfo._type)
		{
			case rengine::SerializableType::SCENE:
			{
				SceneSerializer::Serialize(object, _pt);

				std::ofstream file(StringHelper::WStringToString(path));

				if (!file.is_open())
					return false;

				boost::property_tree::write_json(file, _pt);

				file.close();

				break;
			}
			case rengine::SerializableType::RESOURCE:
			{
				ResourceSerializer::Serialize(object, _pt);
				break;
			}
			case rengine::SerializableType::PREFABS:
			{
				break;
			}
			case rengine::SerializableType::UNKNOWN:
			default:
			{
				assert(false);
				break;
			}
		}

		return true;
	}

	shared_ptr<rengine::Object> Serializer::DeSerialize(tstring path)
	{
		boost::property_tree::ptree _meta_pt;

		rengine::MetaInfo _metaInfo = ReadMetaFile(path, _meta_pt);

		if(_metaInfo._guid.size() == 0)
			return nullptr;

		shared_ptr<rengine::Object> _object;

		switch (_metaInfo._type)
		{
			case rengine::SerializableType::SCENE:
			{
				_object = SceneSerializer::DeSerialize(path, _metaInfo);
				break;
			}
			case rengine::SerializableType::RESOURCE:
			{
				_object = ResourceSerializer::DeSerialize(path, _metaInfo, _meta_pt);
				break;
			}
			case rengine::SerializableType::PREFABS:
			{
				break;
			}
			case rengine::SerializableType::UNKNOWN:
			default:
			{
				assert(false);
				break;
			}
		}
		

		return _object;
	}
}