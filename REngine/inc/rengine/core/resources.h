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
    class Model;

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

        template<>
        RENGINE_API shared_ptr<Model> Load(tstring path);

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

        template<>
        RENGINE_API shared_ptr<Model> GetResource(uuid uuid);

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

        template<>
        RENGINE_API shared_ptr<Model> CreateResource();
        /**
            @brief  새로운 비어있는 리소스를 생성후 uuid를 부여 후 생성된 리소스 반환
            @tparam T    - 생성할 리소스 타입
            @param  uuid - 설정할 uuid
            @retval      - 생성된 리소스
        **/
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

        template<>
        RENGINE_API shared_ptr<Model> CreateResource(uuid uuid);

        /**
            @brief  path에 있는 에셋을 이용하여 리소스를 생성 하고 uuid를 부여 후 반환
            @tparam T    - 생성할 리소스 타입
            @param  uuid - 설정할 uuid
            @param  path - 로드할 리소스 경로
            @retval      - 생성된 리소스
        **/
        template<class T>
        shared_ptr<T> CreateResource(uuid uuid, const tstring& path)
        {
            static_assert(std::is_base_of<Resource, T>::value, "class doesn't derive from the base");

            return nullptr;
        }

        template<>
        RENGINE_API shared_ptr<Mesh> CreateResource(uuid uuid, const tstring& path);

        template<>
        RENGINE_API shared_ptr<Material> CreateResource(uuid uuid, const tstring& path);

        template<>
        RENGINE_API shared_ptr<Texture> CreateResource(uuid uuid, const tstring& path);

        template<>
        RENGINE_API shared_ptr<AnimationClip> CreateResource(uuid uuid, const tstring& path);

        template<>
        RENGINE_API shared_ptr<Model> CreateResource(uuid uuid, const tstring& path);

        RENGINE_API bool DeleteResource(const shared_ptr<Resource>& res);

    private:
        void LoadAllAsset();

        void UnloadAllResource();

        /**
            @brief 머티리얼의 경우 텍스처로드가 먼저 되어있어야 로드가 되기때문에 로드 순서가 필요함
            @param path   - 
            @param gather - 
        **/
        void GatherAsset(const tstring& path, map<tstring, vector<tstring>>& gather);

        void LoadAssets(map<tstring, vector<tstring>>& gather);

        void LoadAsset(const tstring& path);

        map<uuid, shared_ptr<Mesh>>     m_meshMap;

        map<uuid, shared_ptr<Texture>>  m_textureMap;

        map<uuid, shared_ptr<Material>> m_materialMap;

        map<uuid, shared_ptr<AnimationClip>> m_animationClipMap;

        map<uuid, shared_ptr<Model>> m_modelMap;
    };
}