#include <Editor_pch.h>
#include <editor\Widget\ImageButton.h>

namespace editor
{
	ImageButton::ImageButton(void* textureID, const string& lable, const math::Vector2& size, const math::Vector2& top,
		const math::Vector2& bot,
		const math::Vector4& bg_color,
		const math::Vector4& tint_color, uint32 flags)
		: Button(lable, size, flags)
		, m_textureID(textureID)
		, m_top(Utility::ToImVec2(top))
		, m_bot(Utility::ToImVec2(bot))
		, m_bg_color(Utility::ToImVec4(bg_color))
		, m_tint_color(Utility::ToImVec4(tint_color))
	{
	}

	ImageButton::~ImageButton()
	{
	}

	void ImageButton::Render()
	{
		if (m_isDisabled)
		{
			ImGui::BeginDisabled();
		}

		if (ImGui::ImageButton((m_lable + m_idString).c_str(), m_textureID, m_rectSize, m_top, m_bot, m_bg_color, m_tint_color))
		{
			m_clickEvent();
		}

		if (m_isDisabled)
		{
			ImGui::EndDisabled();
		}
	}
}