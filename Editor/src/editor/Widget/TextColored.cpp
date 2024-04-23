#include <Editor_pch.h>

#include <editor\Widget\TextColored.h>

namespace editor
{
	TextColored::TextColored(const string& text, const math::Color& color)
		: Widget(text)
		, m_text(text)
		, m_textColor(color)
	{
	}

	
	void TextColored::Render()
	{
		ImGui::TextColored(Utility::ToImVec4(m_textColor), m_text.c_str());
	}
}