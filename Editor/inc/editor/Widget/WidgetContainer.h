/**

    @file      WidgetContainer.h
    @brief     자식 위젯을 가지는 위젯
    @details   Tree node나 CollapsingHeader 같은 위젯은 하위에 다른 위젯들을 소유한다.
    @author    LWT
    @date      22.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <editor\Widget\Widget.h>

namespace editor
{
    class WidgetContainer : public Widget
    {
    public:
        WidgetContainer(const string& id, uint32 flags = 0);

        virtual ~WidgetContainer();

        EDITOR_API virtual void Render();

        EDITOR_API void AddWidget(Widget* widget);
  
        EDITOR_API Widget* GetChild(const string& name);

        EDITOR_API vector<Widget*>& GetChilds()
        {
            return m_childs;
        }

        /*template <typename TWidget, typename ... Args>
        TWidget* AddWidget(Args&&... args);*/

    protected:
        vector<Widget*> m_childs;
    };

    /*template <typename TWidget, typename ... Args>
    TWidget* WidgetContainer::AddWidget(Args&&... args)
    {
        auto* _newWidget = WidgetManager::GetInstance()->CreateWidget<TWidget>(std::forward<Args>(args)...);

        m_childs.emplace_back(_newWidget);

        return _newWidget;
    }*/
}