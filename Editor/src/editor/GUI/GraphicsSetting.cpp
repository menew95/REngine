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

	void GraphicsSetting::DrawSettings(WidgetContainer* root, const rttr::array_range<rttr::property>& properties, rttr::instance obj)
	{
		for (auto& _prop : properties)
		{
			const rttr::variant _value = _prop.get_value(obj);

			rttr::variant _metaVariant = _prop.get_metadata(rengine::MetaData::Editor);

			if (!_metaVariant.is_valid())
				continue;

			rengine::MetaDataType _metaDataType = _metaVariant.get_value<rengine::MetaDataType>();

			string _propName = _prop.get_name().to_string();

			switch (_metaDataType)
			{
				case rengine::MetaDataType::WSTRING:
				{

					break;
				}
				case rengine::MetaDataType::VECTOR2:
				{

					break;
				}
				case rengine::MetaDataType::VECTOR3:
				{

					break;
				}
				case rengine::MetaDataType::VECTOR4:
				{

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

					break;
				}
				case rengine::MetaDataType::UINT32:
				{

					break;
				}
				case rengine::MetaDataType::INT32:
				{

					break;
				}
				case rengine::MetaDataType::FLOAT:
				{
					TextColored * _name_widget = reinterpret_cast<TextColored*>(root->GetChild(_propName));

					if (_name_widget == nullptr)
					{
						_name_widget = WidgetManager::GetInstance()->CreateWidget<TextColored>(_propName, math::Color{ 0.35f, 0.85f, 0.65f, 1.f });

						root->AddWidget(_name_widget);
					}

					if (DragScalar<float, 1>* _widget = reinterpret_cast<DragScalar<float, 1>*>(root->GetChild("##" + _propName)))
					{
						//_widget->SetHandler(obj);
					}
					else
					{
						uint32 _flags = ImGuiSliderFlags_None;

						float _min = numeric_limits<float>::lowest(), _max = numeric_limits<float>::max();

						_widget = WidgetManager::GetInstance()->CreateWidget<DragScalar<float, 1>>("##" + _propName, obj, _prop, _min, _max, 1.0f, "%.3f", _flags);

						_widget->RegisterGetter([_prop, obj]()
							{
								rttr::variant _var = _prop.get_value(obj);

								return reinterpret_cast<array<float, 1>&>(_var.get_value<float>());
							});

						auto _setter = [_prop, obj](std::array<float, 1>& value)
							{
								assert(_prop.set_value(obj, reinterpret_cast<float&>(value)));
							};

						_widget->RegisterSetter(_setter);

						root->AddWidget(_widget);
					}
					break;
				}
				case rengine::MetaDataType::DOUBLE:
				{
					
					break;
				}
				case rengine::MetaDataType::ENUM:
				{

					break;
				}
				case rengine::MetaDataType::Color:
				{
				
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

	void GraphicsSetting::DrawStruct(class WidgetContainer* root, const rttr::property& property, rttr::instance obj)
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

		const rttr::instance _instance = _var;

		DrawSettings(reinterpret_cast<WidgetContainer*>(_columns), _properties, _instance);
	}
}