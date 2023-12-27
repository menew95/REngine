#include <Editor_pch.h>

#include <editor\Widget\InputFloat.h>

#include <rengine/core/component/Component.h>

namespace editor
{
	InputFloat::InputFloat(string name, rengine::Object* handler, rttr::property& prop, uint32 flags)
		: Widget(name, flags)
		, m_pHandler(handler)
		, m_prop(prop)
	{

	}

	InputFloat::~InputFloat()
	{

	}

	void InputFloat::Render()
	{
		auto _val = m_prop.get_value(m_pHandler);

		if (!_val.can_convert<float>())
			assert(false);

		float _vec = _val.convert<float>();

		if (ImGui::InputFloat(GetWidgetName().c_str(), &_vec, 0.f, 0.f, "%.3f", GetFlags()))
		{
			m_prop.set_value(m_pHandler, _vec);
		}
	}
}