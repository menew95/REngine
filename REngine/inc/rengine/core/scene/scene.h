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

    class RENGINE_API Scene : public Object
    {
    public:
        Scene();

        Scene(uuid uuid);

        Scene(uuid uuid, tstring name);

        Scene(uuid uuid, tstring name, tstring type);

        virtual ~Scene();

        std::shared_ptr<GameObject> FindGameObject(tstring& objectName);

        std::vector<std::shared_ptr<GameObject>>& GetGameObjects() { return m_GameObjects; }

    private:
        std::vector<std::shared_ptr<GameObject>> m_GameObjects;
    };
}