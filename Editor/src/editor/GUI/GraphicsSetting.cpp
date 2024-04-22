#include <Editor_pch.h>

#include <editor\GUI\GraphicsSetting.h>

#include <editor\Widget\WidgetManager.h>

#include <rengine\core\sceneManager.h>

#include <rengine\core\scene\scene.h>

namespace editor
{
	GraphicsSetting::GraphicsSetting()
		: View("Graphics Setting")
	{
		m_widget = WidgetManager::GetInstance()->CreateWidget<CollapsingHeader>("Graphics Settings"
			, ImGuiTreeNodeFlags_DefaultOpen
			| ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_SpanAvailWidth
			| ImGuiTreeNodeFlags_AllowOverlap
			| ImGuiTreeNodeFlags_FramePadding);

		auto* _columns = WidgetManager::GetInstance()->CreateWidget<Columns<1>>("Graphics Settings_Columes", 0);

		m_widget->AddWidget(_columns);
	}
	
	GraphicsSetting::~GraphicsSetting()
	{
	
	}
	
	void GraphicsSetting::Begin()
	{
		__super::Begin();
	}
	
	void GraphicsSetting::Render()
	{
		auto& _graphicsSetting = rengine::SceneManager::GetInstance()->GetCurrentScene()->GetGraphicsSetting();

		const rttr::instance& _obj = _graphicsSetting;

		const rttr::type _type = _obj.get_derived_type();
		std::string _name = _type.get_name().to_string();

		auto _props = _type.get_properties();

		DrawSettings(m_widget, _props, _obj);

		m_widget->Render();

		rengine::SceneManager::GetInstance()->GetCurrentScene()->SetGraphicsSetting(_graphicsSetting);
	}

	void GraphicsSetting::End()
	{
		__super::End();
	}

	bool IsEditor(rttr::property prop)
	{
		const rttr::variant metaData = prop.get_metadata(rengine::MetaData::Editor);

		return metaData.is_valid() ? metaData.get_value<bool>() : false;
	}

