/**

    @file      GraphicsSetting.h
    @brief     graphics setting view
    @details   그래픽스 엔진에 전달할 
    @author    LWT
    @date      17.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <editor\GUI\View.h>

#include <rttr/array_range.h>

namespace editor
{
    class GraphicsSetting : public View
    {
    public:
        GraphicsSetting();
        ~GraphicsSetting();

        void Begin() override;

        void Render() override;
        
        void End() override;
    
    private:
        void DrawSettings(class WidgetContainer* root, const rttr::array_range<rttr::property>& properties, const rttr::instance& obj);

        void DrawStruct(class WidgetContainer* root, const rttr::property& property, const rttr::instance& obj);

        class CollapsingHeader* m_widget;
    };
}