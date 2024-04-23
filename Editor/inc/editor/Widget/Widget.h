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

#include <editor\GUI\GUI.h>

namespace editor
{
    class Widget : public GUI
    {
    public:
        Widget(const string& lable = "", uint32 flags = 0);

        virtual ~Widget();

        virtual EDITOR_API void Render() abstract;

        EDITOR_API const string& GetLable() { return m_lable; }

        EDITOR_API uint32 GetFlags() { return m_flags; }

    protected:
        string m_lable;

        uint32 m_flags = 0;
    };
}
