#include <Editor_pch.h>

#include <editor\Widget\DragFloat4.h>

#include <rengine\core\component\Component.h>

namespace editor
{
	DragFloat4::DragFloat4(const string& id, rengine::Object* handler, rttr::property& prop, float speed, float min, float max, uint32 flags)
		: Widget(id, flags)
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

		if (ImGui::DragFloat(m_id.c_str(), *_handler, m_speed, m_min, m_max, "%.3f", GetFlags()))
		{
			m_prop.set_value(m_pHandler, _value);
		}
	}
}