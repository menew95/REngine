/**

    @file      HierarchyView.h
    @brief     Scene의 계층구도를 보여주는 창
    @details   ~
    @author    LWT
    @date      16.10.2023

**/
#pragma once

#include <editor\GUI\View.h>

class GameObject
{
public:
    GameObject() = default;

    GameObject(std::string& name)
    : m_Name(name) {}

    GameObject(std::string name)
        : m_Name(name) {}

    ~GameObject() = default;

    std::string& GetName() { return m_Name; }

    auto& GetChilds() { return m_Childs; }
    void AddChilds(std::shared_ptr<GameObject> val) { m_Childs.push_back(val); }

private:
    std::string m_Name = "GameObject";

    std::vector<std::shared_ptr<GameObject>> m_Childs;
};

namespace editor
{
    class EDITOR_API HierarchyView : public View
    {
    public:
        HierarchyView();

        ~HierarchyView();

        void Begin();

        void Render();

        void End();

        void SetScene();
        
    private:
        void DrawTree(std::shared_ptr<GameObject> gameObj);

    };
}