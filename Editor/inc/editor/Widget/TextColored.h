/**

    @file      TextColored.h
    @brief     imgui text colored wrapper
    @details   ~
    @author    LWT
    @date      17.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\math.h>

#include <editor\Widget\Widget.h>

namespace editor
{
    class TextColored : public Widget
    {
    public:
        TextColored(const string& text = "", const math::Color& color = math::Color::White);
        ~TextColored() = default;

        void Render() override;

    protected:
        string m_text = "";
        math::Color m_textColor = math::Color::White;
    };
}