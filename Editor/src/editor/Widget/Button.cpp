#include <Editor_pch.h>
#include <editor\Widget\Button.h>

namespace editor
{
	Button::Button(string name, vector<Widget*> childs, uint32 flags)
		: Widget(name, childs, flags)
		, m_rectSize(ImVec2(0.f, 0.f))
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