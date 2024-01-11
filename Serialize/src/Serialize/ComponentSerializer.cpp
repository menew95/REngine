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

	void GetProperty(rengine::MetaDataType type, rengine::Component& component
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