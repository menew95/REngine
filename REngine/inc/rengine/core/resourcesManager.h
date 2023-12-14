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

namespace rengine
{
    class Resources
    {
        template<class T>
        static shared_ptr<T> Load(tstring path);

        template<>
        static std::shared_ptr<class Scene> Load(tstring path);
    };

    template<class T>
    inline shared_ptr<T> Resources::Load(tstring path)
    {
        assert(false);
        return nullptr;
    }


}