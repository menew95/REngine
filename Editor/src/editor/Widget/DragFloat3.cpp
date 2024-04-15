#include <Editor_pch.h>

#include <editor\Widget\DragFloat3.h>

#include <rengine\core\component\Component.h>

namespace editor
{
	DragFloat3::DragFloat3(string name, rengine::Object* handler, rttr::property& prop, float speed, float min, float max, uint32 flags)
		: Widget(name, flags)
		, m_pHandler(handler)
		, m_prop(prop)
		, m_speed(speed)
		, m_min(min)
		, m_max(max)
	{

	}

	DragFloat3::~DragFloat3()
	{

	}

	void DragFloat3::Render()
	{
		auto _val = m_prop.get_value(m_pHandler);

		if (!_val.can_convert<Vector3>())
			assert(false);

		auto _value = _val.convert<Vector3>();

		float* _handler[3] = { &_value.x, &_value.y, &_value.z };

		ImGui::PushID(m_widgetName.c_str());

		ImGui::Text(m_widgetName.c_str());

		ImGui::NextColumn();

		if (ImGui::DragFloat3("", *_handler, m_speed, m_min, m_max, "%.3f", GetFlags()))
		{
			m_prop.set_value(m_pHandler, _value);
		}

		ImGui::PopID();
	}
}