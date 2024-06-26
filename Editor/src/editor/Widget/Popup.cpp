﻿#include <Editor_pch.h>
#include <editor\Widget\Popup.h>


namespace editor
{
	Popup::Popup(const string lable, uint32 flags)
	: Widget(lable, flags)
	{
	}

	void Popup::Render()
	{
		if (m_openPopupCheckFunc())
		{
			ImGui::OpenPopup((m_lable + m_idString).c_str());
		}

		if (ImGui::BeginPopup((m_lable + m_idString).c_str()))
		{
			RenderChild();

			ImGui::EndPopup();
		}
	}

}