	void GraphicsSetting::DrawSettings(WidgetContainer* root, const rttr::array_range<rttr::property>& properties, const rttr::instance& obj)
	{
		for (auto& _prop : properties)
		{
			const rttr::variant _value = _prop.get_value(obj);

			rttr::variant _metaVariant = _prop.get_metadata(rengine::MetaData::Editor);

			if (!_metaVariant.is_valid())
				continue;

			rengine::MetaDataType _metaDataType = _metaVariant.get_value<rengine::MetaDataType>();

			string _propName = _prop.get_name().to_string();

			TextColored* _name_widget = reinterpret_cast<TextColored*>(root->GetChild(_propName));

			if (_metaDataType != rengine::MetaDataType::Structure && _name_widget == nullptr)
			{
				_name_widget = WidgetManager::GetInstance()->CreateWidget<TextColored>(_propName, math::Color{ 0.35f, 0.85f, 0.65f, 1.f });

				root->AddWidget(_name_widget);
			}

			_propName = "##" + _propName;

			switch (_metaDataType)
			{
				case rengine::MetaDataType::WSTRING:
				{
					InputText* _widget = reinterpret_cast<InputText*>(root->GetChild(_propName));

					if (_widget == nullptr)
					{
						uint32 _flags = ImGuiInputTextFlags_EnterReturnsTrue;

						_widget = WidgetManager::GetInstance()->CreateWidget<InputText>(_propName, "", _flags);

						root->AddWidget(_widget);
					}

					_widget->RegisterGetter([_prop, obj]()
						{
							rttr::variant _var = _prop.get_value(obj);

							return _var.get_value<tstring>();
						});

					_widget->RegisterSetter([_prop, obj](tstring& value)
						{
							assert(_prop.set_value(obj, value));

							rengine::SceneManager::GetInstance()->UpdateGraphicsSetting();
						});

					break;
				}
				case rengine::MetaDataType::VECTOR2:
				{
					DragScalar<float, 2>* _widget = reinterpret_cast<DragScalar<float, 2>*>(root->GetChild(_propName));

					if (_widget == nullptr)
					{
						uint32 _flags = ImGuiSliderFlags_None;

						float _min = numeric_limits<float>::lowest(), _max = numeric_limits<float>::max();

						_widget = WidgetManager::GetInstance()->CreateWidget<DragScalar<float, 2>>(_propName, _min, _max, 1.0f, "%.3f", _flags);

						root->AddWidget(_widget);
					}

					_widget->RegisterGetter([_prop, obj]()
						{
							rttr::variant _var = _prop.get_value(obj);

							return reinterpret_cast<array<float, 2>&>(_var.get_value<Vector2>());
						});

					_widget->RegisterSetter([_prop, obj](std::array<float, 2>& value)
						{
							assert(_prop.set_value(obj, reinterpret_cast<Vector2&>(value)));

							rengine::SceneManager::GetInstance()->UpdateGraphicsSetting();
						});
					break;
				}
				case rengine::MetaDataType::VECTOR3:
				{
					DragScalar<float, 3>* _widget = reinterpret_cast<DragScalar<float, 3>*>(root->GetChild(_propName));

					if (_widget == nullptr)
					{
						uint32 _flags = ImGuiSliderFlags_None;

						float _min = numeric_limits<float>::lowest(), _max = numeric_limits<float>::max();

						_widget = WidgetManager::GetInstance()->CreateWidget<DragScalar<float, 3>>(_propName, _min, _max, 1.0f, "%.3f", _flags);

						root->AddWidget(_widget);
					}

					_widget->RegisterGetter([_prop, obj]()
						{
							rttr::variant _var = _prop.get_value(obj);

							return reinterpret_cast<array<float, 3>&>(_var.get_value<Vector3>());
						});

					_widget->RegisterSetter([_prop, obj](std::array<float, 3>& value)
						{
							assert(_prop.set_value(obj, reinterpret_cast<Vector3&>(value)));

							rengine::SceneManager::GetInstance()->UpdateGraphicsSetting();
						});
					break;
				}
				case rengine::MetaDataType::VECTOR4:
				{
					DragScalar<float, 4>* _widget = reinterpret_cast<DragScalar<float, 4>*>(root->GetChild(_propName));

					if (_widget == nullptr)
					{
						uint32 _flags = ImGuiSliderFlags_None;

						float _min = numeric_limits<float>::lowest(), _max = numeric_limits<float>::max();

						_widget = WidgetManager::GetInstance()->CreateWidget<DragScalar<float, 4>>(_propName, _min, _max, 1.0f, "%.3f", _flags);

						root->AddWidget(_widget);
					}

					_widget->RegisterGetter([_prop, obj]()
						{
							rttr::variant _var = _prop.get_value(obj);

							return reinterpret_cast<array<float, 4>&>(_var.get_value<Vector4>());
						});

					_widget->RegisterSetter([_prop, obj](std::array<float, 4>& value)
						{
							assert(_prop.set_value(obj, reinterpret_cast<Vector4&>(value)));

							rengine::SceneManager::GetInstance()->UpdateGraphicsSetting();
						});
					break;
				}
				case rengine::MetaDataType::MATRIX:
				{

					break;
				}
				case rengine::MetaDataType::UUID:
				{

					break;
				}
				case rengine::MetaDataType::BOOL:
				{
					CheckBox* _widget = reinterpret_cast<CheckBox*>(root->GetChild(_propName));

					if (_widget == nullptr)
					{
						_widget = WidgetManager::GetInstance()->CreateWidget<CheckBox>(_propName);

						root->AddWidget(_widget);
					}

					_widget->RegisterGetter([_prop, obj]()
						{
							rttr::variant _var = _prop.get_value(obj);

							return _var.get_value<bool>();
						});

					_widget->RegisterSetter([_prop, obj](bool& value)
						{
							assert(_prop.set_value(obj, value));

							rengine::SceneManager::GetInstance()->UpdateGraphicsSetting();
						});
					break;
				}
				case rengine::MetaDataType::UINT32:
				{
					DragScalar<uint32, 1>* _widget = reinterpret_cast<DragScalar<uint32, 1>*>(root->GetChild(_propName));

					if (_widget == nullptr)
					{
						uint32 _flags = ImGuiSliderFlags_None;

						uint32 _min = numeric_limits<uint32>::lowest(), _max = numeric_limits<uint32>::max();

						_widget = WidgetManager::GetInstance()->CreateWidget<DragScalar<uint32, 1>>(_propName, _min, _max, 1.0f, "%u", _flags);

						root->AddWidget(_widget);
					}

					_widget->RegisterGetter([_prop, obj]()
						{
							rttr::variant _var = _prop.get_value(obj);

							return reinterpret_cast<array<uint32, 1>&>(_var.get_value<uint32>());
						});

					_widget->RegisterSetter([_prop, obj](std::array<uint32, 1>& value)
						{
							assert(_prop.set_value(obj, reinterpret_cast<uint32&>(value)));

							rengine::SceneManager::GetInstance()->UpdateGraphicsSetting();
						});
					break;
				}
				case rengine::MetaDataType::INT32:
				{
					DragScalar<int32, 1>* _widget = reinterpret_cast<DragScalar<int32, 1>*>(root->GetChild(_propName));

					if (_widget == nullptr)
					{
						uint32 _flags = ImGuiSliderFlags_None;

						int32 _min = numeric_limits<int32>::lowest(), _max = numeric_limits<int32>::max();

						_widget = WidgetManager::GetInstance()->CreateWidget<DragScalar<int32, 1>>(_propName, _min, _max, 1.0f, "%d", _flags);

						root->AddWidget(_widget);
					}

					_widget->RegisterGetter([_prop, obj]()
						{
							rttr::variant _var = _prop.get_value(obj);

							return reinterpret_cast<array<int32, 1>&>(_var.get_value<int32>());
						});

					_widget->RegisterSetter([_prop, obj](std::array<int32, 1>& value)
						{
							assert(_prop.set_value(obj, reinterpret_cast<int32&>(value)));

							rengine::SceneManager::GetInstance()->UpdateGraphicsSetting();
						});
					break;
				}
				case rengine::MetaDataType::FLOAT:
				{
					DragScalar<float, 1>* _widget = reinterpret_cast<DragScalar<float, 1>*>(root->GetChild(_propName));

					if (_widget == nullptr)
					{
						uint32 _flags = ImGuiSliderFlags_None;

						float _min = numeric_limits<float>::lowest(), _max = numeric_limits<float>::max();

						_widget = WidgetManager::GetInstance()->CreateWidget<DragScalar<float, 1>>(_propName, _min, _max, 1.0f, "%.3f", _flags);

						root->AddWidget(_widget);
					}

					_widget->RegisterGetter([_prop, obj]()
						{
							rttr::variant _var = _prop.get_value(obj);

							return reinterpret_cast<array<float, 1>&>(_var.get_value<float>());
						});

					_widget->RegisterSetter([_prop, obj](std::array<float, 1>& value)
						{
							assert(_prop.set_value(obj, reinterpret_cast<float&>(value)));

							rengine::SceneManager::GetInstance()->UpdateGraphicsSetting();
						});
					break;
				}
				case rengine::MetaDataType::DOUBLE:
				{
					DragScalar<double, 1>* _widget = reinterpret_cast<DragScalar<double, 1>*>(root->GetChild(_propName));

					if (_widget == nullptr)
					{
						uint32 _flags = ImGuiSliderFlags_None;

						double _min = numeric_limits<double>::lowest(), _max = numeric_limits<double>::max();

						_widget = WidgetManager::GetInstance()->CreateWidget<DragScalar<double, 1>>(_propName, _min, _max, 1.0f, "%.3f", _flags);

						root->AddWidget(_widget);
					}

					_widget->RegisterGetter([_prop, obj]()
						{
							rttr::variant _var = _prop.get_value(obj);

							return reinterpret_cast<array<double, 1>&>(_var.get_value<double>());
						});

					_widget->RegisterSetter([_prop, obj](std::array<double, 1>& value)
						{
							assert(_prop.set_value(obj, reinterpret_cast<double&>(value)));

							rengine::SceneManager::GetInstance()->UpdateGraphicsSetting();
						});
					break;
				}
				case rengine::MetaDataType::ENUM:
				{
					ComboBox* _widget = reinterpret_cast<ComboBox*>(root->GetChild(_propName));

					if (_widget == nullptr)
					{
						uint32 _flags = ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;

						_widget = WidgetManager::GetInstance()->CreateWidget<ComboBox>(_propName, _prop, _flags);

						root->AddWidget(_widget);
					}

					_widget->RegisterGetter([_prop, obj]()
						{
							rttr::variant _var = _prop.get_value(obj);

							return _var.get_value<int>();
						});

					_widget->RegisterSetter([_prop, obj](int& value)
						{
							assert(_prop.set_value(obj, (uint32)value));

							rengine::SceneManager::GetInstance()->UpdateGraphicsSetting();
						});
					break;
				}
				case rengine::MetaDataType::Color:
				{
					ColorEdit* _widget = reinterpret_cast<ColorEdit*>(root->GetChild(_propName));

					if (_widget == nullptr)
					{
						uint32 _flags = ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;

						_widget = WidgetManager::GetInstance()->CreateWidget<ColorEdit>(_propName, _flags);

						root->AddWidget(_widget);
					}

					_widget->RegisterGetter([_prop, obj, _widget]()
						{
							rttr::variant _var = _prop.get_value(obj);

							return _var.get_value<Color>();
						});

					_widget->RegisterSetter([_prop, obj, _widget](Color& value)
						{
							assert(_prop.set_value(obj, value));

							rengine::SceneManager::GetInstance()->UpdateGraphicsSetting();
						});

					break;
				}
				case rengine::MetaDataType::Structure:
				{
					DrawStruct(root, _prop, obj);

					break;
				}
				default:
					break;
			}
		}
	}

