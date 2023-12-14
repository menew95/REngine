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

    class SceneManager
    {
        DECLARE_SINGLETON_CLASS_EXPORT(RENGINE_API, SceneManager)
    
    public:
        bool Initialize();

        RENGINE_API std::shared_ptr<Scene>& GetCurrentScene();

        RENGINE_API size_t GetSceneCount();

        RENGINE_API std::shared_ptr<Scene> GetScene(size_t idx);

        RENGINE_API void LoadScene(size_t idx);

    private:

        std::shared_ptr<Scene> m_pCurrentScene;

        std::vector<std::shared_ptr<Scene>> m_Scenes;
    };
}