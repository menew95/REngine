﻿/**

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
        Component(std::shared_ptr<GameObject>& gameObj);

        Component(std::shared_ptr<GameObject>& gameObj, uuid uuid);

        Component(const Component& component) = default;

        Component(Component&& component) = default;

        virtual ~Component();

        inline RENGINE_API auto GetGameObject() 
        { 
            return m_pGameObject; 
        }

        inline RENGINE_API auto GetTransform() { return m_pTransform; }

        bool GetEnable() { return m_bEnable; }
        void SetEnable(bool value);

        bool GetIsActiveAndEnabled() { return m_bIsActiveAndEnabled; }
        void SetIsActiveAndEnabled(bool value) { m_bIsActiveAndEnabled = value; }

    private:
        std::shared_ptr<GameObject> m_pGameObject;
        
        std::weak_ptr<Transform> m_pTransform;

        bool m_bEnable = true;

        bool m_bIsActiveAndEnabled = true;

        RTTR_ENABLE(Object);

        RTTR_REGISTRATION_FRIEND
    };
}