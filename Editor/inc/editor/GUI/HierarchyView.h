/**

    @file      HierarchyView.h
    @brief     Scene의 계층구도를 보여주는 창
    @details   ~
    @author    LWT
    @date      16.10.2023

**/
#pragma once

#include <editor\GUI\View.h>

namespace rengine
{
    class GameObject;
}

namespace editor
{
    class HierarchyView : public View
    {
    public:
        HierarchyView();

        EDITOR_API ~HierarchyView();

        void EDITOR_API Begin();

        void EDITOR_API Render();

        void EDITOR_API End();

        void EDITOR_API SetScene();
        
    private:
        void DrawTreeNode(rengine::GameObject* gameObj);

        void Drag(rengine::GameObject* gameObj);

        void Right();

        bool m_bIsOnDrag = false;

        float m_fPrevYcursor = 0.0f;

        rengine::GameObject* m_pDragItem;

        rengine::GameObject* m_pFocusGameObject = nullptr;
    };
}