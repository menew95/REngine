#include <Editor_pch.h>

#include <editor\Widget\CheckBox.h>

namespace editor
{
	CheckBox::CheckBox(const string& lable, uint32 flags)
	: WidgetData<bool>(lable, flags)
	{
	}

	CheckBox::~CheckBox()
	{

	}

	void CheckBox::Draw()
	{
		if (ImGui::Checkbox((m_lable + m_idString).c_str(), &m_data))
		{
			m_isValChange = true;
		}
	}
}