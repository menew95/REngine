#include <Editor_pch.h>

#include <editor\Widget\MenuItem.h>

namespace editor
{
	MenuItem::MenuItem(const string& id, const string& shortcut, bool isSelectedable, bool m_isSelected, uint32 flags)
		: Widget(id, flags)
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
		if (ImGui::MenuItem(m_id.c_str(),
			m_shortcut.c_str(), m_isSelectedable ? &m_isSelected : nullptr, m_isEnable))
		{
			m_clickedEvent();
		}
	}
}