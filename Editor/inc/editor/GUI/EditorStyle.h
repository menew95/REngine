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

#include <common\math.h>

#include <ImGui\imgui.h>

namespace editor
{
    class Utility
    {
    public:
        static ImVec4 ToImVec4(const math::Vector4& vec);

        static ImVec2 ToImVec2(const math::Vector2& vec);

        static ImVec4 ToImVec4(const math::Color& col);

        static math::Vector4 ToVector4(const ImVec4& vec);

        static math::Vector2 ToVector2(const ImVec2& vec);

        static Color ToColor(const ImVec4& vec);
    };

    class EditorStyle
    {
    public:
        static ImVec4 GetColor(ImGuiCol col);

    };
}