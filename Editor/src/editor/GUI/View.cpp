#include <Editor_pch.h>

#include <editor\GUI\View.h>

namespace editor
{
	View::View()
	{

	}

	View::~View()
	{

	}

	void View::Begin()
	{
		ImGui::Begin(m_ViewName.c_str());
	}

	void View::Render()
	{

	}

	void View::End()
	{
		ImGui::End();
	}
}