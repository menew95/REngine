/**

    @file      SameLine.h
    @brief     imgui SameLine wrapper
    @details   SameLine layout를 위젯처럼 사용하기 위한 클래스
    @author    LWT
    @date      23.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <editor\Widget\Widget.h>

namespace editor
{
    class SameLine : public Widget
    {
    public:
        SameLine(float offsetX = 0.0f, float spacing = -1.0f);

        void Render() override;

    protected:
        float m_offset_from_start_x = 0.0f;
        float m_spacing = -1.0f;
    };
}