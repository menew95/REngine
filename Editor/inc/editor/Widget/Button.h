﻿/**

    @file      Button.h
    @brief     imgui button wrapper
    @details   imgui button 관리하는 widget
    @author    LWT
    @date      21.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <editor\Widget\Widget.h>

#include <rttr/registration.h>

namespace rengine
{
    class Object;
}

namespace editor
{
    class Button : public Widget
    {
    public:
        Button(const string& lable = "", math::Vector2 size = {0, 0}, uint32 flags = 0);

        ~Button();

        virtual EDITOR_API void Render() override;

        void SetClickEvent(function<void()> event) { m_clickEvent = event; }

        void SetDisable(bool val) { m_isDisabled = val; }

    protected:
        bool m_isDisabled;

        ImVec2 m_rectSize;

        function<void()> m_clickEvent;
    };
}