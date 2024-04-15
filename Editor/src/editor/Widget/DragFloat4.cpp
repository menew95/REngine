#include <Editor_pch.h>

#include <editor\Widget\DragFloat4.h>

#include <rengine\core\component\Component.h>

namespace editor
{
	DragFloat4::DragFloat4(string name, rengine::Object* handler, rttr::property& prop, float speed, float min, float max, uint32 flags)
		: Widget(name, flags)
		, m_pHandler(handler)
		, m_prop(prop)
		, m_speed(speed)
		, m_min(min)
		, m_max(max)
	{

	}

	DragFloat4::~DragFloat4()
	{

	}

	void DragFloat4::Render()
	{
		auto _val = m_prop.get_value(m_pHandler);

		if (!_val.can_convert<Vector4>())
			assert(false);

		auto _value = _val.convert<Vector4>();

		float* _handler[4] = { &_value.x, &_value.y, &_value.z, &_value.w };

		ImGui::PushID(m_widgetName.c_str());

		ImGui::Text(m_widgetName.c_str());

		ImGui::NextColumn();

		if (ImGui::DragFloat("", *_handler, m_speed, m_min, m_max, "%.3f", GetFlags()))
		{
			m_prop.set_value(m_pHandler, _value);
		}

		ImGui::PopID();
	}
}