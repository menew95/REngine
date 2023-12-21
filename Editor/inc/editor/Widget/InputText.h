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

namespace editor
{
    class InputText : public Widget
    {
    public:
        InputText(string name, string& test, uint32 flags = 0);

        ~InputText();

        EDITOR_API void Render() override;

        bool Test();

    private:
        std::string m_label;
        std::string& m_text;
        char m_pInputText[256];
    };
}
