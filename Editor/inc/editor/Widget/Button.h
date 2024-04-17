/**

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
        Button(const string& id, rengine::Object* handler, rttr::property& prop, math::Vector2 size = {0, 0}, uint32 flags = 0);

        ~Button();

        virtual EDITOR_API void Render() override;

        void SetHandler(rengine::Object* handler) { m_pHandler = handler; }

    private:
        ImVec2 m_rectSize;

        // component handler;
        rengine::Object* m_pHandler;

        // property
        rttr::property m_prop;
    };
}