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
        
        /**
            @brief 위젯이 활성화 되어 있으면 렌더함(ImGui에 렌더 데이터를 추가)
        **/
        virtual void Draw();

        /**
            @brief 렌더(ImGui에 렌더 데이터를 추가)
        **/
        virtual EDITOR_API void Render() abstract;

        EDITOR_API const string& GetLable() { return m_lable; }

        EDITOR_API uint32 GetFlags() { return m_flags; }

        auto GetEnable() const { return m_isEnable; }
        void SetEnable(auto val) { m_isEnable = val; }

    protected:
        bool m_isEnable = true;

        bool m_isReservedDestroy = false;

        string m_lable;

        uint32 m_flags = 0;

        friend class WidgetContainer;
    };
}
