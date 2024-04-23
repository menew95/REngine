/**

    @file      Popup.h
    @brief     
    @details   ~
    @author    LWT
    @date      23.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <editor\Widget\Widget.h>

#include <editor\Widget\WidgetContainer.h>

namespace editor
{
    class Popup : public Widget, public WidgetContainer
    {
    public:
        Popup(const string lable, uint32 flags = 0);

        void Render();
    };
}