#include <Editor_pch.h>

#include <editor\Widget\CheckBox.h>

namespace editor
{
	CheckBox::CheckBox(const string& id, uint32 flags)
	: WidgetData<bool>(id, flags)
	{
	}

	CheckBox::~CheckBox()
	{

	}

	void CheckBox::Draw()
	{
		if (ImGui::Checkbox(m_id.c_str(), &m_data))
		{
			m_isValChange = true;
		}
	}
}