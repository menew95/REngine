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
        TextColored(const string& id, const math::Color& color);
        ~TextColored() = default;

        void Render() override;

    protected:
        math::Color m_textColor = math::Color::White;
    };
}