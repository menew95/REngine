/**

    @file      ButtonImage.h
    @brief     imgui image button wrapper
    @details   ~
    @author    LWT
    @date      24.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <editor\Widget\Button.h>

namespace editor
{
    class ImageButton : public Button
    {
    public:
        ImageButton(void* textureID, const string& lable = "", const math::Vector2& size = {0, 0}, const math::Vector2& m_top = {1, 1},
        const math::Vector2& m_bot = { 0, 0 },
        const math::Vector4& m_bg_color = { 0, 0, 0, 0 },
        const math::Vector4& m_tint_color = { 1, 1, 1, 1 }, uint32 flags = 0);
        ~ImageButton();

        EDITOR_API void Render() override;

        void SetTextureID(void* val) { m_textureID = val; }
        void SetTop(const Vector2& val) { m_top = Utility::ToImVec2(val); }
        void SetBot(const Vector2& val) { m_bot = Utility::ToImVec2(val); }
        void SetBGColor(const Color& val) { m_bg_color = Utility::ToImVec4(val); }
        void SetTintColor(const Color& val) { m_tint_color = Utility::ToImVec4(val); }

    private:
        void* m_textureID = nullptr;

        ImVec2 m_top = {1, 1};
        ImVec2 m_bot = {0, 0};
        ImVec4 m_bg_color = {0, 0, 0, 0};
        ImVec4 m_tint_color = {1, 1, 1, 1};
    };
}