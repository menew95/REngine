#include <Serialize_pch.h>
#include <serialize\Serializer.h>

#include <rttr\registration.h>
#include <rttr\type.h>

#include <serialize\SerializerHelper.h>
#include <serialize\GameObjectSerializer.h>
#include <serialize\ObjectSerializer.h>
#include <serialize\SceneSerializer.h>

#include <common\math.h>

#include <rengine\core\object\MetaInfo.h>

#include <rengine\core\scene\scene.h>
#include <rengine\core\object\GameObject.h>
#include <rengine\core\component\Component.h>


namespace utility
{
	/*template<>
	bool Serialized(rengine::GameObject* object, boost::property_tree::basic_ptree<std::string, std::string>& pt)
	{
		bool _hr = false;


		return _hr;
	}*/


	void GetProperty(rengine::MetaDataType type, const std::shared_ptr<rengine::Component>& component
		, const rttr::type& component_type, const rttr::variant& var
		, rttr::property& prop, boost::property_tree::ptree& pt, bool isArray = false)
	{
		string _propName = prop.get_name().to_string();

		switch (type)
		{
			case rengine::MetaDataType::WSTRING:
			{
				auto _wstr = var.convert<tstring>();
				auto _str = StringHelper::WStringToString(_wstr);

				pt.put(_propName, _str);
				break;
			}
			case rengine::MetaDataType::VECTOR2:
			{
				auto _v = var.convert<math::Vector2>();

				serializeConfig(_v, _propName, pt);
				break;
			}
			case rengine::MetaDataType::VECTOR3:
			{
				auto _v = var.convert<math::Vector3>();

				serializeConfig(_v, _propName, pt);
				break;
			}
			case rengine::MetaDataType::VECTOR4:
			{
				auto _v = var.convert<math::Vector4>();

				serializeConfig(_v, _propName, pt);
				break;
			}
			case rengine::MetaDataType::MATRIX:
			{
				auto _v = var.convert<math::Matrix>();

				serializeConfig(_v, _propName, pt);
				break;
			}
			case rengine::MetaDataType::UUID:
			{
				break;
			}
			case rengine::MetaDataType::BOOL:
			{
				auto _bool = var.to_bool();

				pt.put(_propName, _bool);
				break;
			}
			case rengine::MetaDataType::UINT32:
			{
				auto _u32 = var.to_uint32();

				pt.put(_propName, _u32);
				break;
			}
			case rengine::MetaDataType::INT32:
			{
				auto _i32 = var.to_int32();

				pt.put(_propName, _i32);
				break;
			}
			case rengine::MetaDataType::FLOAT:
			{
				auto _f = var.to_float();

				pt.put(_propName, _f);
				break;
			}
			case rengine::MetaDataType::DOUBLE:
			{
				auto _d = var.to_double();

				pt.put(_propName, _d);
				break;
			}
			default:
			{
				assert(false);
				break;
			}
		}
	}

	Serializer::Serializer()
	{
	}

	Serializer::~Serializer()
	{
	}

	bool Serializer::Serialize(rengine::Scene* scene)
	{
		bool _hr = false;

		boost::property_tree::ptree _pt;

		_pt.add<string>("uuid", scene->GetUUIDStr());

		for (auto _go : scene->GetRootGameObjects())
		{
			boost::property_tree::ptree _go_pt;

			GameObjectSerializer _goSerializer;

			_goSerializer.Serialize(_go.get(), _pt);
		}

		std::ofstream file(StringHelper::WStringToString(scene->GetPath()));

		_hr = file.is_open();

		if (_hr)
		{
			boost::property_tree::write_json(file, _pt);

			file.close();
		}

		return _hr;
	}

	SERIALIZE_API bool Serializer::Serialize(rengine::Object* object)
	{
		return true;
	}

	/*bool Serialization::Serialize(rengine::GameObject* object, void* pt)
	{
		auto* _pt = reinterpret_cast<boost::property_tree::basic_ptree<std::string, std::string>*>(pt);

		return false;
	}*/

	shared_ptr<rengine::Object> Serializer::DeSerialize(tstring path)
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

		auto _object = SceneSerializer::DeSerialize(path);

		/*for (auto& _node : pt)
		{
			ObjectSerializer _objectSerializer;

			_objectSerializer.DeSerialize(_node);
		}*/

		return _object;
	}
}