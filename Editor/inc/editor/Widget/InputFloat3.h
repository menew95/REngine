﻿/**

    @file      InputFloat3.h
    @brief     imgui float3 wrapper class
    @details   vector3 peoperty과 상호작용함
    @author    LWT
    @date      22.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <editor\Widget\Widget.h>

#include <editor\Core\Event.h>

namespace editor
{
    class InputFloat3 : public Widget
    {
    public:
        InputFloat3(string name, float* handler, /*Event<rengine::Object, void, string> event,*/ uint32 flags = 0);

        ~InputFloat3();

        EDITOR_API void Render() override;

    private:
        float* m_pHandler = nullptr;

        float _handler[3];

        void* _comp;
        //Event<rengine::Object, void, string> m_event;
    };
}