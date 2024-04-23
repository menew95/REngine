﻿/**

    @file      ComboBox.h
    @brief     imgui combo box widget wrapper
    @details   enum 값 등을 관리하는 widget
    @author    LWT
    @date      3.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <editor\Widget\WidgetData.h>

namespace editor
{
    class ComboBox : public WidgetData<int>
    {
    public:
        ComboBox(const string& lable, const rttr::property& prop, uint32 flags = 0);

        ~ComboBox();

    private:
        void Draw() override;

    protected:
        vector<string> m_items;
    };
}