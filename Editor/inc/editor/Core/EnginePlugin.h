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

#include <common\AppBase.h>

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
        void Initialize(WindowInfo& winInfo);

        inline AppBase* GetEngine()
        {
            return m_pRengine;
        }

    private:
        AppBase* m_pRengine;
    };
}
