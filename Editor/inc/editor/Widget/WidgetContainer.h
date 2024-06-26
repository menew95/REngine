﻿/**

    @file      WidgetContainer.h
    @brief     자식 위젯을 가지는 위젯
    @details   Tree node나 CollapsingHeader 같은 위젯은 하위에 다른 위젯들을 소유한다.
    @author    LWT
    @date      22.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#ifndef WIDGET_CONTAINER
#define WIDGET_CONTAINER

#include <editor\Widget\Widget.h>

namespace editor
{
    class WidgetContainer
    {
    public:
        WidgetContainer();

        virtual ~WidgetContainer();

        EDITOR_API virtual void RenderChild();
  
        EDITOR_API Widget* GetChild(const string& lable);

        EDITOR_API const vector<shared_ptr<Widget>>& GetChilds()
        {
            return m_childs;
        }
        
        template<typename TWidget, typename ... Args>
        TWidget* AddWidget(Args&&... args) requires std::derived_from<TWidget, Widget>;

        template <typename TWidget>
        TWidget* GetWidgetByLable(const string& lable) requires std::derived_from<TWidget, Widget>;

        void AddReservedWidgets();

        void DestroyReservedWidgets();

        void RemoveWidget(Widget* widget);

        void RemoveAllWidget();

        void RemoveAllWidgetReserved();

    protected:
        vector<shared_ptr<Widget>> m_childs;

        std::list<shared_ptr<Widget>> m_addReservedWidgets;
    };

    template<typename TWidget, typename ...Args>
    inline TWidget* WidgetContainer::AddWidget(Args && ...args)
        requires std::derived_from<TWidget, Widget>
    {
        m_addReservedWidgets.emplace_back(new TWidget(std::forward<Args>(args)...));

        return static_cast<TWidget*>(m_addReservedWidgets.back().get());
    }

    template<typename TWidget>
    inline TWidget* WidgetContainer::GetWidgetByLable(const string& lable)
        requires std::derived_from<TWidget, Widget>   
    {
        for (auto& widget : m_childs)
        {
            if (widget != nullptr && widget->GetLable() == lable)
                return static_cast<TWidget*>(widget.get());
        }

        for (auto& widget : m_addReservedWidgets)
        {
            if (widget != nullptr && widget->GetLable() == lable)
                return static_cast<TWidget*>(widget.get());
        }

        return nullptr;
    }
}
#endif // !1