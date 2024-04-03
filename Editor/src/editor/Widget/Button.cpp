#include <Editor_pch.h>
#include <editor\Widget\Button.h>

#include <rengine/core/component/Component.h>
namespace editor
{
	Button::Button(string name, rengine::Object* handler, rttr::property& prop, math::Vector2 size, uint32 flags)
		: Widget(name, flags)
		, m_rectSize{ size.x, size.y }
		, m_pHandler(handler)
		, m_prop(prop)
	{
	}

	Button::~Button()
	{

	}

	void Button::Render()
	{
		ImGui::PushID(m_widgetName.c_str());

		ImGui::Text(GetWidgetName().c_str());
		//ImGui::SameLine();
		ImGui::NextColumn();

		if (ImGui::ButtonEx("", m_rectSize, GetFlags()))
		{

		}

		ImGui::PopID();
	}
}