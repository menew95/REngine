/**

    @file      InputFloat.h
    @brief     imgui float wrapper class
    @details   float peoperty과 상호작용함
    @author    LWT
    @date      22.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <editor\Widget\Widget.h>

#include <rttr/registration.h>

namespace rengine
{
    class Component;
}

namespace editor
{
    class InputFloat : public Widget
    {
    public:
        InputFloat(string name, rengine::Component* handler, rttr::property& prop, uint32 flags = 0);

        ~InputFloat();

        EDITOR_API void Render() override;

    private:

        // component handler;
        rengine::Component* m_pHandler;

        // property
        rttr::property m_prop;
    };
}

