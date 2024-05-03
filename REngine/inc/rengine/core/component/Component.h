/**

    @file      Component.h
    @brief     Component base
    @details   ~
    @author    LWT
    @date      16.10.2023

**/
#pragma once

#include <rengine\rengine_dllexport.h>

#include <rengine\core\object\object.h>

#include <common\Event.h>

namespace rengine
{
    class GameObject;
    class Transform;

    class Component : public Object
    {
    public:
       // Component(std::shared_ptr<GameObject>& gameObj);

        Component(const uuid& uuid, const tstring& type = TEXT("Component"));

        Component(const Component& component) = default;

        Component(Component&& component) = default;

        virtual ~Component();

        inline RENGINE_API std::weak_ptr<GameObject> GetGameObject() 
        { 
            return m_pGameObject; 
        }

        RENGINE_API std::shared_ptr<Transform> GetTransform();

        bool GetEnable() { return m_bEnable; }
        void SetEnable(bool value);

        bool GetIsActiveAndEnabled() { return m_bIsActiveAndEnabled; }
        void SetIsActiveAndEnabled(bool value) { m_bIsActiveAndEnabled = value; }

        /**
            @brief 객체 생성시 호출
        **/
        virtual void Awake() {}

        /**
            @brief 씬이 시작할때 한 번 호출
        **/
        virtual void Start() {}

        virtual void OnEnable() {}

        /**
            @brief 고정 프레임마다 호출
        **/
        virtual void FixedUpdate() {}

        /**
            @brief 매 프레임 호출
        **/
        virtual void Update() {}

        /**
            @brief 매 프레임 Update 이후 호출
        **/
        virtual void LateUpdate() {}

        virtual void OnDisable() {}

        /**
            @brief 객체가 파괴시 호출
        **/
        virtual void OnDestroy() {}
    
    protected:
        /**
            @brief 
        **/
        void PreDestroy() override;

        inline void SetGameObject(std::weak_ptr<GameObject> gameObject) { m_pGameObject = gameObject; }

        std::weak_ptr<GameObject> m_pGameObject;

        bool m_isAwake = false;

        bool m_bEnable = false;

        bool m_bIsActiveAndEnabled = false;

        friend class GameObject;

        RTTR_ENABLE(Object);

        RTTR_REGISTRATION_FRIEND
    };
}
