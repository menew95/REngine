/**

    @file      sceneManager.h
    @brief     scene들을 관리하는 class
    @details   ~
    @author    LWT
    @date      13.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <common\singleton.h>

#include <rengine/rengine_dllexport.h>

namespace rengine
{
    class Scene;
    class GameObject;

    class SceneManager
    {
        DECLARE_SINGLETON_CLASS_EXPORT(RENGINE_API, SceneManager)
    
    public:
        /**
            @brief  Scene info txt로부터 씬들의 정보들을 읽어오고 생성함
            @retval  - 로드 성공 여부
        **/
        bool Initialize();

        RENGINE_API std::shared_ptr<Scene>& GetCurrentScene();

        RENGINE_API size_t GetSceneCount();

        /**
            @brief  Scene 배열로 부터 해당하는 인덱스의 Scene을 가져옴
            @param  idx - Scene 인덱스
            @retval     - Scene
        **/
        RENGINE_API std::shared_ptr<Scene> GetScene(size_t idx);

        /**
            @brief 해당 인덱스에 맞는 Scene을 로드함
            @param idx - 로드할 Scene의 인덱스 번호
        **/
        RENGINE_API void LoadScene(size_t idx);

        /**
            @brief  새로운 Scene 파일을 생성함
            @param  name - Scene 파일의 이름
            @retval      - 생성된 Scene
        **/
        static RENGINE_API std::shared_ptr<Scene> CreateScene(tstring name);

        /**
            @brief  현재 로드된 씬의 루트 게임오브젝트 배열를 반환
            @retval  - 현재 로드된 씬의 루트 게임오브젝트 리스트
        **/
        RENGINE_API std::vector<std::shared_ptr<GameObject>>& GetRootGameObjects()
        {
            return m_rootGameObjects;
        }

    private:
        std::shared_ptr<Scene> m_pCurrentScene;

        std::vector<std::shared_ptr<Scene>> m_scenes;

        std::vector<std::shared_ptr<GameObject>> m_rootGameObjects;
    };
}