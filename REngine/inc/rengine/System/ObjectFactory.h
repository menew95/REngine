/**

    @file      ObjectFactory.h
    @brief     Object Factory class
    @details   Object의 shared_ptr를 생성 및 uuid을 키로 소유하고 관리한다.
    @author    LWT
    @date      28.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <common\singleton.h>

namespace rengine
{
    class Object;

    class ObjectFactory
    {
        DECLARE_SINGLETON_CLASS(ObjectFactory);

    public:
        template<typename T>
        shared_ptr<T> CreateObject()
        {
            return nullptr;
        }

        template<typename T>
        shared_ptr<T> CreateObject(uuid uuid)
        {
            return nullptr;
        }

    private:
        map<uuid, shared_ptr<Object>> m_objectMap;
    };
}