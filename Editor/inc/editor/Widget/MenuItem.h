/**

    @file      MenuItem.h
    @brief     imgui menu item wrapper
    @details   menu widget의 자식으로 클릭시 등록된 이벤트를 invoke
    @author    LWT
    @date      22.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <editor\Widget\Widget.h>

namespace rengine
{
    class Object;
}

namespace editor
{
    class MenuItem : public Widget
    {
    public:
        MenuItem(const string& id, const string& shortcut, bool isSelectedable, bool m_isSelected, uint32 flags = 0);

        ~MenuItem();

    private:
        void Render() override;

    protected:
        string m_shortcut;

        bool m_isSelectedable = false;
        bool m_isSelected = false;

        bool m_isEnable = true;

        function<void()> m_clickedEvent;
    };
}