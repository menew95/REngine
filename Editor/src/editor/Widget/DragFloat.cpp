#include <Editor_pch.h>

#include <editor\Widget\DragFloat.h>

#include <rengine\core\component\Component.h>

namespace editor
{
	DragFloat::DragFloat(const string& id, rengine::Object* handler, rttr::property& prop, float speed, float min, float max, uint32 flags)
		: Widget(id, flags)
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

		if (ImGui::DragFloat(m_id.c_str(), &_value, m_speed, m_min, m_max, "%.3f", GetFlags()))
		{
			m_prop.set_value(m_pHandler, _value);
		}
	}
}