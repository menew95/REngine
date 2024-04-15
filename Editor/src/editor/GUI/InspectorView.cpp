#include <Editor_pch.h>

#include <editor\Core\EventManager.h>
#include <editor\Core\AssetManager.h>

#include <editor\GUI\InspectorView.h>
#include <editor\GUI\SearchView.h>

#include <editor\Widget\WidgetManager.h>

#include <rengine\core\object\GameObject.h>
#include <rengine\core\component\Component.h>

#include <rengine\core\resource\Material.h>
#include <rengine\core\resource\Texture.h>

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

        if (EventManager::GetInstance()->GetFocusObject() == nullptr)
			return;
        
		rengine::Object* _object = reinterpret_cast<rengine::Object*>(EventManager::GetInstance()->GetFocusObject());

		if(_object->GetType() == TEXT("GameObject"))
		{
			// 오브젝트가 게임 오브젝트일 경우 게임오브젝트 인스펙터창을 렌더
            auto* _go = reinterpret_cast<rengine::GameObject*>(EventManager::GetInstance()->GetFocusObject());

			DrawGameObject(_go);

            for (auto& _comp : _go->GetComponents())
            {
                DrawComponent(_comp.lock().get());
            }

			DrawAddComponent();

			/*if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				EventManager::GetInstance()->SetFocusObject(nullptr);*/
        }
		else if (_object->GetType() == TEXT("Material"))
		{
			// 오브젝트가 머티리얼일 경우 머티리얼 프로퍼티 인스펙터창을 렌더

			DrawMaterialProperty();
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
				/*if (InputFloat2* _widget = reinterpret_cast<InputFloat2*>(header.GetChild(_propName)))
				{
					_widget->SetHandler(component);
				}
				else
				{
					uint32 _flags = ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;

					_widget = WidgetManager::GetInstance()->CreateWidget<InputFloat2>(_propName, component, prop, _flags);

					header.AddWidget(_widget);
				}*/

				if (DragFloat2* _widget = reinterpret_cast<DragFloat2*>(header.GetChild(_propName)))
				{
					_widget->SetHandler(component);
				}
				else
				{
					uint32 _flags = ImGuiSliderFlags_None;

					float _min = numeric_limits<float>::lowest(), _max = numeric_limits<float>::max();

					_widget = WidgetManager::GetInstance()->CreateWidget<DragFloat2>(_propName, component, prop, 1.0f, _min, _max, _flags);

					header.AddWidget(_widget);
				}

				break;
			}
			case rengine::MetaDataType::VECTOR3:
			{
				/*if (InputFloat3* _widget = reinterpret_cast<InputFloat3*>(header.GetChild(_propName)))
				{
					_widget->SetHandler(component);
				}
				else
				{
					uint32 _flags = ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;

					_widget = WidgetManager::GetInstance()->CreateWidget<InputFloat3>(_propName, component, prop, _flags);

					header.AddWidget(_widget);
				}*/

				if (DragFloat3* _widget = reinterpret_cast<DragFloat3*>(header.GetChild(_propName)))
				{
					_widget->SetHandler(component);
				}
				else
				{
					uint32 _flags = ImGuiSliderFlags_None;

					float _min = numeric_limits<float>::lowest(), _max = numeric_limits<float>::max();

					_widget = WidgetManager::GetInstance()->CreateWidget<DragFloat3>(_propName, component, prop, 1.0f, _min, _max, _flags);

					header.AddWidget(_widget);
				}

				break;
			}
			case rengine::MetaDataType::VECTOR4:
			{
				/*if (InputFloat4* _widget = reinterpret_cast<InputFloat4*>(header.GetChild(_propName)))
				{
					_widget->SetHandler(component);
				}
				else
				{
					uint32 _flags = ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;

					_widget = WidgetManager::GetInstance()->CreateWidget<InputFloat4>(_propName, component, prop, _flags);

					header.AddWidget(_widget);
				}*/

				if (DragFloat4* _widget = reinterpret_cast<DragFloat4*>(header.GetChild(_propName)))
				{
					_widget->SetHandler(component);
				}
				else
				{
					uint32 _flags = ImGuiSliderFlags_None;

					float _min = numeric_limits<float>::lowest(), _max = numeric_limits<float>::max();

					_widget = WidgetManager::GetInstance()->CreateWidget<DragFloat4>(_propName, component, prop, 1.0f, _min, _max, _flags);

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

				if (ObjectButton* _widget = reinterpret_cast<ObjectButton*>(header.GetChild(_propName)))
				{
					_widget->SetHandler(component);
				}
				else
				{
					_widget = WidgetManager::GetInstance()->CreateWidget<ObjectButton>(_propName, component, prop);

					header.AddWidget(_widget);
				}

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
				/*if (InputFloat* _widget = reinterpret_cast<InputFloat*>(header.GetChild(_propName)))
				{
					_widget->SetHandler(component);
				}
				else
				{
					uint32 _flags = ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;

					_widget = WidgetManager::GetInstance()->CreateWidget<InputFloat>(_propName, component, prop, _flags);

					header.AddWidget(_widget);
				}*/

				if (DragFloat* _widget = reinterpret_cast<DragFloat*>(header.GetChild(_propName)))
				{
					_widget->SetHandler(component);
				}
				else
				{
					uint32 _flags = ImGuiSliderFlags_None;

					float _min = numeric_limits<float>::lowest(), _max = numeric_limits<float>::max();

					_widget = WidgetManager::GetInstance()->CreateWidget<DragFloat>(_propName, component, prop, 1.0f, _min, _max, _flags);

					header.AddWidget(_widget);
				}
				break;
			}
			case rengine::MetaDataType::DOUBLE:
			{
				auto _d = var.to_double();
				break;
			}
			case rengine::MetaDataType::ENUM:
			{
				if (ComboBox* _widget = reinterpret_cast<ComboBox*>(header.GetChild(_propName)))
				{
					_widget->SetHandler(component);
				}
				else
				{
					uint32 _flags = ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;

					_widget = WidgetManager::GetInstance()->CreateWidget<ComboBox>(_propName, component, prop, _flags);

					header.AddWidget(_widget);
				}
				break;
			}
			case rengine::MetaDataType::Color:
			{
				if (ColorEdit* _widget = reinterpret_cast<ColorEdit*>(header.GetChild(_propName)))
				{
					_widget->SetHandler(component);
				}
				else
				{
					uint32 _flags = ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;

					_widget = WidgetManager::GetInstance()->CreateWidget<ColorEdit>(_propName, component, prop, _flags);

					header.AddWidget(_widget);
				}

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

		ImGui::Columns(4, "Test", false);

		{
			rttr::property _prop = gameobject_type.get_property("Active Self");

			CheckBox _widget{ "Active Self", go, _prop };

			_widget.Render();
		}

		ImGui::NextColumn();

		{
			rttr::property _prop = gameobject_type.get_property("Static");

			CheckBox _widget{ "Static", go, _prop };

			_widget.Render();
		}

		ImGui::Columns(2);
		
		{
			rttr::property _prop = gameobject_type.get_property("m_objectName");

			InputText _widget{"Name", go, _prop};

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

		auto* _componentWidget = WidgetManager::GetInstance()->GetCollapsWidget(comp->GetTypeStr()
		, ImGuiTreeNodeFlags_DefaultOpen
			| ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_SpanAvailWidth
			| ImGuiTreeNodeFlags_AllowOverlap
			| ImGuiTreeNodeFlags_FramePadding);

        for (rttr::property _prop : component_type.get_properties())
        {
            const rttr::variant _value = _prop.get_value(comp);

            rttr::variant _metaVariant = _prop.get_metadata(rengine::MetaData::Editor);

            if (!_metaVariant.is_valid())
                continue;

            rengine::MetaDataType _metaDataType = _metaVariant.get_value<rengine::MetaDataType>();

            //if (_value.is_sequential_container())
            //{
            //    auto _seq = _value.create_sequential_view();

            //    for (const auto& item : _seq)
            //    {
            //        auto _extractVar = item.extract_wrapped_value();
            //        //GetProperty(*_componentWidget, _metaDataType, comp, _value, _prop);
            //    }
            //}
            //else
            {
                GetProperty(*_componentWidget, _metaDataType, comp, _value, _prop);
            }
        }

		_componentWidget->Render();
    }

	void InspectorView::DrawAddComponent()
	{
		static char _buf[256];
		static vector<string> _componentList;

		int (*FindComponent)(ImGuiInputTextCallbackData*)
		 = [](ImGuiInputTextCallbackData* data)
		{
			string _componentName = data->Buf;

			_componentList.clear();

			for (auto& _comp : AssetManager::GetInstance()->GetComponentList())
			{
				if (_comp.find(_componentName) != string::npos)
				{
					_componentList.push_back(_comp);
				}
			}

			return 0;
		};

		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("Add Component");

			memset(_buf, 0, sizeof(_buf));
			_componentList = AssetManager::GetInstance()->GetComponentList();
		}

		if (ImGui::BeginPopup("Add Component"))
		{
			if (ImGui::InputTextEx("Add Component", "Component Name", _buf, IM_ARRAYSIZE(_buf), ImVec2(.0f, 0.f), ImGuiInputTextFlags_CallbackEdit, FindComponent))
			{
				int a= 0;
			}

			ImGui::PushID("Add Component");

			ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));

			for (auto _comp : _componentList)
			{
				if (ImGui::ButtonEx(_comp.c_str(), ImVec2(ImGui::GetWindowWidth() * 0.9f, 0.f), ImGuiButtonFlags_PressedOnDoubleClick))
				{
					auto* _go = reinterpret_cast<rengine::GameObject*>(EventManager::GetInstance()->GetFocusObject());
					
					_go->AddComponent(_comp);

					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::PopStyleVar(1);

			ImGui::PopID();

			ImGui::EndPopup();
		}
	}

	void InspectorView::DrawMaterialProperty()
	{
		auto* _material = reinterpret_cast<rengine::Material*>(EventManager::GetInstance()->GetFocusObject());

		for (auto& _pair : _material->GetProperties())
		{
			rengine::MaterialProperty::PropType _proptype = _pair.first;

			switch (_pair.first)
			{
				case rengine::MaterialProperty::PropType::Color:
				{
					for (auto& _prop : _pair.second)
					{
						/*ImVec4 _color = ImVec4(_prop.GetColor().x, _prop.GetColor().y, _prop.GetColor().z, _prop.GetColor().w);
						
						if (ImGui::ColorButton(_prop.GetNameStr().c_str(), _color))
						{
							_material->SetColor(_prop.GetName(), { _color.x, _color.y, _color.z, _color.w });
						}*/

						float _cor[4] = { _prop.GetColor().x, _prop.GetColor().y, _prop.GetColor().z, _prop.GetColor().w };

						if (ImGui::ColorEdit4(_prop.GetNameStr().c_str(), _cor))
						{
							_material->SetColor(_prop.GetName(), { _cor[0], _cor[1], _cor[2], _cor [3]} );
						}
					}
					break;
				}
				case rengine::MaterialProperty::PropType::Vector:
				{
					for (auto& _prop : _pair.second)
					{
						float _val[4] = { _prop.GetVector4().x, _prop.GetVector4().y, _prop.GetVector4().z, _prop.GetVector4().w };

						if (ImGui::InputFloat4(_prop.GetNameStr().c_str(), _val))
						{
							_material->SetVector4(_prop.GetName(), { _val[0], _val[1], _val[2], _val[3] });
						}
					}
					break;
				}
				case rengine::MaterialProperty::PropType::Float:
				{
					for (auto& _prop : _pair.second)
					{
						float _val = _prop.GetFloat();

						if (ImGui::InputFloat(_prop.GetNameStr().c_str(), &_val))
						{
							_material->SetFloat(_prop.GetName(), _val);
						}
					}
					break;
				}
				case rengine::MaterialProperty::PropType::Range:
				{
					for (auto& _prop : _pair.second)
					{
						float _val[2] = { _prop.GetRange().x, _prop.GetRange().y };

						if (ImGui::InputFloat4(_prop.GetNameStr().c_str(), _val))
						{
							assert(false);
							//_material->SetR(_prop.GetName(), { _val[0], _val[1], _val[2], _val[3] });
						}
					}
					break;
				}

				case rengine::MaterialProperty::PropType::Texture:
				{
					for (auto& _prop : _pair.second)
					{
						void* _textureID = _prop.GetTexture() != nullptr ? _prop.GetTexture()->GetTextureID() : nullptr;

						struct EventData
						{
							rengine::Material* _target;
							tstring _propName;
						};

						auto _event = [](void* userData, const shared_ptr<rengine::Object>& obj)
							{
								tstring* _propName = reinterpret_cast<tstring*>(userData);

								auto _texture = static_pointer_cast<rengine::Texture>(obj);

								auto* _material = reinterpret_cast<rengine::Material*>(EventManager::GetInstance()->GetFocusObject());

								_material->SetTexture(*_propName, _texture);
							};

						ImGui::Columns(2);
						ImGui::Text(_prop.GetNameStr().c_str());
						ImGui::NextColumn();

						if (ImGui::ImageButton(_prop.GetNameStr().c_str(), _textureID, ImVec2(20, 10)))
						{
							m_propName = _prop.GetName();

							SearchView::OpenSeachView("Texture", &m_propName, _event);
						}
						ImGui::EndColumns();
					}
					break;
				}
				case rengine::MaterialProperty::PropType::Int:
				{
					for (auto& _prop : _pair.second)
					{
						int _val = _prop.GetInt();

						if (ImGui::InputInt(_prop.GetNameStr().c_str(), &_val))
						{
							_material->SetInteger(_prop.GetName(), _val);
						}
					}
					break;
				}
				default:
					assert(false);
					break;
			}
		}
	}
}