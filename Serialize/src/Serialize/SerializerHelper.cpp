#include <Serialize_pch.h>
#include <serialize\SerializerHelper.h>

#include <rengine\system\ObjectFactory.h>

#include <rttr\rttr_cast.h>

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
	void serializeConfig(math::Color& v, std::string& valueName, boost::property_tree::ptree& pt)
	{
		boost::property_tree::ptree _vec;

		_vec.put("r", v.x);
		_vec.put("g", v.y);
		_vec.put("b", v.z);
		_vec.put("a", v.w);

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

	template<>
	math::Vector2 parseConfig(boost::property_tree::ptree& pt)
	{
		math::Vector2 _data;

		_data.x = pt.get<float>("x");
		_data.y = pt.get<float>("y");

		return _data;
	}

	template<>
	math::Vector3 parseConfig(boost::property_tree::ptree& pt)
	{
		math::Vector3 _data;

		_data.x = pt.get<float>("x");
		_data.y = pt.get<float>("y");
		_data.z = pt.get<float>("z");

		return _data;
	}

	template<>
	math::Vector4 parseConfig(boost::property_tree::ptree& pt)
	{
		math::Vector4 _data;

		_data.x = pt.get<float>("x");
		_data.y = pt.get<float>("y");
		_data.z = pt.get<float>("z");
		_data.w = pt.get<float>("w");

		return _data;
	}

	template<>
	math::Color parseConfig(boost::property_tree::ptree& pt)
	{
		math::Color _data;

		_data.x = pt.get<float>("r");
		_data.y = pt.get<float>("g");
		_data.z = pt.get<float>("b");
		_data.w = pt.get<float>("a");

		return _data;
	}

	template<>
	math::Matrix parseConfig(boost::property_tree::ptree& pt)
	{
		math::Matrix _data;

		_data._11 = pt.get<float>("_11"); _data._12 = pt.get<float>("_12"); _data._13 = pt.get<float>("_13");  _data._14 = pt.get<float>("_14");
		_data._21 = pt.get<float>("_21"); _data._22 = pt.get<float>("_22"); _data._23 = pt.get<float>("_23");  _data._24 = pt.get<float>("_24");
		_data._31 = pt.get<float>("_31"); _data._32 = pt.get<float>("_32"); _data._33 = pt.get<float>("_33");  _data._34 = pt.get<float>("_34");
		_data._41 = pt.get<float>("_41"); _data._42 = pt.get<float>("_42"); _data._43 = pt.get<float>("_43");  _data._44 = pt.get<float>("_44");

		return _data;
	}

	void GetPropertyArray(boost::property_tree::ptree& pt, const rttr::variant& value, string name, rengine::MetaDataType metaDataType)
	{
		boost::property_tree::ptree _array_pt;

		string _emptyname = "";

		switch (metaDataType)
		{
			case rengine::MetaDataType::WSTRING:
			{
				for (auto& val : value.create_sequential_view())
				{
					if (!val.can_convert<tstring>())
						continue;

					boost::property_tree::ptree _child;

					auto _v = val.convert<tstring>();

					_child.put("", StringHelper::WStringToString(_v));

					_array_pt.push_back(std::make_pair("", _child));
				}

				pt.add_child(name, _array_pt);

				break;
			}
			case rengine::MetaDataType::VECTOR2:
			{
				for (auto& val : value.create_sequential_view())
				{
					if (!val.can_convert<math::Vector2>())
						continue;

					boost::property_tree::ptree _child;

					auto _v = val.convert<math::Vector2>();

					serializeConfig(_v, _emptyname, _child);

					_array_pt.push_back(std::make_pair("", _child));
				}

				pt.add_child(name, _array_pt);

				break;
			}
			case rengine::MetaDataType::VECTOR3:
			{
				for (auto& val : value.create_sequential_view())
				{
					if (!val.can_convert<math::Vector3>())
						continue;

					boost::property_tree::ptree _child;

					auto _v = val.convert<math::Vector3>();

					serializeConfig(_v, _emptyname, _child);

					_array_pt.push_back(std::make_pair("", _child));
				}

				pt.add_child(name, _array_pt);

				break;
			}
			case rengine::MetaDataType::VECTOR4:
			{
				for (auto& val : value.create_sequential_view())
				{
					if (!val.can_convert<math::Vector4>())
						continue;

					boost::property_tree::ptree _child;

					auto _v = val.convert<math::Vector4>();

					serializeConfig(_v, _emptyname, _child);

					_array_pt.push_back(std::make_pair("", _child));
				}

				pt.add_child(name, _array_pt);

				break;
			}
			case rengine::MetaDataType::MATRIX:
			{
				for (auto& val : value.create_sequential_view())
				{
					if (!val.can_convert<math::Matrix>())
						continue;

					boost::property_tree::ptree _child;

					auto _v = val.convert<math::Matrix>();

					serializeConfig(_v, _emptyname, _child);

					_array_pt.push_back(std::make_pair("", _child));
				}

				pt.add_child(name, _array_pt);

				break;
			}
			case rengine::MetaDataType::UUID:
			{
				for (auto& val : value.create_sequential_view())
				{
					auto _ptr = val.get_wrapped_value<weak_ptr<rengine::Object>>();

					auto _shared_ptr = _ptr.lock();

					if(_shared_ptr == nullptr)
						continue;

					boost::property_tree::ptree _child;

					_child.put("", StringHelper::WStringToString(_shared_ptr->GetUUID()));

					_array_pt.push_back(std::make_pair("", _child));
				}

				pt.add_child(name, _array_pt);

				break;
			}
			case rengine::MetaDataType::BOOL:
			{
				for (auto& val : value.create_sequential_view())
				{
					if(val.can_convert<bool>())
						continue;

					boost::property_tree::ptree _child;

					_child.put("", val.to_bool());

					_array_pt.push_back(std::make_pair("", _child));
				}

				pt.add_child(name, _array_pt);

				break;
			}
			case rengine::MetaDataType::UINT32:
			{
				for (auto& val : value.create_sequential_view())
				{
					if (val.can_convert<uint32>())
						continue;

					boost::property_tree::ptree _child;

					_child.put("", val.to_uint32());

					_array_pt.push_back(std::make_pair("", _child));
				}

				pt.add_child(name, _array_pt);

				break;
			}
			case rengine::MetaDataType::INT32:
			{
				for (auto& val : value.create_sequential_view())
				{
					if (val.can_convert<int32>())
						continue;

					boost::property_tree::ptree _child;

					_child.put("", val.to_int32());

					_array_pt.push_back(std::make_pair("", _child));
				}

				pt.add_child(name, _array_pt);
			}
			case rengine::MetaDataType::FLOAT:
			{
				for (auto& val : value.create_sequential_view())
				{
					if (val.can_convert<float>())
						continue;

					boost::property_tree::ptree _child;

					_child.put("", val.to_float());

					_array_pt.push_back(std::make_pair("", _child));
				}

				pt.add_child(name, _array_pt);
			}
			case rengine::MetaDataType::DOUBLE:
			{
				for (auto& val : value.create_sequential_view())
				{
					if (val.can_convert<double>())
						continue;

					boost::property_tree::ptree _child;

					_child.put("", val.to_double());

					_array_pt.push_back(std::make_pair("", _child));
				}

				pt.add_child(name, _array_pt);
			}
			default:
			{
				assert(false);
				break;
			}
		}
	}

	void GetPropertySingle(boost::property_tree::ptree& pt, const rttr::variant& value, string name, rengine::MetaDataType metaDataType)
	{
		switch (metaDataType)
		{
			case rengine::MetaDataType::WSTRING:
			{
				auto _wstr = value.convert<tstring>();
				auto _str = StringHelper::WStringToString(_wstr);

				pt.put(name, _str);
				break;
			}
			case rengine::MetaDataType::VECTOR2:
			{
				auto _v = value.convert<math::Vector2>();

				serializeConfig(_v, name, pt);
				break;
			}
			case rengine::MetaDataType::VECTOR3:
			{
				auto _v = value.convert<math::Vector3>();

				serializeConfig(_v, name, pt);
				break;
			}
			case rengine::MetaDataType::VECTOR4:
			{
				auto _v = value.convert<math::Vector4>();

				serializeConfig(_v, name, pt);
				break;
			}
			case rengine::MetaDataType::MATRIX:
			{
				auto _v = value.convert<math::Matrix>();

				serializeConfig(_v, name, pt);
				break;
			}
			case rengine::MetaDataType::UUID:
			{
				auto _obj_type = value.get_type();
				
				auto _uuid_prop = _obj_type.get_wrapped_type().get_property("m_uuid");
				
				assert(_uuid_prop.is_valid());

				auto _uuid = _uuid_prop.get_value(value).convert<uuid>();

				auto _str_uuid = StringHelper::WStringToString(_uuid);

				pt.put(name, _str_uuid);

				break;
			}
			case rengine::MetaDataType::BOOL:
			{
				auto _bool = value.to_bool();

				pt.put(name, _bool);
				break;
			}
			case rengine::MetaDataType::UINT32:
			{
				auto _u32 = value.to_uint32();

				pt.put(name, _u32);
				break;
			}
			case rengine::MetaDataType::INT32:
			{
				auto _i32 = value.to_int32();

				pt.put(name, _i32);
				break;
			}
			case rengine::MetaDataType::FLOAT:
			{
				auto _f = value.to_float();

				pt.put(name, _f);
				break;
			}
			case rengine::MetaDataType::DOUBLE:
			{
				auto _d = value.to_double();

				pt.put(name, _d);
				break;
			}
			default:
			{
				assert(false);
				break;
			}
		}
	}

	void GetProperty(boost::property_tree::ptree& pt, rttr::property& property, rengine::Object* object)
	{
		rttr::variant _metaVariant = property.get_metadata(rengine::MetaData::Serializable);

		if(!_metaVariant.is_valid())
			return;

		if (!_metaVariant.can_convert<rengine::MetaDataType>())
			return;

		rengine::MetaDataType _metaDataType = _metaVariant.get_value<rengine::MetaDataType>();

		const rttr::variant _value = property.get_value(object);

		if(!_value.is_valid())
			return;

		string _propName = property.get_name().to_string();

		if (_value.is_sequential_container())
		{
			GetPropertyArray(pt, _value, _propName, _metaDataType);
		}
		else
		{
			GetPropertySingle(pt, _value, _propName, _metaDataType);
		}
	}

	void SetPropertyArray(boost::property_tree::ptree& pt, const rttr::property& prop, rttr::variant& object, rengine::MetaDataType metaDataType)
	{
		switch (metaDataType)
		{
			case rengine::MetaDataType::WSTRING:
			{
				vector<tstring> _data;
				
				for (auto& _item : pt)
				{
					auto _str = _item.second.get_value<string>();

					_data.push_back(StringHelper::StringToWString(_str));
				}

				assert(prop.set_value(object, _data));

				break;
			}
			case rengine::MetaDataType::VECTOR2:
			{
				vector<math::Vector2> _data;

				for (auto& _item : pt)
				{
					auto _val = parseConfig<math::Vector2>(_item.second);

					_data.push_back(_val);
				}

				assert(prop.set_value(object, _data));

				break;
			}
			case rengine::MetaDataType::VECTOR3:
			{
				vector<math::Vector3> _data;

				for (auto& _item : pt)
				{
					auto _val = parseConfig<math::Vector3>(_item.second);

					_data.push_back(_val);
				}

				assert(prop.set_value(object, _data));

				break;
			}
			case rengine::MetaDataType::VECTOR4:
			{
				vector<math::Vector4> _data;

				for (auto& _item : pt)
				{
					auto _val = parseConfig<math::Vector4>(_item.second);

					_data.push_back(_val);
				}

				assert(prop.set_value(object, _data));

				break;
			}
			case rengine::MetaDataType::MATRIX:
			{
				vector<math::Matrix> _data;

				for (auto& _item : pt)
				{
					auto _val = parseConfig<math::Matrix>(_item.second);

					_data.push_back(_val);
				}

				assert(prop.set_value(object, _data));

				break;
			}
			case rengine::MetaDataType::UUID:
			{
				vector<weak_ptr<rengine::Object>> _data;

				for (auto& _item : pt)
				{
					auto _str = _item.second.get_value<string>();

					std::weak_ptr _obj = rengine::ObjectFactory::GetInstance()->Find(StringHelper::StringToWString(_str));

					_data.push_back(_obj);
				}

				rttr::variant _var = _data;

				assert(_var.convert(prop.get_type()));

				assert(prop.set_value(object, _var));

				//{
				//	// 만약 전부 raw pointer로 변경시에
				//	vector<rengine::Object*> _objs{ _data[0].lock().get(), _data[1].lock().get() };

				//	rttr::variant _objs_var = _objs;
				//	rttr::variant _comps_var = prop.get_value(object);

				//	rttr::variant_sequential_view _objs_seq_var = _objs_var.create_sequential_view();
				//	rttr::variant_sequential_view _comps_seq_var = _comps_var.create_sequential_view();

				//	_comps_seq_var.set_size(_objs_seq_var.get_size());

				//	for (size_t i = 0; i < _objs_seq_var.get_size(); i++)
				//	{
				//		auto _conv_var = _objs_seq_var.get_value(i);

				//		_conv_var.convert(_comps_seq_var.get_value_type());

				//		_comps_seq_var.set_value(i, _conv_var);
				//	}

				//	rttr::type _objs_type = _objs_var.get_type();
				//	rttr::type _comps_type = _comps_var.get_type();

				//	assert(prop.set_value(object, _comps_var));
				//}

				break;
			}
			case rengine::MetaDataType::BOOL:
			{
				vector<bool> _data;

				for (auto& _item : pt)
				{
					auto _val = _item.second.get_value<bool>();

					_data.push_back(_val);
				}

				assert(prop.set_value(object, _data));

				break;
			}
			case rengine::MetaDataType::UINT32:
			{
				vector<uint32> _data;

				for (auto& _item : pt)
				{
					auto _val = _item.second.get_value<uint32>();

					_data.push_back(_val);
				}

				assert(prop.set_value(object, _data));

				break;
			}
			case rengine::MetaDataType::INT32:
			{
				vector<int32> _data;

				for (auto& _item : pt)
				{
					auto _val = _item.second.get_value<int32>();

					_data.push_back(_val);
				}

				assert(prop.set_value(object, _data));

				break;
			}
			case rengine::MetaDataType::FLOAT:
			{
				vector<float> _data;

				for (auto& _item : pt)
				{
					auto _val = _item.second.get_value<float>();

					_data.push_back(_val);
				}

				assert(prop.set_value(object, _data));

				break;
			}
			default:
			{
				assert(false);
				break;
			}
		}
	}

	void SetPropertySingle(boost::property_tree::ptree& pt, const rttr::property& prop, rttr::variant& object, rengine::MetaDataType metaDataType)
	{
		switch (metaDataType)
		{
			case rengine::MetaDataType::WSTRING:
			{
				auto _data = pt.get<std::string>("");
				auto _tstring = StringHelper::StringToWString(_data);

				assert(prop.set_value(object, _tstring));

				break;
			}
			case rengine::MetaDataType::VECTOR2:
			{
				math::Vector2 _data = parseConfig<math::Vector2>(pt);

				assert(prop.set_value(object, _data));

				break;
			}
			case rengine::MetaDataType::VECTOR3:
			{
				math::Vector3 _data = parseConfig<math::Vector3>(pt);

				assert(prop.set_value(object, _data));

				break;
			}
			case rengine::MetaDataType::VECTOR4:
			{
				math::Vector4 _data = parseConfig<math::Vector4>(pt);

				assert(prop.set_value(object, _data));

				break;
			}
			case rengine::MetaDataType::MATRIX:
			{
				math::Matrix _data = parseConfig<math::Matrix>(pt);

				assert(prop.set_value(object, _data));

				break;
			}
			case rengine::MetaDataType::UUID:
			{
				auto _data = pt.get<std::string>("");

				if(!_data.size())
					return;

				auto _prop_type_name = prop.get_type().get_name().to_string();

				rttr::variant _var;

				if (_prop_type_name.find("weak_ptr") != std::string::npos)
				{
					std::weak_ptr _obj = rengine::ObjectFactory::GetInstance()->Find(StringHelper::StringToWString(_data));

					_var = _obj;
				}
				else if(_prop_type_name.find("shared_ptr") != std::string::npos)
				{
					std::shared_ptr _obj = rengine::ObjectFactory::GetInstance()->Find(StringHelper::StringToWString(_data));

					_var = _obj;
				}

				assert(_var.convert(prop.get_type()));

				assert(prop.set_value(object, _var));

				break;
			}
			case rengine::MetaDataType::BOOL:
			{
				auto _data = pt.get<bool>("");

				assert(prop.set_value(object, _data));

				break;
			}
			case rengine::MetaDataType::UINT32:
			{
				auto _data = pt.get<uint32>("");

				assert(prop.set_value(object, _data));

				break;
			}
			case rengine::MetaDataType::INT32:
			{
				auto _data = pt.get<int32>("");

				assert(prop.set_value(object, _data));

				break;
			}
			case rengine::MetaDataType::FLOAT:
			{
				auto _data = pt.get<float>("");

				assert(prop.set_value(object, _data));

				break;
			}
			default:
				assert(false);
				break;
		}
	}

	void SetProperty(boost::property_tree::ptree& pt, rttr::property& prop, shared_ptr<rengine::Object> object)
	{
		rttr::variant _metaVariant = prop.get_metadata(rengine::MetaData::Serializable);

		if (!_metaVariant.is_valid())
			return;

		if (!_metaVariant.can_convert<rengine::MetaDataType>())
			return;

		rengine::MetaDataType _metaDataType = _metaVariant.get_value<rengine::MetaDataType>();

		rttr::variant _value(object.get());

		if (!_value.is_valid())
			return;

		string _propName = prop.get_name().to_string();

		if (prop.get_type().is_sequential_container())
		{
			SetPropertyArray(pt, prop, _value, _metaDataType);
		}
		else
		{
			SetPropertySingle(pt, prop, _value, _metaDataType);
		}
	}
}