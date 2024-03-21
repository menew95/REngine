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
		if (ImGui::ButtonEx(GetWidgetName().c_str(), m_rectSize, GetFlags()))
		{

		}
	}
}