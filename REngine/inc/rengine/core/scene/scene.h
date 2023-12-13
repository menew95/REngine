/**

    @file      scene.h
    @brief     게임오브젝트들을 소유하고 있는 씬
    @details   ~
    @author    LWT
    @date      16.10.2023

**/
#pragma once

#include <common\common.h>

#include <rengine\core\object\object.h>

#include <rengine\rengine_dllexport.h>

namespace rengine
{
    class GameObject;

    class Scene : public Object
    {
    public:
        Scene();

        Scene(uuid uuid);

        Scene(uuid uuid, tstring name);

        Scene(uuid uuid, tstring name, tstring type);

        virtual ~Scene();

        RENGINE_API std::shared_ptr<GameObject> FindGameObject(tstring& objectName);

        RENGINE_API std::vector<std::shared_ptr<GameObject>>& GetRootGameObjects() { return m_RootGameObjects; }

        RENGINE_API void AddRootGameObject(std::shared_ptr<GameObject> go)
        {
            m_RootGameObjects.push_back(go);
        }

    private:
        std::vector<std::shared_ptr<GameObject>> m_RootGameObjects;
    };
}