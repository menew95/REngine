/**

    @file      Container.h
    @brief     Widget Container의 widget 버전
    @details   ~
    @author    LWT
    @date      29.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <editor\Widget\WidgetContainer.h>

namespace editor
{
    class Container : public Widget, public WidgetContainer
    {
    public:
        Container();
        ~Container();

        void EDITOR_API Render() override;
        
    protected:
    };
}