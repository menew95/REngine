#include <Editor_pch.h>
#include <editor\GUI\SearchView.h>
#include <editor\Widget\ObjectButton.h>

#include <editor\Core\AssetManager.h>
#include <rengine\core\component\Component.h>
#include <rengine\System\ObjectFactory.h>
namespace editor
{
	ObjectButton::ObjectButton(string widgetName,rengine::Object* handler, rttr::property& prop, math::Vector2 size, uint32 flags)
		: Button(widgetName, handler, prop, size, flags)
		, m_rectSize{ size.x, size.y }
		, m_pHandler(handler)
		, m_prop(prop)
	{
	}

	ObjectButton::~ObjectButton()
	{

	}

	string GetObjectName(rttr::variant& var)
	{
		string _objName;

		auto _varType = var.get_type();

		if (var.is_sequential_container())
		{
			auto _seq = var.create_sequential_view();

			_varType = _seq.get_value_type();
		}

		if (_varType.is_wrapper())
			_varType = _varType.get_wrapped_type();
		if (_varType.is_pointer())
			_varType = _varType.get_raw_type();

		_objName = _varType.get_name().to_string();

		return std::move(_objName);
	}

	string GetLableName(rttr::variant& var)
	{
		rengine::Object* _obj = nullptr;

		if (var.get_type().is_wrapper())
		{
			auto _wrap = var.extract_wrapped_value();
			_obj = _wrap.get_value<rengine::Object*>();
		}
		else
			_obj = var.get_value<rengine::Object*>();

		assert(_obj = nullptr);

		return _obj->GetNameStr();
	}

	void ObjectButton::Render()
	{
		ImGui::Columns(2);
		ImGui::Text(GetWidgetName().c_str());
		ImGui::NextColumn();

		auto _var = m_prop.get_value(m_pHandler);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

		string _objName = GetObjectName(_var);
		string _lable = "None(" + _objName + ")";

		ButtonEvent _buttonEvent = nullptr;
		bool _openSeachView = false;

		if (_var.is_sequential_container())
		{
			auto _seq = _var.create_sequential_view();

			if (_seq.get_size() == 0)
			{
				if (ImGui::ButtonEx(_lable.c_str(), m_rectSize, GetFlags()))
					_openSeachView = true;
			}
			else
			{
				for (const auto& item : _seq)
				{
					auto _extractVar = item.extract_wrapped_value();

					if (_extractVar != nullptr)
						_lable = GetLableName(_extractVar);

					if (ImGui::ButtonEx(_lable.c_str(), m_rectSize, GetFlags()))
						_openSeachView = true;
				}
			}
		}
		else
		{
			if(_var != nullptr)
				_lable = GetLableName(_var);

			if (ImGui::ButtonEx(_lable.c_str(), m_rectSize, GetFlags()))
				_openSeachView = true;
		}
		
		ImGui::EndColumns();

		if (_openSeachView)
		{
			SearchView::OpenSeachView(_objName, this, &ObjectButton::SetProperty);
		}

		ImGui::PopStyleColor();
	}

	void ObjectButton::SetProperty(void* thisClass, const shared_ptr<rengine::Object>& obj)
	{
		ObjectButton* _this = reinterpret_cast<ObjectButton*>(thisClass);

		
	}
}