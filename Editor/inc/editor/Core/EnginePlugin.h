/**

    @file      EnginePlugin.h
    @brief     GameEngine과 plugin 작업을 해줄 class 
    @details   ~
    @author    LWT
    @date      13.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <common\common.h>
#include <common\singleton.h>

namespace rengine
{
    class SceneManager;
    class ResourceManager;
    class Application;
}

namespace editor
{
    class EnginePlugin
    {
        DECLARE_SINGLETON_CLASS(EnginePlugin)

    public:
        void Initialize(rengine::Application* app);

    private:
        
        rengine::Application* m_pApp;

        rengine::SceneManager* m_pSceneMgr;

        rengine::ResourceManager* m_pResourceMgr;
        
    };
}
