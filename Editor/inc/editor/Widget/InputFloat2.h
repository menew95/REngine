/**

    @file      InputFloat2.h
    @brief     imgui float2 wrapper class
    @details   vector2 peoperty과 상호작용함
    @author    LWT
    @date      22.12.2023
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
    class InputFloat2 : public Widget
    {
    public:
        InputFloat2(string name, rengine::Object* handler, rttr::property& prop, uint32 flags = 0);

        ~InputFloat2();

        EDITOR_API void Render() override;

        EDITOR_API void SetHandler(rengine::Object* handler) { m_pHandler = handler; }

    private:

        // object handler;
        rengine::Object* m_pHandler;

        // property
        rttr::property m_prop;
    };
}