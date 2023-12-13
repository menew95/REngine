#include <Serialize_pch.h>
#include <Serialize\Serialization.h>

#include <rttr\registration.h>
#include <rttr\type.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <common\math.h>

#include <rengine\core\object\MetaInfo.h>

#include <rengine\core\scene\scene.h>
#include <rengine\core\object\GameObject.h>
#include <rengine/core/component/Component.h>

namespace utility
{
	template<>
	void serializeConfig(math::Vector2& v, std::string& valueName, boost::property_tree::ptree& pt)
	{
		boost::property_tree::ptree _vec;

		_vec.put("x", v.x);
		_vec.put("y", v.y);

		pt.push_back(std::make_pair(valueName, _vec));
	}

	template<>
	void serializeConfig(math::Vector3& v, std::string& valueName, boost::property_tree::ptree& pt)
	{
		boost::property_tree::ptree _vec;

		_vec.put("x", v.x);
		_vec.put("y", v.y);
		_vec.put("z", v.z);

		pt.push_back(std::make_pair(valueName, _vec));
	}

	template<>
	void serializeConfig(math::Vector4& v, std::string& valueName, boost::property_tree::ptree& pt)
	{
		boost::property_tree::ptree _vec;

		_vec.put("x", v.x);
		_vec.put("y", v.y);
		_vec.put("z", v.z);
		_vec.put("w", v.w);

		pt.push_back(std::make_pair(valueName, _vec));
	}


	template<>
	void serializeConfig(math::Matrix& m, std::string& valueName, boost::property_tree::ptree& pt)
	{
		boost::property_tree::ptree _matrix;

		_matrix.put("_11", m._11); _matrix.put("_12", m._12); _matrix.put("_13", m._13); _matrix.put("_14", m._14);
		_matrix.put("_21", m._21); _matrix.put("_22", m._22); _matrix.put("_23", m._23); _matrix.put("_24", m._24);
		_matrix.put("_31", m._31); _matrix.put("_32", m._32); _matrix.put("_33", m._33); _matrix.put("_34", m._34);
		_matrix.put("_41", m._41); _matrix.put("_42", m._42); _matrix.put("_43", m._43); _matrix.put("_44", m._44);

		pt.push_back(std::make_pair(valueName, _matrix));
	}

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

	void Serialize(std::shared_ptr<rengine::GameObject>& go, boost::property_tree::ptree& pt)
	{
		boost::property_tree::ptree _components_pt;

		for (auto& _comp : go->GetComponents())
		{
			boost::property_tree::ptree _comp_pt;

			const rttr::type component_type = rttr::type::get_by_name(StringHelper::ToString(_comp->GetName()));

			for (rttr::property _prop : component_type.get_properties())
			{
				const rttr::variant _value = _prop.get_value(_comp);

				rttr::variant _metaVariant = _prop.get_metadata(rengine::MetaData::Serializable);

				if (!_metaVariant.is_valid())
					continue;

				rengine::MetaDataType _metaDataType = _metaVariant.get_value<rengine::MetaDataType>();

				if (_value.is_sequential_container())
				{
					auto _seq = _value.create_sequential_view();
					boost::property_tree::ptree _array;

					for (const auto& item : _seq) // iterates over all elements of the std::vector<T>
					{
						boost::property_tree::ptree _item;

						auto _extractVar = item.extract_wrapped_value();
						GetProperty(_metaDataType, _comp, component_type, _extractVar, _prop, _item, true);

						_array.push_back(std::make_pair("", _item));
					}

					_comp_pt.push_back(std::make_pair(_prop.get_name().to_string(), _array));
				}
				else
				{
					GetProperty(_metaDataType, _comp, component_type, _value, _prop, _comp_pt);
				}
			}

			_components_pt.push_back(std::make_pair(StringHelper::ToString(_comp->GetName()), _comp_pt));
		}
	}

	bool Serialization::Serialize(rengine::Scene* scene)
	{
		bool _hr = false;

		boost::property_tree::ptree _pt;

		for (auto _go : scene->GetRootGameObjects())
		{
			boost::property_tree::ptree _go_pt;

			//Serialize(_go, _go_pt);

			auto _uuid = StringHelper::WStringToString(_go->GetUUID());

			_pt.push_back(std::make_pair(_uuid, _go_pt));
		}

		std::ofstream file("Asset/Scene/" + StringHelper::WStringToString(scene->GetName()) + ".scene");

		_hr = file.is_open();

		if (_hr)
		{
			boost::property_tree::write_json(file, _pt);

			file.close();
		}

		return _hr;
	}

	/*bool Serialization::Serialize(rengine::GameObject* object, void* pt)
	{
		auto* _pt = reinterpret_cast<boost::property_tree::basic_ptree<std::string, std::string>*>(pt);

		return false;
	}*/

	rengine::Object* Serialization::DeSerialize(tstring& path)
	{
		return nullptr;
	}
}