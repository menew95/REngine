#include <Editor_pch.h>
#include <editor\Widget\InputFloat4.h>

#include <rengine/core/component/Component.h>

namespace editor
{
	InputFloat4::InputFloat4(string name, rengine::Object* handler, rttr::property& prop, uint32 flags)
		: Widget(name, flags)
		, m_pHandler(handler)
		, m_prop(prop)
	{

	}

	InputFloat4::~InputFloat4()
	{

	}

	void InputFloat4::Render()
	{
		auto _val = m_prop.get_value(m_pHandler);

		if (!_val.can_convert<math::Vector4>())
			assert(false);

		math::Vector4 _vec = _val.convert<math::Vector4>();

		float* _handler[4] = { &_vec.x, &_vec.y, &_vec.z, &_vec.w };

		ImGui::PushID(m_widgetName.c_str());

		ImGui::Text(m_widgetName.c_str());
		//ImGui::SameLine();
		ImGui::NextColumn();

		if (ImGui::InputFloat4("", *_handler, "%.3f", GetFlags()))
		{
			m_prop.set_value(m_pHandler, _vec);
		}
		
		ImGui::PopID();
	}
}