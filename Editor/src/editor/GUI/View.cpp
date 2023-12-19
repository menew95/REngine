#include <Editor_pch.h>

#include <editor\GUI\View.h>

namespace editor
{
	View::View(std::string name)
	: m_ViewName(name)
	{
		LoadSetting();
	}

	View::~View()
	{

	}

	void View::Begin()
	{
		if(!m_bOpen)
			return;

		ImGui::PushID(m_ViewName.c_str());
		ImGui::Begin(m_ViewName.c_str(), &m_bOpen, m_flags);
	}

	void View::Render()
	{
		if (!m_bOpen)
			return;
	}

	void View::End()
	{
		if (!m_bOpen)
			return;
		ImGui::End();
	}

	void View::LoadSetting()
	{
		auto* _settings = ImGui::FindWindowSettingsByID(ImGui::GetID(m_ViewName.c_str()));

		m_bOpen = _settings->Collapsed;
	}
}