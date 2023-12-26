#include <Editor_pch.h>

#include <editor\Core\EventManager.h>

#include <editor/GUI/InspectorView.h>

#include <editor/Widget/WidgetManager.h>

#include <rengine\core\object\GameObject.h>
#include <rengine\core\component\Component.h>

#include <rttr\registration.h>
#include <rttr\type.h>
#include <rengine\core\object\MetaInfo.h>

#include <common\math.h>

namespace editor
{
    InspectorView::InspectorView()
    : View("Inspector View")
    {

    }

    InspectorView::~InspectorView()
    {
    }

    void InspectorView::Begin()
    {
        __super::Begin();

    }
    void InspectorView::Render()
    {
        __super::Render();


        if (ImGui::ButtonEx("Button1"))
        {

        }

        if (EventManager::GetInstance()->GetFocusObject() != nullptr)
        {
            auto* _go = reinterpret_cast<rengine::GameObject*>(EventManager::GetInstance()->GetFocusObject());

            string _str = _go->GetNameStr();

            Event<rengine::Object, void, string> _event(*_go, &rengine::GameObject::SetNameStr);
            InputText _inputText(_go->GetNameStr(), _str, 0, _event);

            _inputText.Render();

            for (auto& _comp : _go->GetComponents())
            {
                DrawComponent(_comp.get());
            }
        }
    }
    void InspectorView::End()
    {
        __super::End();

    }

	void GetProperty(CollapsingHeader& header, rengine::MetaDataType type, const rengine::Component* component
	, const rttr::variant& var, rttr::property& prop)
	{
		string _propName = prop.get_name().to_string();

		switch (type)
		{
		case rengine::MetaDataType::WSTRING:
		{
			auto _wstr = var.convert<tstring>();
			auto _str = StringHelper::WStringToString(_wstr);
			break;
		}
		case rengine::MetaDataType::VECTOR2:
		{
			auto _v = var.convert<math::Vector2>();
			break;
		}
		case rengine::MetaDataType::VECTOR3:
		{
			auto _v = var.convert<math::Vector3>();
			break;
		}
		case rengine::MetaDataType::VECTOR4:
		{
			auto _v = var.convert<math::Vector4>();
			break;
		}
		case rengine::MetaDataType::MATRIX:
		{
			auto _v = var.convert<math::Matrix>();

			math::Vector3 _pos, _rot, _scale;
			math::Quaternion _q;

			uint32 _flags = ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;

			_v.Decompose(_scale, _q, _pos);

			_rot = _q.ToEuler();

			float _pos_ptr[3] = { _pos.x, _pos.y, _pos.z };
			float _rot_ptr[3] = { _rot.x, _rot.y, _rot.z };
			float _scl_ptr[3] = { _scale.x, _scale.y, _scale.z };

			/*InputFloat3 _input[3] = {
				{string("Position"),	*_pos_ptr, _flags},
				{string("Rotation"),	*_rot_ptr, _flags},
				{string("Scale"),		*_scl_ptr, _flags}
			};*/

			if (InputFloat3* _pos_widget = reinterpret_cast<InputFloat3*>(header.GetChild("Position")))
			{

			}
			else
			{
				InputFloat3* _newWidget = WidgetManager::GetInstance()->CreateWidget<InputFloat3>("Position", _pos_ptr, 0);

				header.AddWidget(_newWidget);
			}

			if (InputFloat3* _pos_widget = reinterpret_cast<InputFloat3*>(header.GetChild("Rotation")))
			{

			}
			else
			{
				InputFloat3* _newWidget = WidgetManager::GetInstance()->CreateWidget<InputFloat3>("Rotation", _rot_ptr, 0);

				header.AddWidget(_newWidget);
			}

			if (InputFloat3* _pos_widget = reinterpret_cast<InputFloat3*>(header.GetChild("Scale")))
			{

			}
			else
			{
				InputFloat3* _newWidget = WidgetManager::GetInstance()->CreateWidget<InputFloat3>("Scale", _scl_ptr, 0);

				header.AddWidget(_newWidget);
			}

			break;
		}
		case rengine::MetaDataType::UUID:
		{
			auto _uuid = var.convert<uuid>();
			break;
		}
		case rengine::MetaDataType::BOOL:
		{
			auto _bool = var.to_bool();
			break;
		}
		case rengine::MetaDataType::UINT32:
		{
			auto _u32 = var.to_uint32();
		}
		case rengine::MetaDataType::INT32:
		{
			auto _i32 = var.to_int32();
			break;
		}
		case rengine::MetaDataType::FLOAT:
		{
			auto _f = var.to_float();
			break;
		}
		case rengine::MetaDataType::DOUBLE:
		{
			auto _d = var.to_double();
			break;
		}
		default:
		{
			assert(false);
			break;
		}
		}
	}

    void InspectorView::DrawComponent(rengine::Component* comp)
    {
        const rttr::type component_type = rttr::type::get_by_name(StringHelper::ToString(comp->GetName()));

		auto* _componentWidget = WidgetManager::GetInstance()->GetCollapsWidget(comp->GetNameStr());

        for (rttr::property _prop : component_type.get_properties())
        {
            const rttr::variant _value = _prop.get_value(comp);

            cout << _prop.get_name() << endl;

            rttr::variant _metaVariant = _prop.get_metadata(rengine::MetaData::Editor);

            if (!_metaVariant.is_valid())
                continue;

            rengine::MetaDataType _metaDataType = _metaVariant.get_value<rengine::MetaDataType>();

            if (_value.is_sequential_container())
            {
                auto _seq = _value.create_sequential_view();

                for (const auto& item : _seq) // iterates over all elements of the std::vector<T>
                {
                    auto _extractVar = item.extract_wrapped_value();
                    //GetProperty(_metaDataType, _comp, component_type, _extractVar, _prop, _item, true);
                }
            }
            else
            {
                GetProperty(*_componentWidget, _metaDataType, comp, _value, _prop);
            }
        }

		_componentWidget->Render();
    }


}