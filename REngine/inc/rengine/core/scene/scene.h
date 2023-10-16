/**

    @file      scene.h
    @brief     게임오브젝트들을 소유하고 있는 씬
    @details   ~
    @author    LWT
    @date      16.10.2023

**/
#pragma once

#include <common\common.h>

#include <rengine\rengine_dllexport.h>

namespace rengine
{
    class GameObject;

    class RENGINE_API Scene
    {
    public:
        Scene();
        ~Scene();

        std::shared_ptr<GameObject> FindGameObject(tstring& objectName);

    private:
        std::vector<std::shared_ptr<GameObject>> m_GameObjects;
    };
}