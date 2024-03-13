/**

    @file      Widget.h
    @brief     Imgui widget를 컨트롤하는 widget class들의 인터페이스
    @details   ~
    @author    LWT
    @date      20.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <common\math.h>

#include <editor\Editor.h>

namespace editor
{
    class Widget
    {
    public:
        Widget(string name, uint32 flags);

        virtual ~Widget();

        virtual EDITOR_API void Render() abstract;

        EDITOR_API string& GetWidgetName() { return m_widgetName; }

        EDITOR_API uint32 GetFlags() { return m_flags; }

    protected:
        string m_widgetName;

        uint32 m_flags;
    };
}
