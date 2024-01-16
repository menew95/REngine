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
        //Scene();

        Scene(uuid uuid);

        //Scene(uuid uuid, tstring name);

        //Scene(uuid uuid, tstring name, tstring type);

        virtual ~Scene();

        RENGINE_API std::shared_ptr<GameObject> FindGameObject(tstring& objectName);

        RENGINE_API std::vector<std::shared_ptr<GameObject>>& GetRootGameObjects() { return m_rootGameObjects; }

        RENGINE_API void AddRootGameObject(std::shared_ptr<GameObject> go)
        {
            m_rootGameObjects.push_back(go);
        }

        RENGINE_API bool GetIsDirty() { return m_bIsDirty; }
        RENGINE_API bool GetIsLoaded() { return m_bIsLoaded; }
        RENGINE_API tstring GetPath() { return m_path; }

        RENGINE_API void SetIsDirty(bool value) { m_bIsDirty = value; }
        RENGINE_API void SetIsLoaded(bool value) { m_bIsLoaded = value; }
        RENGINE_API void SetPath(tstring value) { m_path = value; }

    private:
        bool m_bIsDirty = false;

        bool m_bIsLoaded = false;

        tstring m_path;

        std::vector<std::shared_ptr<GameObject>> m_rootGameObjects;

        RTTR_ENABLE(Object);

        RTTR_REGISTRATION_FRIEND;
    };
}