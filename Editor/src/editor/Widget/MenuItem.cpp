#include <Editor_pch.h>

#include <editor\Widget\MenuItem.h>

namespace editor
{
	MenuItem::MenuItem(const string& lable, const string& shortcut, bool isSelectedable, bool m_isSelected, uint32 flags)
		: Widget(lable, flags)
		, m_shortcut(shortcut)
		, m_isSelectedable(isSelectedable)
		, m_isSelected(m_isSelected)
	{

	}

	MenuItem::~MenuItem()
	{

	}

	void MenuItem::Render()
	{
		if (ImGui::MenuItem((m_lable + m_idString).c_str(),
			m_shortcut.c_str(), m_isSelectedable ? &m_isSelected : nullptr, m_isEnable))
		{
			m_clickedEvent();
		}
	}
}