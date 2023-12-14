/**

    @file      GameObject.h
    @brief     컴포넌트를 소유하는 게임 오브젝트
    @author    LWT
    @date      16.10.2023

**/
#pragma once

#include <rengine\rengine_dllexport.h>
#include <rengine\core\object\object.h>

namespace rengine
{
    class Scene;
    class Component;
    class Transform;

    class RENGINE_API GameObject : public Object, public enable_shared_from_this<GameObject>
    {
    public:
        GameObject();

        GameObject(uuid uuid);

        GameObject(uuid uuid, tstring name);

        GameObject(const GameObject& object) = default;

        GameObject(GameObject&& object) = default;
        
        template<typename T>
        std::shared_ptr<T> AddComponent();

        template<typename T>
        std::shared_ptr<T> GetComponent();

        template<typename T>
        std::shared_ptr<T> GetComponentInChildren();

        template<typename T>
        std::shared_ptr<T> GetComponentInParent();

        template<typename T>
        std::vector<std::shared_ptr<T>> GetComponentsInChildren();

        template<typename T>
        std::vector<std::shared_ptr<T>> GetComponentsInParent();

        inline tstring& GetTag() { return m_Tag; }
        inline void SetTag(tstring& val) { m_Tag = val; }
        
        inline std::vector<std::shared_ptr<Component>>& GetComponents() { return m_Components; }

        inline std::shared_ptr<Transform>& GetTransform() { return m_pTransform; }

        inline std::weak_ptr<Scene>& GetScene() { return m_pScene; }

    private:
        std::vector<std::shared_ptr<Component>>  m_Components;
        
        std::shared_ptr<Transform> m_pTransform;
        
        std::weak_ptr<Scene> m_pScene;

        bool m_bActiveHierarchy = true;
        bool m_bActiveSelf = true;
        bool m_bIsStatic = false;

        tstring m_Tag;
    };
}

#include <rengine\core\object\GameObject.inl>