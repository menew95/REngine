#include <Editor_pch.h>

#include <editor\GUI\View.h>

#include <editor\Widget\Widget.h>

namespace editor
{
	View::View(std::string name, WindowSettings settings)
	: m_viewName(name)
	, m_windowSettings(settings)
	{
		//LoadSetting();
	}

	View::~View()
	{

	}

	void View::Begin()
	{
		m_flags = ImGuiWindowFlags_None;

		if (m_windowSettings._notitleBar)					m_flags |= ImGuiWindowFlags_None;
		if (m_windowSettings._noResize)						m_flags |= ImGuiWindowFlags_NoTitleBar;
		if (m_windowSettings._noMove)						m_flags |= ImGuiWindowFlags_NoResize;
		if (m_windowSettings._noScrollbar)					m_flags |= ImGuiWindowFlags_NoMove;
		if (m_windowSettings._noScrollWithMouse)			m_flags |= ImGuiWindowFlags_NoScrollbar;
		if (m_windowSettings._noCollapse)					m_flags |= ImGuiWindowFlags_NoScrollWithMouse;
		if (m_windowSettings._alwaysAutoResize)				m_flags |= ImGuiWindowFlags_NoCollapse;
		if (m_windowSettings._noBackground)					m_flags |= ImGuiWindowFlags_AlwaysAutoResize;
		if (m_windowSettings._noInput)						m_flags |= ImGuiWindowFlags_NoBackground;
		if (m_windowSettings._noSavedSettings)				m_flags |= ImGuiWindowFlags_NoSavedSettings;
		if (m_windowSettings._useMouseInput)				m_flags |= ImGuiWindowFlags_NoMouseInputs;
		if (m_windowSettings._hasMenuBar)					m_flags |= ImGuiWindowFlags_MenuBar;
		if (m_windowSettings._horizontalScrollbar)			m_flags |= ImGuiWindowFlags_HorizontalScrollbar;
		if (m_windowSettings._noFocusOnAppearing)			m_flags |= ImGuiWindowFlags_NoFocusOnAppearing;
		if (m_windowSettings._noBringToFrontOnFocus)		m_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		if (m_windowSettings._alwaysVerticalScrollbar)		m_flags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;
		if (m_windowSettings._alwaysHorizontalScrollbar)	m_flags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;
		if (m_windowSettings._alwaysUseWindowPadding)		m_flags |= ImGuiWindowFlags_AlwaysUseWindowPadding;
		if (m_windowSettings._noNavInputs)					m_flags |= ImGuiWindowFlags_NoNavInputs;
		if (m_windowSettings._noNavFocus)					m_flags |= ImGuiWindowFlags_NoNavFocus;
		if (m_windowSettings._unsavedDocument)				m_flags |= ImGuiWindowFlags_UnsavedDocument;
		if (m_windowSettings._noDocking)					m_flags |= ImGuiWindowFlags_NoDocking;

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