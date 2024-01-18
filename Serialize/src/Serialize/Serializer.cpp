#include <Serialize_pch.h>
#include <serialize\Serializer.h>

#include <rttr\registration.h>
#include <rttr\type.h>

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


namespace utility
{
	Serializer::Serializer()
	{
	}

	Serializer::~Serializer()
	{
	}

	//bool Serializer::Serialize(rengine::Scene* scene)
	//{
	//	bool _hr = false;

	//	boost::property_tree::ptree _pt;

	//	/*_pt.add<string>("uuid", scene->GetUUIDStr());

	//	for (auto _go : scene->GetRootGameObjects())
	//	{
	//		boost::property_tree::ptree _go_pt;

	//		GameObjectSerializer _goSerializer;

	//		_goSerializer.Serialize(_go.get(), _pt);
	//	}*/

	//	std::ofstream file(StringHelper::WStringToString(scene->GetPath()));

	//	_hr = file.is_open();

	//	if (_hr)
	//	{
	//		boost::property_tree::write_json(file, _pt);

	//		file.close();
	//	}

	//	return _hr;
	//}

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

	bool Serializer::Serialize(const tstring& path, rengine::Object* object)
	{
		boost::property_tree::ptree _pt;

		rengine::MetaInfo _metaInfo = ReadMetaFile(path, _pt);

		switch (_metaInfo._type)
		{
			case rengine::SerializableType::SCENE:
			{
				SceneSerializer::Serialize(object, _pt);
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

		std::ofstream file(StringHelper::WStringToString(path));

		if (!file.is_open())
			return false;

		boost::property_tree::write_json(file, _pt);

		file.close();

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