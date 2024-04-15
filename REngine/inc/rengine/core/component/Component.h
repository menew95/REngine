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

namespace rengine
{
    class GameObject;
    class Transform;

    class Component : public Object
    {
    public:
       // Component(std::shared_ptr<GameObject>& gameObj);

        Component(uuid uuid, tstring type = TEXT("Component"));

        Component(const Component& component) = default;

        Component(Component&& component) = default;

        virtual ~Component();

        inline RENGINE_API std::weak_ptr<GameObject> GetGameObject()
        { 
            return m_pGameObject; 
        }

        inline RENGINE_API auto GetTransform() { return m_pTransform; }
        inline RENGINE_API void SetTransform(auto transform) { m_pTransform = transform; }

        bool GetEnable() { return m_bEnable; }
        void SetEnable(bool value);

        bool GetIsActiveAndEnabled() { return m_bIsActiveAndEnabled; }
        void SetIsActiveAndEnabled(bool value) { m_bIsActiveAndEnabled = value; }

        virtual void Awake() {}

        virtual void OnEnable() {}

        virtual void Reset() {}

        virtual void Start() {}

        virtual void FixedUpdate() {}

        virtual void Update() {}

        virtual void LateUpdate() {}

        virtual void OnDisable() {}

        virtual void OnDestroy() {}

    protected:
        inline void SetGameObject(std::weak_ptr<GameObject> gameObject) { m_pGameObject = gameObject; }

        std::weak_ptr<GameObject> m_pGameObject;
        
        std::weak_ptr<Transform> m_pTransform;

        bool m_bEnable = false;

        bool m_bIsActiveAndEnabled = false;

        friend class GameObject;

        RTTR_ENABLE(Object);

        RTTR_REGISTRATION_FRIEND
    };
}
