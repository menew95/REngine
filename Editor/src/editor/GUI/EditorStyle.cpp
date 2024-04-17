#include <Editor_pch.h>

#include <editor\GUI\EditorStyle.h>

namespace editor
{
    ImVec4 g_editorColor[] =
    {
        ImVec4(1.0f, 0.0f, 0.0f, 1.0f), // ImGuiCol_Text,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_TextDisabled,
        ImVec4(0.8f, 0.8f, 0.8f, 1.0f), // ImGuiCol_WindowBg,              // Background of normal windows
        ImVec4(0.8f, 0.8f, 0.8f, 1.0f), // ImGuiCol_ChildBg,               // Background of child windows
        ImVec4(0.8f, 0.8f, 0.8f, 1.0f), // ImGuiCol_PopupBg,               // Background of popups, menus, tooltips windows
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_Border,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_BorderShadow,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_FrameBg,               // Background of checkbox, radio button, plot, slider, text input
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_FrameBgHovered,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_FrameBgActive,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_TitleBg,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_TitleBgActive,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_TitleBgCollapsed,
        ImVec4(0.7f, 0.7f, 0.7f, 1.0f), // ImGuiCol_MenuBarBg,
        ImVec4(1.0f, 1.0f, 1.0f, 1.0f), // ImGuiCol_ScrollbarBg,
        ImVec4(0.8f, 0.8f, 0.8f, 1.0f), // ImGuiCol_ScrollbarGrab,
        ImVec4(0.7f, 0.7f, 0.7f, 1.0f), // ImGuiCol_ScrollbarGrabHovered,
        ImVec4(0.7f, 0.7f, 0.7f, 1.0f), // ImGuiCol_ScrollbarGrabActive,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_CheckMark,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_SliderGrab,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_SliderGrabActive,
        ImVec4(1.0f, 1.0f, 1.0f, 1.0f), // ImGuiCol_Button,
        ImVec4(0.7f, 0.7f, 0.7f, 1.0f), // ImGuiCol_ButtonHovered,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_ButtonActive,
        ImVec4(0.8f, 0.8f, 0.8f, 1.0f), // ImGuiCol_Header,                // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
        ImVec4(0.7f, 0.7f, 0.7f, 1.0f), // ImGuiCol_HeaderHovered,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_HeaderActive,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_Separator,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_SeparatorHovered,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_SeparatorActive,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_ResizeGrip,            // Resize grip in lower-right and lower-left corners of windows.
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_ResizeGripHovered,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_ResizeGripActive,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_Tab,                   // TabItem in a TabBar
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_TabHovered,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_TabActive,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_TabUnfocused,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_TabUnfocusedActive,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_DockingPreview,        // Preview overlay color when about to docking something
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_DockingEmptyBg,        // Background color for empty node (e.g. CentralNode with no window docked into it)
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_PlotLines,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_PlotLinesHovered,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_PlotHistogram,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_PlotHistogramHovered,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_TableHeaderBg,         // Table header background
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_TableBorderStrong,     // Table outer and header borders (prefer using Alpha=1.0 here)
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_TableBorderLight,      // Table inner borders (prefer using Alpha=1.0 here)
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_TableRowBg,            // Table row background (even rows)
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_TableRowBgAlt,         // Table row background (odd rows)
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_TextSelectedBg,
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_DragDropTarget,        // Rectangle highlighting a drop target
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_NavHighlight,          // Gamepad/keyboard: current highlighted item
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_NavWindowingHighlight, // Highlight window when using CTRL+TAB
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_NavWindowingDimBg,     // Darken/colorize entire screen behind the CTRL+TAB window list, when active
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f), // ImGuiCol_ModalWindowDimBg,      // Darken/colorize entire screen behind a modal window, when one is active
        ImVec4(0.2f, 0.2f, 0.2f, 1.0f)  // ImGuiCol_COUNT
    };

    ImVec4 EditorStyle::GetColor(ImGuiCol col)
    {
        return g_editorColor[col];
    }

    ImVec4 Utility::ToImVec4(const math::Vector4& vec)
    {
        ImVec4 ret;

        memcpy(&ret, &vec, sizeof(ImVec4));

        return ret;
    }
    
    ImVec2 Utility::ToImVec2(const math::Vector2& vec)
    {
        ImVec2 ret;

        memcpy(&ret, &vec, sizeof(ImVec2));

        return ret;
    }
    
    ImVec4 Utility::ToImVec4(const math::Color& col)
    {
        ImVec4 ret;

        memcpy(&ret, &col, sizeof(ImVec4));

        return ret;
    }
    
    math::Vector4 Utility::ToVector4(const ImVec4& vec)
    {
        Vector4 ret;

        memcpy(&ret, &vec, sizeof(Vector4));

        return ret;
    }
    
    math::Vector2 Utility::ToVector2(const ImVec2& vec)
    {
        Vector2 ret;

        memcpy(&ret, &vec, sizeof(Vector2));

        return ret;
    }
    
    Color Utility::ToColor(const ImVec4& vec)
    {
        Color ret;

        memcpy(&ret, &vec, sizeof(Color));

        return ret;
    }
}