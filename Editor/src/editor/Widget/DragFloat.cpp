#include <Editor_pch.h>

#include <editor\Widget\DragFloat.h>

#include <rengine\core\component\Component.h>

namespace editor
{
	DragFloat::DragFloat(string name, rengine::Object* handler, rttr::property& prop, float speed, float min, float max, uint32 flags)
		: Widget(name, flags)
		, m_pHandler(handler)
		, m_prop(prop)
		, m_speed(speed)
		, m_min(min)
		, m_max(max)
	{

	}
	
	DragFloat::~DragFloat()
	{

	}
	
	void DragFloat::Render()
	{
		auto _val = m_prop.get_value(m_pHandler);

		if (!_val.can_convert<float>())
			assert(false);

		float _value = _val.convert<float>();

		ImGui::PushID(m_widgetName.c_str());

		ImGui::Text(m_widgetName.c_str());
		//ImGui::SameLine();
		ImGui::NextColumn();

		if (ImGui::DragFloat("", &_value, m_speed, m_min, m_max, "%.3f", GetFlags()))
		{
			m_prop.set_value(m_pHandler, _value);
		}

		ImGui::PopID();
	}
}