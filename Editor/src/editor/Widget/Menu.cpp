#include <Editor_pch.h>

#include <editor\Widget\Menu.h>

namespace editor
{
	Menu::Menu(const string& id, uint32 flags)
		: WidgetContainer(id, flags)
	{

	}

	Menu::~Menu()
	{

	}

	void Menu::Render()
	{
		if (ImGui::BeginMenu(m_id.c_str(), &m_isOpened))
		{
			/*if (!m_isOpened)
			{
				m_isOpened = true;
			}*/

			WidgetContainer::Render();

			ImGui::EndMenu();
		}
	}
}