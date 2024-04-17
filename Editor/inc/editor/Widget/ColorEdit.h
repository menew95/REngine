/**

    @file      ColorEdit.h
    @brief     
    @details   ~
    @author    Cool Guy
    @date      3.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <editor\Widget\Widget.h>

#include <rttr\registration.h>

namespace rengine
{
    class Object;
}

namespace editor
{
    class ColorEdit : public Widget
    {
    public:
        ColorEdit(const string& id, rengine::Object* handler, rttr::property& prop, uint32 flags = 0);

        ~ColorEdit();

        virtual EDITOR_API void Render() override;

        void SetHandler(rengine::Object* handler) { m_pHandler = handler; }

    private:
        // component handler;
        rengine::Object* m_pHandler;

        // property
        rttr::property m_prop;
    };
}