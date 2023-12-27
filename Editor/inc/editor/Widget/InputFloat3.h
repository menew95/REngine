/**

    @file      InputFloat3.h
    @brief     imgui float3 wrapper class
    @details   vector3 peoperty과 상호작용함
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
    class InputFloat3 : public Widget
    {
    public:
        //InputFloat3(string name, float* handler, /*Event<rengine::Object, void, string> event,*/ uint32 flags = 0);
        InputFloat3(string name, rengine::Component* handler, rttr::property& prop, uint32 flags = 0);

        ~InputFloat3();

        EDITOR_API void Render() override;

        void SetHandler(rengine::Component* handler) { m_pHandler = handler; }

    private:
        
        // component handler;
        rengine::Component* m_pHandler;

        // property
        rttr::property m_prop;
    };
}