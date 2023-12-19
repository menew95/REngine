/**

    @file      EditorView.h
    @brief     Editor View class
    @details   에디터 전체 뷰로 하위에 다른 뷰들이 도킹될 것이다.
    @author    LWT
    @date      19.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <editor\GUI\View.h>

namespace editor
{
    class EditorView : public View 
    {
    public:
        EditorView();

        EDITOR_API ~EditorView();

        void EDITOR_API Begin() override;

        void EDITOR_API Render() override;

        void EDITOR_API End() override;

    private:
        vector<View*> m_views;

        // test 중
        class GameView* m_pGameView;
        class InspectorView* m_pInspectorView;
        class HierarchyView* m_pHierarchyView;
        class ProjectView* m_pProjectView;
    };
}
