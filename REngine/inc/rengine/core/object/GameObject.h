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

    class RENGINE_API GameObject : public Object, public enable_shared_from_this<GameObject>
    {
    public:
        
        static shared_ptr<GameObject> Instantiate(shared_ptr<Transform> parent = nullptr);

        //GameObject();

        GameObject(uuid uuid);

        //GameObject(uuid uuid, tstring name = TEXT("Game Object"));

        GameObject(const GameObject& object) = default;

        GameObject(GameObject&& object) = default;

        ~GameObject();

        template<typename T>
        std::shared_ptr<T> AddComponent();

        void AddComponent(string type);

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

        inline std::vector<std::weak_ptr<Component>> GetComponents() { return m_Components; }
        inline std::shared_ptr<Transform> GetTransform() { return m_pTransform.lock(); }

        inline std::shared_ptr<Scene> GetScene() { return m_pScene.lock(); }
        //inline void SetScene(const std::shared_ptr<Scene>& scene);

        inline void SetComponents(std::vector<std::weak_ptr<Component>> comps);
        // 일단 GetTransform을 기존에 shared_ptr<Transform> ref를 받아서 Instantiate 할시에 설정이 가능했으나 지금은 안됨
        // 임시로 해둠 추후 변경 될 수 있음
        inline void SetTransform(const shared_ptr<Transform>& trans) { m_pTransform = trans;}
        
        inline bool GetActiveSelf()         { return m_bActiveSelf; }
        inline bool GetActiveInHierarchy()  { return m_bActiveInHierarchy; }
        inline bool GetIsStatic()           { return m_bIsStatic; }
        inline tstring GetTag()             { return m_tag; }
        inline uint32 GetLayer()            { return m_layer; }

        inline void SetActive(bool val);
        inline void SetIsStatic(bool val)           { m_bIsStatic = val; }
        inline void SetTag(tstring val)             { m_tag = val; }
        inline void SetLayer(uint32 val)            { m_layer = val; }

        shared_ptr<GameObject> GetShared() { return shared_from_this(); }

    private:
        /**
            @brief 등록된 컴포넌트들을 삭제.
        **/
        void PreDestroy() override;

        bool RemoveComponent(tstring type);

        /**
            @brief  컴포넌트가 삭제 될때 호출됨 게임 오브젝트에 등록된 컴포넌트 목록으로 부터 삭제
            @param  component - 삭제된 컴포넌트
        **/
        void RemoveComponent(const shared_ptr<Component>& component);

        void RemoveComponent(Component* uuid);

        /**
            @brief 모든 컴포넌트를 삭제 대기에 걸고 자식 게임 오브젝트 또한 삭제 대기한다.
        **/
        void DestroyGameObject();

        std::vector<std::weak_ptr<Component>>  m_Components;
        
        std::weak_ptr<Transform> m_pTransform;
        
        std::weak_ptr<Scene> m_pScene;

        bool m_bActiveInHierarchy = true;
        bool m_bActiveSelf = true;
        bool m_bIsStatic = false;

        tstring m_tag;

        uint32 m_layer = 0;
        
        friend class ObjectFactory;

        friend class ComponentManager;

        friend class Component;

        RTTR_ENABLE(rengine::Object);

        RTTR_REGISTRATION_FRIEND
    };
}

#include <rengine\core\object\GameObject.inl>