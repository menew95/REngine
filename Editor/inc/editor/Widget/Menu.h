/**

    @file      Menu.h
    @brief     imgui menu widget wrapper
    @details   하위 menu item을 순회하며 렌더링
    @author    LWT
    @date      22.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <editor\Widget\Widget.h>

#include <editor\Widget\WidgetContainer.h>

namespace editor
{
    class Menu : public Widget, public WidgetContainer
    {
    public:
        Menu(const string& lable = "", uint32 flags = 0);
        ~Menu();

        EDITOR_API void Render() override;

    private:
        bool m_isOpened = true;
    };
}