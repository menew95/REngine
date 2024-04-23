/**

    @file      CollapsingHeader.h
    @brief     imgui collaps wrapper
    @details   collaps 할 수 있는 widget
    @author    LWT
    @date      22.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <editor\Widget\Widget.h>

#include <editor\Widget\WidgetContainer.h>

#include <rttr\registration>

namespace editor
{
    class CollapsingHeader : public Widget, public WidgetContainer
    {
    public:
        CollapsingHeader(const string& lable = "", uint32 flags = 0);
        ~CollapsingHeader();

        EDITOR_API void Render() override;

    private:
        bool m_bPopUpMenu = false;
    };
}