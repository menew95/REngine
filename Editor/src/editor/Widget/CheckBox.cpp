#include <Editor_pch.h>

#include <editor\Widget\CheckBox.h>

#include <rengine\core\component\Component.h>

namespace editor
{
	CheckBox::CheckBox(string name, rengine::Object* handler, rttr::property& prop, uint32 flags)
		: Widget(name, flags)
		, m_pHandler(handler)
		, m_prop(prop)
	{

	}

	CheckBox::~CheckBox()
	{

	}

	void CheckBox::Render()
	{
		auto _val = m_prop.get_value(m_pHandler);

		if (!_val.can_convert<bool>())
			assert(false);

		bool _bool = _val.convert<bool>();
		
		ImGui::PushID(m_widgetName.c_str());
		
		ImGui::Text(GetWidgetName().c_str());
		//ImGui::SameLine();
		ImGui::NextColumn();

		if (ImGui::Checkbox("", &_bool))
		{
			m_prop.set_value(m_pHandler, _bool);
		}

		ImGui::PopID();
	}
}