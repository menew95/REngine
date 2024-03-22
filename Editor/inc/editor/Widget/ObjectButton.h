/**

    @file      ObjectButton.h
    @brief     object button class
    @details   editor에서 object property button의 기능을 담당하는 widget
    @author    LWT
    @date      21.03.2024
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <editor\Widget\Button.h>

namespace editor
{
    class ObjectButton : public Button
    {
    public:
        ObjectButton(string widgetName, rengine::Object* handler, rttr::property& prop, math::Vector2 size = { 0, 0 }, uint32 flags = 0);

        ~ObjectButton();

        virtual EDITOR_API void Render() override;

        void SetHandler(rengine::Object* handler) { m_pHandler = handler; }

    private:
        int FindObjects(ImGuiInputTextCallbackData* data);

        bool _test = false;

        ImVec2 m_rectSize;

        // component handler;
        rengine::Object* m_pHandler;

        // property
        rttr::property m_prop;
    };
}