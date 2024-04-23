#include <Editor_pch.h>

#include <editor\Widget\InputText.h>

#include <rengine/core/component/Component.h>

namespace editor
{
	InputText::InputText(const string& lable, const string& hint, uint32 flags)
	: WidgetData<tstring>(lable, flags)
	, m_hint(hint)
	{
		memset(m_buf, 0, sizeof(m_buf));
	}

	InputText::~InputText()
	{
	}

	void InputText::Draw()
	{
		string _str = StringHelper::WStringToString(m_data);

		strcpy_s(m_buf, _str.c_str());

		if (ImGui::InputTextEx((m_lable + m_idString).c_str(), m_hint.c_str(), m_buf, IM_ARRAYSIZE(m_buf), ImVec2(.0f, .0f), m_flags, m_callback, m_userData))
		{
			m_isValChange = true;

			m_data = StringHelper::ToWString(m_buf);
		}
	}
}