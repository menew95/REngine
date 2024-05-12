#include <Editor_pch.h>

#include <editor\Widget\ComboBox.h>

#include <rengine/core/component/Component.h>

namespace editor
{
	ComboBox::ComboBox(const string& lable, uint32 flags)
	: WidgetData<int>(lable, flags)
	{

	}

	ComboBox::~ComboBox()
	{

	}

	void ComboBox::Draw()
	{
		if (ImGui::BeginCombo((m_lable + m_idString).c_str(), m_items[m_data].c_str()))
		{
			for (size_t i = 0; i < m_items.size(); i++)
			{
				bool selected = (i == m_data);

				if (ImGui::Selectable(m_items[i].c_str(), selected))
				{
					if (!selected)
					{
						ImGui::SetItemDefaultFocus();

						m_data = (int)i;

						m_isValChange = true;
					}
				}
			}

			ImGui::EndCombo();
		}
	}
}