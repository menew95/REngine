﻿/**

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

    class RENGINE_API GameObject : public Object
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
        
    private:
        std::vector<std::shared_ptr<Component>>  m_Components;
        
        std::weak_ptr<Transform> m_pTransform;
        
        std::shared_ptr<Scene> m_pScene;

        bool m_bActiveHierarchy = true;
        bool m_bActiveSelf = true;
        bool m_bIsStatic = false;

        tstring m_Tag;
    };

    template<typename T>
    inline std::shared_ptr<T> GameObject::AddComponent()
    {
        return std::shared_ptr<T>();
    }

    template<typename T>
    inline std::shared_ptr<T> GameObject::GetComponent()
    {
        return std::shared_ptr<T>();
    }
}