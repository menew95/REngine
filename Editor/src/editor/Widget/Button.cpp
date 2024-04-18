#include <Editor_pch.h>
#include <editor\Widget\Button.h>

#include <rengine/core/component/Component.h>
namespace editor
{
	Button::Button(const string& id, math::Vector2 size, uint32 flags)
		: Widget(id, flags)
		, m_rectSize{ size.x, size.y }
	{
	}

	Button::~Button()
	{

	}

	void Button::Render()
	{
		if (ImGui::ButtonEx(m_id.c_str(), m_rectSize, GetFlags()))
		{

		}
	}
}