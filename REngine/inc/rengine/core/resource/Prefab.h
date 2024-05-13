/**

    @file      Prefab.h
    @brief     
    @details   ~
    @author    Cool Guy
    @date      20.03.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <rengine\core\resource\Resource.h>

namespace rengine
{
    class GameObject;

    class Prefab : public Resource
    {
    public:
        Prefab(const uuid& uuid);

        Prefab(const Prefab& mesh) = default;

        Prefab(Prefab&& mesh) = default;

        ResourceType GetResourceType() override
        {
            return ResourceType::PREFAB;
        }

    private:
        shared_ptr<GameObject> m_rootGameObject;

        // 인스턴스된 게임 오브젝트
        vector<shared_ptr<GameObject>> m_instanceObjects;
    };
}