	void GraphicsSetting::DrawStruct(class WidgetContainer* root, const rttr::property& property, const rttr::instance& obj)
	{
		string _propName = property.get_name().to_string();

		CollapsingHeader* _widget = reinterpret_cast<CollapsingHeader*>(root->GetChild(_propName));

		if (_widget == nullptr)
		{
			uint32 _flags = ImGuiTreeNodeFlags_DefaultOpen
				| ImGuiTreeNodeFlags_Framed
				| ImGuiTreeNodeFlags_SpanAvailWidth
				| ImGuiTreeNodeFlags_AllowOverlap
				| ImGuiTreeNodeFlags_FramePadding;

			_widget = WidgetManager::GetInstance()->CreateWidget<CollapsingHeader>(_propName, _flags);

			root->AddWidget(_widget);
		}

		auto* _columns = _widget->GetChild(_propName + "_columes");

		if (_columns == nullptr)
		{
			auto _columns_meta = property.get_metadata("Columns Size");

			assert(_columns_meta.is_valid());

			auto _columns_size = _columns_meta.convert<size_t>();

			if(_columns_size == 1)
				_columns = WidgetManager::GetInstance()->CreateWidget<Columns<1>>(_propName + "_columes", 0);
			else
				_columns = WidgetManager::GetInstance()->CreateWidget<Columns<2>>(_propName + "_columes", 0);

			_widget->AddWidget(_columns);
		}

		auto _var = property.get_value(obj);
		auto _type = _var.get_type();
		auto _properties = _type.get_properties();

		rttr::instance _instance = _var;

		DrawSettings(reinterpret_cast<WidgetContainer*>(_columns), _properties, _instance);
	}
}