#include <Editor_pch.h>
#include <editor\Widget\Button.h>

#include <rengine/core/component/Component.h>
namespace editor
{
	Button::Button(const string& lable, math::Vector2 size, uint32 flags)
		: Widget(lable, flags)
		, m_rectSize{ size.x, size.y }
	{
	}

	Button::~Button()
	{

	}

	void Button::Render()
	{
		if (ImGui::ButtonEx((m_lable + m_idString).c_str(), m_rectSize, GetFlags()))
		{

		}
	}
}