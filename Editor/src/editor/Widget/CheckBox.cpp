#include <Editor_pch.h>

#include <editor\Widget\CheckBox.h>

namespace editor
{
	CheckBox::CheckBox(const string& id, rttr::instance& obj, rttr::property& prop, uint32 flags)
	: WidgetData<bool>(id, obj, prop, flags)
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