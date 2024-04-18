/**

    @file      InputText.h
    @brief     imgui input text wrapper
    @details   imgui input text를 관리하는 widget
    @author    LWT
    @date      21.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <editor\Widget\WidgetData.h>

namespace rengine
{
    class Object;
}

namespace editor
{
    class InputText : public WidgetData<tstring>
    {
    public:
        InputText(const string& id, rttr::instance& obj, rttr::property& prop, const string& hint, uint32 flags = 32);

        ~InputText();

    private:
        void Draw() override;

    protected:
        string m_hint;

        char m_buf[256];

        ImGuiInputTextCallback m_callback = NULL;

        void* m_userData = NULL;
    };
}
