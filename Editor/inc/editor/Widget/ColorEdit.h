/**

    @file      ColorEdit.h
    @brief     imgui color edit4 wrapper
    @details   ~
    @author    LWT
    @date      3.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <editor\Widget\WidgetData.h>

namespace rengine
{
    class Object;
}

namespace editor
{
    class ColorEdit : public WidgetData<Color>
    {
    public:
        ColorEdit(const string& id, rttr::instance& obj, rttr::property& prop, uint32 flags = 0);

        ~ColorEdit();

    private:
        void Draw() override;

    protected:
        float m_colorBuf[4] = { .0f, .0f, .0f, .0f };
    };
}