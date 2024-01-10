/**

    @file      resourcesManager.h
    @brief     assets 폴더의 리소스를 관리
    @details   ~
    @author    LWT
    @date      14.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <common\singleton.h>

#include <rengine\core\resource\Resource.h>

#include <rengine\rengine_dllexport.h>

namespace rengine
{
    class Mesh;
    class Texture;
    class Material;

    class Resources
    {
        DECLARE_SINGLETON_CLASS(Resources);

    public:
        bool Initialize();

        template<class T>
        RENGINE_API static shared_ptr<T> Load(tstring path)
        {
            static_assert(std::is_base_of<Resource, T>::value, "class doesn't derive from the base");

            return nullptr;
        }

        template<>
        RENGINE_API static shared_ptr<Mesh> Load(tstring path);

        template<>
        RENGINE_API static shared_ptr<Material> Load(tstring path);

        template<>
        RENGINE_API static shared_ptr<Texture> Load(tstring path);

        template<class T>
        RENGINE_API shared_ptr<T> GetResource(uuid uuid)
        {
            static_assert(std::is_base_of<Resource, T>::value, "class doesn't derive from the base");

            return nullptr;
        }

        template<>
        RENGINE_API shared_ptr<Mesh> GetResource(uuid uuid);

        template<>
        RENGINE_API shared_ptr<Material> GetResource(uuid uuid);

        template<>
        RENGINE_API shared_ptr<Texture> GetResource(uuid uuid);

    private:
        void LoadAllAsset();

        void LoadAssert(const tstring& path);

        map<uuid, shared_ptr<Mesh>>     m_meshMap;

        map<uuid, shared_ptr<Texture>>  m_textureMap;

        map<uuid, shared_ptr<Material>> m_materialMap;
    };
}