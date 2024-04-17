/**

    @file      CheckBox.h
    @brief     imgui Check Box wrapper
    @details   imgui Check Box 관리하는 widget component나 gamobject의 active 컨트롤 들에 사용됨
    @author    LWT
    @date      27.12.2023
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
    class CheckBox : public Widget
    {
    public:
        CheckBox(const string& id, rengine::Object* handler, rttr::property& prop, uint32 flags = 0);

        ~CheckBox();

        virtual EDITOR_API void Render() override;

        void SetHandler(rengine::Object* handler) { m_pHandler = handler; }

    private:
        // component handler;
        rengine::Object* m_pHandler;

        // property
        rttr::property m_prop;
    };
}