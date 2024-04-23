/**

    @file      Document.h
    @brief     document class
    @details   하위에 도킹된 view들을 소유한다.
    @author    LWT
    @date      3.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <editor\GUI\GUI.h>

#include <editor\Editor_dllexport.h>

namespace editor
{
    class Document : public GUI
    {
    public:
        Document(std::string name);

        virtual EDITOR_API ~Document();

        virtual EDITOR_API void Begin();

        virtual EDITOR_API void Render();

        virtual EDITOR_API void End();

        template <typename TView, typename ...Args>
        TView* AddView(Args&&... args) requires std::derived_from<TView, class View>;

        template <typename TView>
        TView* GetView() requires std::derived_from<TView, class View>;

        View* GetView(const string& name);

    protected:
        void SetDockingSpace();
        
        vector<shared_ptr<class View>> m_childs;

        std::string m_documentName;

        bool m_isDockingEnable = true;
    };

    template <typename TView, typename ...Args>
    TView* Document::AddView(Args&&... args) requires std::derived_from<TView, class View>
    {
        m_childs.emplace_back(std::make_shared<TView>(args...));

        return static_cast<TView*>(m_childs.back().get());
    }

    template <typename TView>
    TView* Document::GetView() requires std::derived_from<TView, class View>
    {
        for (auto& _view : m_childs)
        {
            if (std::dynamic_pointer_cast<TView>(_view) != nullptr)
                return static_cast<TView*>(_view.get());
        }

        return nullptr;
    }
}