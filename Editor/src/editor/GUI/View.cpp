#include <Editor_pch.h>

#include <editor\GUI\View.h>

#include <editor\Widget\Widget.h>

namespace editor
{
	View::View(std::string name)
	: m_viewName(name)
	{
		//LoadSetting();
	}

	View::~View()
	{

	}

	void View::Begin()
	{
		ImGui::Begin(m_viewName.c_str(), &m_bOpen, m_flags);
	}

	void View::Render()
	{
		RenderChild();
		/*for (auto* _widget : m_childWidget)
		{
			_widget->Render();
		}*/
	}

	void View::End()
	{
		/*if (!m_bOpen)
			return;*/

		ImGui::End();
	}
}