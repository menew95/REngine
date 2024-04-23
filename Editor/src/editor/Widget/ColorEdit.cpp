#include <Editor_pch.h>

#include <editor\Widget\ColorEdit.h>

#include <rengine\core\component\Component.h>

namespace editor
{
	ColorEdit::ColorEdit(const string& lable, uint32 flags)
		: WidgetData<Color>(lable, flags)
	{
	}

	ColorEdit::~ColorEdit()
	{

	}

	void ColorEdit::Draw()
	{
		memcpy(m_colorBuf, &m_data, sizeof(m_data));

		if (ImGui::ColorEdit4((m_lable + m_idString).c_str(), m_colorBuf, m_flags))
		{
			memcpy(&m_data, m_colorBuf, sizeof(m_colorBuf));

			m_isValChange = true;
		}
	}
}