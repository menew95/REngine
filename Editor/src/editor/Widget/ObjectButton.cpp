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
		m_flags |= ImGuiButtonFlags_PressedOnDoubleClick;

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

		assert(_obj != nullptr);

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
			m_bIsArray = true;

			if (_seq.get_size() == 0)
			{
				if (ImGui::ButtonEx(_lable.c_str(), m_rectSize, GetFlags()))
					_openSeachView = true;
			}
			else
			{
				for (size_t i = 0; i < _seq.get_size(); i++)
				{
					auto _extractVar = _seq.get_value(i).extract_wrapped_value();

					if (_extractVar != nullptr)
						_lable = GetLableName(_extractVar);

					if (ImGui::ButtonEx(_lable.c_str(), m_rectSize, GetFlags()))
					{
						_openSeachView = true;
						m_arrayIndex = i;
					}
				}
			}
		}
		else
		{
			m_bIsArray = false;

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


		if (_this->m_bIsArray)
		{
			auto _varSeq = _this->m_prop.get_value(_this->m_pHandler);

			auto _seq = _varSeq.create_sequential_view();

			// shared_ptr<object> => shared_ptr<해당클래스>
			rttr::variant _var = obj;

			assert(_var.convert(_seq.get_value_type()));


			if (_seq.get_size() == 0)
			{
				_seq.set_size(1);
				_seq.set_value(0, _var);
			}
			else
				_seq.set_value(_this->m_arrayIndex, _var);

			assert(_this->m_prop.set_value(_this->m_pHandler, _varSeq));
		}
		else
		{
			rttr::variant _var = obj;

			assert(_var.convert(_this->m_prop.get_type()));

			assert(_this->m_prop.set_value(_this->m_pHandler, _var));
		}
	}
}