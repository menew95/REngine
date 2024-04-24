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
    class AnimationClip;
    class AnimationController;
    class AudioClip;

    class Resources
    {
        DECLARE_SINGLETON_CLASS_EXPORT(RENGINE_API, Resources);

    public:
        bool Initialize();

        template<class T>
        shared_ptr<T> Load(tstring path)
        {
            static_assert(std::is_base_of<Resource, T>::value, "class doesn't derive from the base");

            return nullptr;
        }

        template<>
        RENGINE_API shared_ptr<Mesh> Load(tstring path);

        template<>
        RENGINE_API shared_ptr<Material> Load(tstring path);

        template<>
        RENGINE_API shared_ptr<Texture> Load(tstring path);

        template<>
        RENGINE_API shared_ptr<AnimationClip> Load(tstring path);

        template<class T>
        shared_ptr<T> GetResource(uuid uuid)
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

        template<>
        RENGINE_API shared_ptr<AnimationClip> GetResource(uuid uuid);

        template<class T>
        shared_ptr<T> CreateResource()
        {
            static_assert(std::is_base_of<Resource, T>::value, "class doesn't derive from the base");

            return nullptr;
        }

        template<>
        RENGINE_API shared_ptr<Mesh> CreateResource();

        template<>
        RENGINE_API shared_ptr<Material> CreateResource();

        template<>
        RENGINE_API shared_ptr<Texture> CreateResource();

        template<>
        RENGINE_API shared_ptr<AnimationClip> CreateResource();

        template<class T>
        shared_ptr<T> CreateResource(uuid uuid)
        {
            static_assert(std::is_base_of<Resource, T>::value, "class doesn't derive from the base");

            return nullptr;
        }

        template<>
        RENGINE_API shared_ptr<Mesh> CreateResource(uuid uuid);

        template<>
        RENGINE_API shared_ptr<Material> CreateResource(uuid uuid);

        template<>
        RENGINE_API shared_ptr<Texture> CreateResource(uuid uuid);

        template<>
        RENGINE_API shared_ptr<AnimationClip> CreateResource(uuid uuid);

        RENGINE_API bool DeleteResource(const shared_ptr<Resource>& res);

    private:
        void LoadAllAsset();

        void UnloadAllResource();

        void LoadAsset(const tstring& path);

        map<uuid, shared_ptr<Mesh>>     m_meshMap;

        map<uuid, shared_ptr<Texture>>  m_textureMap;

        map<uuid, shared_ptr<Material>> m_materialMap;

        map<uuid, shared_ptr<AnimationClip>> m_animationClipMap;
    };
}