#include <Editor_pch.h>

#include <editor\Widget\InputText.h>

namespace editor
{
	InputText::InputText(string name, string& text, uint32 flags, Event<rengine::Object, void, string> event)
	: Widget(name, flags)
	, m_text(text)
	, m_label("##")
	, m_event(event)
	{
		m_label.append(name);
	}

	InputText::~InputText()
	{
	}

	void InputText::Render()
	{
		strcpy_s(m_pInputText, m_text.c_str());

		if (ImGui::InputTextEx(m_label.c_str(), "GameObject Name", m_pInputText, IM_ARRAYSIZE(m_pInputText), ImVec2(.0f, 0.f), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			m_event.Invoke(m_pInputText);
		}
	}
}