#include <Editor_pch.h>
#include <editor\Widget\InputFloat2.h>

#include <rengine/core/component/Component.h>

namespace editor
{
	InputFloat2::InputFloat2(string name, rengine::Object* handler, rttr::property& prop, uint32 flags)
		: Widget(name, flags)
		, m_pHandler(handler)
		, m_prop(prop)
	{

	}

	InputFloat2::~InputFloat2()
	{

	}

	void InputFloat2::Render()
	{
		auto _val = m_prop.get_value(m_pHandler);

		if (!_val.can_convert<math::Vector2>())
			assert(false);

		math::Vector2 _vec = _val.convert<math::Vector2>();

		float* _handler[2] = { &_vec.x, &_vec.y };

		ImGui::PushID(m_widgetName.c_str());

		ImGui::Text(m_widgetName.c_str());
		//ImGui::SameLine();
		ImGui::NextColumn();

		if (ImGui::InputFloat2("", *_handler, "%.3f", GetFlags()))
		{
			m_prop.set_value(m_pHandler, _vec);
		}

		ImGui::PopID();
	}
}