#include <Editor_pch.h>
#include <editor\Widget\SameLine.h>

namespace editor
{
	SameLine::SameLine(float offsetX, float spacing)
		: m_offset_from_start_x(offsetX)
		, m_spacing(spacing)
	{
	}

	void SameLine::Render()
	{
		ImGui::SameLine(m_offset_from_start_x, m_spacing);
	}
}