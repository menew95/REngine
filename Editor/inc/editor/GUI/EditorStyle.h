/**

    @file      EditorStyle.h
    @brief     editor style class
    @details   imgui에 사용될 global style의 설정들을 관리
    @author    Cool Guy
    @date      20.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <ImGui\imgui.h>

namespace editor
{
    class EditorStyle
    {
    public:
        static ImVec4 GetColor(ImGuiCol col);
    };
}