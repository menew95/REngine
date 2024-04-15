/**

    @file      DragFloat.h
    @brief     imgui drag float4 widget wrapper
    @details   vector4 peoperty과 상호작용함
    @author    LWT
    @date      15.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

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
    class DragFloat4 : public Widget
    {
    public:
        DragFloat4(string name, rengine::Object* handler, rttr::property& prop, float speed, float min, float max, uint32 flags = 0);

        ~DragFloat4();

        EDITOR_API void Render() override;

        EDITOR_API void SetHandler(rengine::Object* handler) { m_pHandler = handler; }

    private:

        // object handler;
        rengine::Object* m_pHandler;

        // property
        rttr::property m_prop;

        float m_speed = 1.0f;

        float m_min = FLT_MIN;

        float m_max = FLT_MAX;
    };
}