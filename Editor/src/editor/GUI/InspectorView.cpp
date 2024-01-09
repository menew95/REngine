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

        if (EventManager::GetInstance()->GetFocusObject() != nullptr)
        {
            auto* _go = reinterpret_cast<rengine::GameObject*>(EventManager::GetInstance()->GetFocusObject());

			DrawGameObject(_go);

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

	void GetProperty(CollapsingHeader& header, rengine::MetaDataType type, rengine::Component* component
	, const rttr::variant& var, rttr::property& prop)
	{
		string _propName = prop.get_name().to_string();

		switch (type)
		{
			case rengine::MetaDataType::WSTRING:
			{
				auto _wstr = var.convert<tstring>();
				auto _str = StringHelper::WStringToString(_wstr);

				if (InputText* _widget = reinterpret_cast<InputText*>(header.GetChild(_propName)))
				{
					_widget->SetHandler(component);
				}
				else
				{
					uint32 _flags = ImGuiInputTextFlags_EnterReturnsTrue;

					_widget = WidgetManager::GetInstance()->CreateWidget<InputText>(_propName, component, prop, _flags);

					header.AddWidget(_widget);
				}

				break;
			}
			case rengine::MetaDataType::VECTOR2:
			{
				if (InputFloat2* _widget = reinterpret_cast<InputFloat2*>(header.GetChild(_propName)))
				{
					_widget->SetHandler(component);
				}
				else
				{
					uint32 _flags = ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;

					_widget = WidgetManager::GetInstance()->CreateWidget<InputFloat2>(_propName, component, prop, _flags);

					header.AddWidget(_widget);
				}

				break;
			}
			case rengine::MetaDataType::VECTOR3:
			{
				if (InputFloat3* _widget = reinterpret_cast<InputFloat3*>(header.GetChild(_propName)))
				{
					_widget->SetHandler(component);
				}
				else
				{
					uint32 _flags = ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;

					_widget = WidgetManager::GetInstance()->CreateWidget<InputFloat3>(_propName, component, prop, _flags);

					header.AddWidget(_widget);
				}

				break;
			}
			case rengine::MetaDataType::VECTOR4:
			{
				if (InputFloat4* _widget = reinterpret_cast<InputFloat4*>(header.GetChild(_propName)))
				{
					_widget->SetHandler(component);
				}
				else
				{
					uint32 _flags = ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;

					_widget = WidgetManager::GetInstance()->CreateWidget<InputFloat4>(_propName, component, prop, _flags);

					header.AddWidget(_widget);
				}

				break;
			}
			case rengine::MetaDataType::MATRIX:
			{
				auto _v = var.convert<math::Matrix>();

				uint32 _flags = ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;

				break;
			}
			case rengine::MetaDataType::UUID:
			{
				auto _uuid = var.convert<uuid>();
				break;
			}
			case rengine::MetaDataType::BOOL:
			{
				if (CheckBox* _widget = reinterpret_cast<CheckBox*>(header.GetChild(_propName)))
				{
					_widget->SetHandler(component);
				}
				else
				{
					_widget = WidgetManager::GetInstance()->CreateWidget<CheckBox>(_propName, component, prop);

					header.AddWidget(_widget);
				}
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
				if (InputFloat* _widget = reinterpret_cast<InputFloat*>(header.GetChild(_propName)))
				{
					_widget->SetHandler(component);
				}
				else
				{
					uint32 _flags = ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;

					_widget = WidgetManager::GetInstance()->CreateWidget<InputFloat>(_propName, component, prop, _flags);

					header.AddWidget(_widget);
				}
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

	void InspectorView::DrawGameObject(rengine::GameObject* go)
	{
		//Event<rengine::Object, void, string> _event(*_go, &rengine::GameObject::SetNameStr);
		//InputText _inputText(go->GetNameStr(), _str, 0, _event);

		//_inputText.Render();

		const rttr::type gameobject_type = rttr::type::get_by_name("GameObject");

		ImGui::Columns(3, "Test", false);

		{
			rttr::property _prop = gameobject_type.get_property("Active Self");

			CheckBox _widget{ "", go, _prop };

			_widget.Render();
		}

		ImGui::NextColumn();

		{
			rttr::property _prop = gameobject_type.get_property("m_objectName");

			InputText _widget{"Name", go, _prop};

			_widget.Render();
		}

		ImGui::NextColumn();

		{
			rttr::property _prop = gameobject_type.get_property("Static");

			CheckBox _widget{ "Static", go, _prop };

			_widget.Render();
		}

		ImGui::Columns(1);

		ImGui::SetNextItemWidth(ImGui::GetWindowSize().x / 2);

		{
			rttr::property _prop = gameobject_type.get_property("Tag");

			if (ImGui::BeginCombo("Tag", "Tag Test", ImGuiComboFlags_PopupAlignLeft))
			{

				ImGui::EndCombo();
			}
		}

		ImGui::SameLine();

		ImGui::SetNextItemWidth(ImGui::GetWindowSize().x / 2);

		{
			rttr::property _prop = gameobject_type.get_property("Layer");

			if (ImGui::BeginCombo("Layer", "Layer Test", ImGuiComboFlags_PopupAlignLeft))
			{

				ImGui::EndCombo();
			}
		}
	}

	void InspectorView::DrawComponent(rengine::Component* comp)
    {
        const rttr::type component_type = rttr::type::get_by_name(StringHelper::ToString(comp->GetTypeStr()));

		auto* _componentWidget = WidgetManager::GetInstance()->GetCollapsWidget(comp->GetTypeStr());

        for (rttr::property _prop : component_type.get_properties())
        {
            const rttr::variant _value = _prop.get_value(comp);

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