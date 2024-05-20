#include <Editor_pch.h>

#include <editor\Core\EventManager.h>
#include <editor\Core\AssetManager.h>

#include <editor\GUI\InspectorView.h>
#include <editor\GUI\SearchView.h>

#include <editor\Widget\WidgetManager.h>
#include <editor\Widget\Container.h>

#include <rengine\core\object\GameObject.h>
#include <rengine\core\component\Component.h>
#include <rengine\core\component\TestComponent.h>

#include <rengine\core\resource\Mesh.h>
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
		CreateInspectorWidget();

		EventManager::GetInstance()->m_unselectGameObjectEvent += [&]()
			{
				m_gamObjectWidget->SetEnable(false);
			};
    }

    InspectorView::~InspectorView()
    {
    }

    void InspectorView::Begin()
    {
        __super::Begin();

		m_gamObjectWidget->SetEnable(false);
	}
    
	void InspectorView::Render()
    {
        if (EventManager::GetInstance()->GetFocusObject() == nullptr)
			return;

		rengine::Object* _object = reinterpret_cast<rengine::Object*>(EventManager::GetInstance()->GetFocusObject());

		if(_object->GetType() == TEXT("GameObject"))
		{
			// 오브젝트가 게임 오브젝트일 경우 게임오브젝트 인스펙터창을 렌더
            auto* _go = reinterpret_cast<rengine::GameObject*>(EventManager::GetInstance()->GetFocusObject());

			m_gamObjectWidget->SetEnable(true);

			DrawGameObject(_go);

			__super::Render();

            for (auto& _comp : _go->GetComponents())
            {
                DrawComponent(_comp.lock().get());
            }

			DrawAddComponent();

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

	void InspectorView::CreateInspectorWidget()
	{
#pragma region GameObject Info Widget
		m_gamObjectWidget = AddWidget<Container>();

		auto* _columns = m_gamObjectWidget->AddWidget<Columns<2>>("GameObject Info");

		_columns->AddWidget<TextColored>("Active Self", math::Color{ 0.35f, 0.85f, 0.65f, 1.f });
		_columns->AddWidget<CheckBox>("##Active Self");

		_columns->AddWidget<TextColored>("Static", math::Color{ 0.35f, 0.85f, 0.65f, 1.f });
		_columns->AddWidget<CheckBox>("##Static");

		_columns->AddWidget<TextColored>("Name", math::Color{ 0.35f, 0.85f, 0.65f, 1.f });
		_columns->AddWidget<InputText>("##Name", "GameObject");

		/*_columns->AddWidget<TextColored>("Tag", math::Color{ 0.35f, 0.85f, 0.65f, 1.f });
		_columns->AddWidget<ComboBox>("##Tag", );

		_columns->AddWidget<TextColored>("Layer", math::Color{ 0.35f, 0.85f, 0.65f, 1.f });
		_columns->AddWidget<ComboBox>("##Layer", );*/
#pragma endregion

	}

	void GetProperty(WidgetContainer& root, WidgetContainer& container, rengine::MetaDataType type, rttr::property& prop, rttr::instance obj, rttr::variant val);

	void DrawStructure(WidgetContainer& root, const rttr::instance& obj, const rttr::property& property)
	{
		string _propName = property.get_name().to_string();

		CollapsingHeader* _widget = reinterpret_cast<CollapsingHeader*>(root.GetChild(_propName + "header"));

		if (_widget == nullptr)
		{
			uint32 _flags = ImGuiTreeNodeFlags_DefaultOpen
				| ImGuiTreeNodeFlags_Framed
				| ImGuiTreeNodeFlags_SpanAvailWidth
				| ImGuiTreeNodeFlags_AllowOverlap
				| ImGuiTreeNodeFlags_FramePadding;

			/*_widget = WidgetManager::GetInstance()->CreateWidget<CollapsingHeader>(_propName + "header", _flags);

			root.AddWidget(_widget);*/

			_widget = root.AddWidget<CollapsingHeader>(_propName + "header", _flags);
		}

		auto* _columns = reinterpret_cast<Columns<2>*>(_widget->GetChild(_propName + "_columes"));

		if (_columns == nullptr)
		{
			/*_columns = WidgetManager::GetInstance()->CreateWidget<Columns<2>>(_propName + "_columes");

			_widget->AddWidget(_columns);*/

			_columns = _widget->AddWidget<Columns<2>>(_propName + "_columes");
		}

		auto _var = property.get_value(obj);
		auto _type = _var.get_type();
		auto _properties = _type.get_properties();

		rttr::instance _instance = _var;

		for (rttr::property _prop : _properties)
		{
			const rttr::variant _value = _prop.get_value(_instance);

			rttr::variant _metaVariant = _prop.get_metadata(rengine::MetaData::Editor);

			if (!_metaVariant.is_valid())
				continue;

			rengine::MetaDataType _metaDataType = _metaVariant.get_value<rengine::MetaDataType>();

			GetProperty(*_widget , *_columns, _metaDataType, _prop, _instance, _var);
		}
	}

	void GetProperty(WidgetContainer& root, WidgetContainer& container, rengine::MetaDataType type, rttr::property& prop, rttr::instance obj, rttr::variant val)
	{
		string _propName = prop.get_name().to_string();

		TextColored* _name_widget = reinterpret_cast<TextColored*>(container.GetChild(_propName));

		if (type != rengine::MetaDataType::Structure && _name_widget == nullptr)
		{
			/*_name_widget = WidgetManager::GetInstance()->CreateWidget<TextColored>(_propName, math::Color{ 0.35f, 0.85f, 0.65f, 1.f });

			container.AddWidget(_name_widget);*/

			_name_widget = container.AddWidget<TextColored>(_propName, math::Color{ 0.35f, 0.85f, 0.65f, 1.f });
		}

		_propName = "##" + _propName;

		switch (type)
		{
			case rengine::MetaDataType::WSTRING:
			{
				InputText* _widget = reinterpret_cast<InputText*>(container.GetChild(_propName));

				if (_widget == nullptr)
				{
					uint32 _flags = ImGuiInputTextFlags_EnterReturnsTrue;

					/*_widget = WidgetManager::GetInstance()->CreateWidget<InputText>(_propName, "", _flags);

					container.AddWidget(_widget);*/

					_widget = container.AddWidget<InputText>(_propName, "", _flags);
				}

				_widget->RegisterGetter([prop, obj]()
					{
						rttr::variant _var = prop.get_value(obj);

						return _var.get_value<tstring>();
					});

				_widget->RegisterSetter([prop, obj](tstring& value)
					{
						assert(prop.set_value(obj, value));
					});

				break;
			}
			case rengine::MetaDataType::VECTOR2:
			{
				DragScalar<float, 2>* _widget = reinterpret_cast<DragScalar<float, 2>*>(container.GetChild(_propName));

				if (_widget == nullptr)
				{
					uint32 _flags = ImGuiSliderFlags_None;

					float _min = numeric_limits<float>::lowest(), _max = numeric_limits<float>::max();

					_widget = container.AddWidget<DragScalar<float, 2>>(_propName, _min, _max, 0.1f, "%.3f", _flags);
				}

				_widget->RegisterGetter([prop, obj]()
					{
						rttr::variant _var = prop.get_value(obj);

						return reinterpret_cast<array<float, 2>&>(_var.get_value<Vector2>());
					});

				_widget->RegisterSetter([prop, obj](std::array<float, 2>& value)
					{
						assert(prop.set_value(obj, reinterpret_cast<Vector2&>(value)));
					});

				break;
			}
			case rengine::MetaDataType::VECTOR3:
			{
				DragScalar<float, 3>*_widget = reinterpret_cast<DragScalar<float, 3>*>(container.GetChild(_propName));
				
				if(_widget == nullptr)
				{
					uint32 _flags = ImGuiSliderFlags_None;

					float _min = numeric_limits<float>::lowest(), _max = numeric_limits<float>::max();

					_widget = container.AddWidget<DragScalar<float, 3>>(_propName, _min, _max, 0.1f, "%.3f", _flags);
				}

				_widget->RegisterGetter([prop, obj]()
					{
						rttr::variant _var = prop.get_value(obj);

						return reinterpret_cast<array<float, 3>&>(_var.get_value<Vector3>());
					});

				_widget->RegisterSetter([prop, obj](std::array<float, 3>& value)
					{
						assert(prop.set_value(obj, reinterpret_cast<Vector3&>(value)));
					});

				break;
			}
			case rengine::MetaDataType::VECTOR4:
			{
				DragScalar<float, 4>* _widget = reinterpret_cast<DragScalar<float, 4>*>(container.GetChild(_propName));

				if (_widget == nullptr)
				{
					uint32 _flags = ImGuiSliderFlags_None;

					float _min = numeric_limits<float>::lowest(), _max = numeric_limits<float>::max();

					_widget = container.AddWidget<DragScalar<float, 4>>(_propName, _min, _max, 0.1f, "%.3f", _flags);
				}

				_widget->RegisterGetter([prop, obj]()
					{
						rttr::variant _var = prop.get_value(obj);

						return reinterpret_cast<array<float, 4>&>(_var.get_value<Vector4>());
					});

				_widget->RegisterSetter([prop, obj](std::array<float, 4>& value)
					{
						assert(prop.set_value(obj, reinterpret_cast<Vector4&>(value)));
					});

				break;
			}
			case rengine::MetaDataType::MATRIX:
			{

				break;
			}
			case rengine::MetaDataType::UUID:
			{
				ObjectButton* _widget = reinterpret_cast<ObjectButton*>(container.GetChild(_propName));

				if (_widget == nullptr)
				{
					_widget = container.AddWidget<ObjectButton>(_propName);
				}

				_widget->RegisterGetter([prop, obj]()
					{
						return prop.get_value(obj);
					});

				_widget->RegisterSetter([prop, obj](rttr::variant& value)
					{
						assert(value.convert(prop.get_type()));

						assert(prop.set_value(obj, value));
					});

				break;
			}
			case rengine::MetaDataType::BOOL:
			{
				CheckBox* _widget = reinterpret_cast<CheckBox*>(container.GetChild(_propName));

				if (_widget == nullptr)
				{
					_widget = container.AddWidget<CheckBox>(_propName);
				}

				_widget->RegisterGetter([prop, obj]()
					{
						rttr::variant _var = prop.get_value(obj);

						return _var.get_value<bool>();
					});

				_widget->RegisterSetter([prop, obj](bool& value)
					{
						assert(prop.set_value(obj, value));
					});

				break;
			}
			case rengine::MetaDataType::UINT32:
			{
				DragScalar<uint32, 1>* _widget = reinterpret_cast<DragScalar<uint32, 1>*>(container.GetChild(_propName));

				if (_widget == nullptr)
				{
					uint32 _flags = ImGuiSliderFlags_None;

					uint32 _min = numeric_limits<uint32>::lowest(), _max = numeric_limits<uint32>::max();

					_widget = container.AddWidget<DragScalar<uint32, 1>>(_propName, _min, _max, 1.0f, "%u", _flags);
				}

				_widget->RegisterGetter([prop, obj]()
					{
						rttr::variant _var = prop.get_value(obj);

						return reinterpret_cast<array<uint32, 1>&>(_var.get_value<uint32>());
					});

				_widget->RegisterSetter([prop, obj](std::array<uint32, 1>& value)
					{
						assert(prop.set_value(obj, reinterpret_cast<uint32&>(value)));
					});

				break;
			}
			case rengine::MetaDataType::INT32:
			{
				DragScalar<int32, 1>* _widget = reinterpret_cast<DragScalar<int32, 1>*>(container.GetChild(_propName));

				if (_widget == nullptr)
				{
					uint32 _flags = ImGuiSliderFlags_None;

					int32 _min = numeric_limits<int32>::lowest(), _max = numeric_limits<int32>::max();

					_widget = container.AddWidget<DragScalar<int32, 1>>(_propName, _min, _max, 1.0f, "%d", _flags);
				}

				_widget->RegisterGetter([prop, obj]()
					{
						rttr::variant _var = prop.get_value(obj);

						return reinterpret_cast<array<int32, 1>&>(_var.get_value<int32>());
					});

				_widget->RegisterSetter([prop, obj](std::array<int32, 1>& value)
					{
						assert(prop.set_value(obj, reinterpret_cast<int32&>(value)));
					});
				
				break;
			}
			case rengine::MetaDataType::FLOAT:
			{
				DragScalar<float, 1>*_widget = reinterpret_cast<DragScalar<float, 1>*>(container.GetChild(_propName));

				if(_widget == nullptr)
				{
					uint32 _flags = ImGuiSliderFlags_None;

					float _min = numeric_limits<float>::lowest(), _max = numeric_limits<float>::max();

					_widget = container.AddWidget<DragScalar<float, 1>>(_propName, _min, _max, 0.1f, "%.3f", _flags);
				}

				_widget->RegisterGetter([prop, obj]()
					{
						rttr::variant _var = prop.get_value(obj);

						return reinterpret_cast<array<float, 1>&>(_var.get_value<float>());
					});

				_widget->RegisterSetter([prop, obj](std::array<float, 1>& value)
					{
						assert(prop.set_value(obj, reinterpret_cast<float&>(value)));
					});

				break;
			}
			case rengine::MetaDataType::DOUBLE:
			{
				DragScalar<double, 1>* _widget = reinterpret_cast<DragScalar<double, 1>*>(container.GetChild(_propName));

				if (_widget == nullptr)
				{
					uint32 _flags = ImGuiSliderFlags_None;

					double _min = numeric_limits<double>::lowest(), _max = numeric_limits<double>::max();

					_widget = container.AddWidget<DragScalar<double, 1>>(_propName, _min, _max, 0.1f, "%.3f", _flags);
				}

				_widget->RegisterGetter([prop, obj]()
					{
						rttr::variant _var = prop.get_value(obj);

						return reinterpret_cast<array<double, 1>&>(_var.get_value<double>());
					});

				_widget->RegisterSetter([prop, obj](std::array<double, 1>& value)
					{
						assert(prop.set_value(obj, reinterpret_cast<double&>(value)));
					});

				break;
			}
			case rengine::MetaDataType::ENUM:
			{
				ComboBox* _widget = reinterpret_cast<ComboBox*>(container.GetChild(_propName));
				
				if(_widget == nullptr)
				{
					uint32 _flags = ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;

					/*_widget = WidgetManager::GetInstance()->CreateWidget<ComboBox>(_propName, prop, _flags);

					container.AddWidget(_widget);*/

					_widget = container.AddWidget<ComboBox>(_propName, _flags);

					/*
							enumeration enum_align = enum_type.get_enumeration();
							std::string name = enum_align.value_to_name(E_Alignment::AlignHCenter);
							std::cout << name; // prints "AlignHCenter"
							variant var = enum_align.name_to_value(name);
							E_Alignment value = var.get_value<E_Alignment>(); // stores value 'AlignHCenter'
						*/


						// char*를 캐싱 해두어야 하기때문에 string list를 미리 생성할 필요가 있다.
					rttr::type _enumType = rttr::type::get_by_name(prop.get_name());

					rttr::enumeration _enums = _enumType.get_enumeration();

					auto _names = _enums.get_names();

					vector<string> _items;

					for (auto& _name : _names)
					{
						_items.push_back(_name.to_string());
					}

					_widget->SetComboItems(_items);
				}

				_widget->RegisterGetter([prop, obj]()
					{
						rttr::variant _var = prop.get_value(obj);

						return _var.get_value<int>();
					});

				_widget->RegisterSetter([prop, obj](int& value)
					{
						assert(prop.set_value(obj, (uint32)value));
					});

				break;
			}
			case rengine::MetaDataType::Color:
			{
				ColorEdit* _widget = reinterpret_cast<ColorEdit*>(container.GetChild(_propName));
				
				if(_widget == nullptr)
				{
					uint32 _flags = ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;

					/*_widget = WidgetManager::GetInstance()->CreateWidget<ColorEdit>(_propName, _flags);

					container.AddWidget(_widget);*/

					_widget = container.AddWidget<ColorEdit>(_propName, _flags);
				}

				_widget->RegisterGetter([prop, obj, _widget]()
					{
						rttr::variant _var = prop.get_value(obj);

						return _var.get_value<Color>();
					});

				_widget->RegisterSetter([prop, obj, _widget](Color& value)
					{
						assert(prop.set_value(obj, value));
					});

				break;
			}
			case rengine::MetaDataType::Structure:
			{
				DrawStructure(root, obj, prop);

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
		const rttr::type gameobject_type = rttr::type::get_by_name("GameObject");

		rttr::instance _instance = go;

		auto* _columns = reinterpret_cast<Columns<2>*>(m_gamObjectWidget->GetChild("GameObject Info"));

		{
			rttr::property _prop = gameobject_type.get_property("Active Self");

			auto _widget = reinterpret_cast<CheckBox*>(_columns->GetChild("##Active Self"));

			_widget->RegisterGetter(
				std::bind(&rengine::GameObject::GetActiveSelf, go)
			);

			_widget->RegisterSetter(
				[_prop, _instance](auto& val)
				{
					_prop.set_value(_instance, val);
				}
			);
		}

		{
			rttr::property _prop = gameobject_type.get_property("Static");

			auto _widget = reinterpret_cast<CheckBox*>(_columns->GetChild("##Static"));

			_widget->RegisterGetter(
				std::bind(&rengine::GameObject::GetIsStatic, go)
			);
			_widget->RegisterSetter(
				[_prop, _instance](auto& val)
				{
					_prop.set_value(_instance, val);
				}
			);
		}
		
		{
			rttr::property _prop = gameobject_type.get_property("m_objectName");

			auto _widget = reinterpret_cast<InputText*>(_columns->GetChild("##Name"));

			_widget->RegisterGetter(
				std::bind(&rengine::GameObject::GetName, go)
			);
			_widget->RegisterSetter(
				[_prop, _instance](auto& val)
				{
					_prop.set_value(_instance, val);
				}
			);
		}

		ImGui::Columns(1);

		ImGui::SetNextItemWidth(ImGui::GetWindowSize().x / 2);

		{
			rttr::property _prop = gameobject_type.get_property("Tag");

			if (ImGui::BeginCombo("Tag", "Tag", ImGuiComboFlags_PopupAlignLeft))
			{

				ImGui::EndCombo();
			}
		}

		ImGui::SameLine();

		ImGui::SetNextItemWidth(ImGui::GetWindowSize().x / 2);

		{
			rttr::property _prop = gameobject_type.get_property("Layer");

			if (ImGui::BeginCombo("Layer", "Layer", ImGuiComboFlags_PopupAlignLeft))
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

		Popup* _popup = reinterpret_cast<Popup*>(_componentWidget->GetChild("Component Popup"));

		if (_popup == nullptr)
		{
			_popup = _componentWidget->AddWidget<Popup>("Component Popup");

			_popup->SetCheckFunction(
				[]()
				{
					return ImGui::IsItemClicked(1);
				}
			);

			_popup->AddWidget<MenuItem>("Reset Component", "", false, false, true, 0);
			_popup->AddWidget<MenuItem>("Delete Component", "", false, false, _componentWidget->GetLable() == "Transform" ? false : true, 0)->SetClickEvent(
				[&_componentWidget]()
				{
					auto* _go = reinterpret_cast<rengine::GameObject*>(EventManager::GetInstance()->GetFocusObject());

					for (auto& _comp : _go->GetComponents())
					{
						if (_comp.lock()->GetTypeStr() == _componentWidget->GetLable())
						{
							rengine::ObjectFactory::GetInstance()->ReserveDestroyObject(_comp.lock().get());
						}
					}
				}
			);
		}


		Columns<2>* _columns = reinterpret_cast<Columns<2>*>(_componentWidget->GetChild(comp->GetTypeStr() + "_Columes"));
		
		if (_columns == nullptr)
		{
			_columns = _componentWidget->AddWidget<Columns<2>>(comp->GetTypeStr() + "_Columes");
		}

        for (rttr::property _prop : component_type.get_properties())
        {
            const rttr::variant _value = _prop.get_value(comp);

            rttr::variant _metaVariant = _prop.get_metadata(rengine::MetaData::Editor);

            if (!_metaVariant.is_valid())
                continue;

            rengine::MetaDataType _metaDataType = _metaVariant.get_value<rengine::MetaDataType>();

            {
				rttr::instance _instance = comp;

                GetProperty(*_componentWidget, *_columns, _metaDataType, _prop, _instance, _value);
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

						float _min = 0.0f, _max = 1.0f;

						if (ImGui::DragScalar(_prop.GetNameStr().c_str(), ImGuiDataType_Float, &_val, 0.1f, &_min, &_max, "%.3f"))
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

						ImGui::Columns(2);
						ImGui::Text(_prop.GetNameStr().c_str());
						ImGui::NextColumn();

						if (ImGui::ImageButton(_prop.GetNameStr().c_str(), _textureID, ImVec2(20, 10)))
						{
							m_propName = _prop.GetName();

							auto _itemClickEvent = [&](tstring propName, const shared_ptr<rengine::Object>& obj)
								{
									auto _texture = static_pointer_cast<rengine::Texture>(obj);

									auto* _material = reinterpret_cast<rengine::Material*>(EventManager::GetInstance()->GetFocusObject());

									_material->SetTexture(propName, _texture);
								};


							SearchView::OpenSeachView("Texture"
								, std::bind(_itemClickEvent, m_propName, std::placeholders::_1)
								, std::bind(&rengine::ObjectFactory::FindObjectsOfType, rengine::ObjectFactory::GetInstance(), TEXT("Texture"))
							);
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

		if (_material->IsDirty())
		{
			AssetManager::GetInstance()->SaveAsset(_material);
		}
	}
		
	void InspectorView::SetGameObject(rengine::GameObject* go)
	{

	}
}