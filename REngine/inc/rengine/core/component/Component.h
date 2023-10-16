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

    class RENGINE_API Component : public Object
    {
    public:
        Component(std::weak_ptr<GameObject>& gameObj);

        Component(std::weak_ptr<GameObject>& gameObj, uuid uuid);

        Component(std::weak_ptr<GameObject>& gameObj, uuid uuid, tstring name);

        Component(const Component& component) = default;

        Component(Component&& component) = default;

        inline auto GetGameObject() { return m_pGameObject; }

        inline auto GetTransform() { return m_pTransform; }

    private:
        std::weak_ptr<GameObject> m_pGameObject;
        
        std::weak_ptr<Transform> m_pTransform;
    };
}
