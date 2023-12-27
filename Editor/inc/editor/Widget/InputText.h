/**

    @file      InputText.h
    @brief     imgui input text wrapper
    @details   imgui input text를 관리하는 widget
    @author    LWT
    @date      21.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <editor\Widget\Widget.h>

#include <editor\Core\Event.h>

#include <rttr\registration.h>

namespace rengine
{
    class Object;
}

namespace editor
{
    class InputText : public Widget
    {
    public:
        //InputText(string name, string& test, uint32 flags, Event<rengine::Object, void, string> event);
        InputText(string name, rengine::Object* handler, rttr::property& prop, uint32 flags = 32);

        ~InputText();

        EDITOR_API void Render() override;

        EDITOR_API void SetHandler(rengine::Object* handler) { m_pHandler = handler;}

    private:
        // object handler;
        rengine::Object* m_pHandler;

        // property
        rttr::property m_prop;

        char m_pInputText[256];

        /*std::string m_label;
        std::string& m_text;
        char m_pInputText[256];

        Event<rengine::Object, void, string> m_event;*/
    };
}
