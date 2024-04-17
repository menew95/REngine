#include <Editor_pch.h>

#include <editor\Widget\TextColored.h>

namespace editor
{
	TextColored::TextColored(const string& id, const math::Color& color)
		: Widget(id)
		, m_textColor(color)
	{
	}

	
	void TextColored::Render()
	{
		ImGui::TextColored(Utility::ToImVec4(m_textColor), m_id.c_str());
	}
}