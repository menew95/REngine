#include <Editor_pch.h>

#include <editor\Widget\Menu.h>

namespace editor
{
	Menu::Menu(const string& lable, uint32 flags)
		: Widget(lable, flags)
	{

	}

	Menu::~Menu()
	{

	}

	void Menu::Render()
	{
		if (ImGui::BeginMenu((m_lable + m_idString).c_str(), &m_isOpened))
		{
			/*if (!m_isOpened)
			{
				m_isOpened = true;
			}*/

			RenderChild();

			ImGui::EndMenu();
		}
	}
}