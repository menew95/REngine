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

        void EDITOR_API Begin() override;

        void EDITOR_API Render() override;

        void EDITOR_API End() override;

        void EDITOR_API SetScene();
        
    private:
        void DrawTreeNode(rengine::GameObject* gameObj);

        void Drag(rengine::GameObject* gameObj);

        void Right(rengine::GameObject* gameObj);

        void DrawPopUp(bool& open, rengine::GameObject* gameObj);

        bool m_bIsOnDrag = false;

        bool m_bPopUpMenu = false;

        float m_fPrevYcursor = 0.0f;

        rengine::GameObject* m_pDragItem;
        rengine::GameObject* test;

        rengine::GameObject* m_pFocusGameObject = nullptr;
    };
}