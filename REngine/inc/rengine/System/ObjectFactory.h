﻿/**

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

#include <rengine\core\object\object.h>
#include <rengine\core\component\Component.h>
#include <rengine\core\ComponentManager.h>

#include <common\UUIDGenerator.h>

namespace rengine
{
    class ObjectFactory
    {
        DECLARE_SINGLETON_CLASS(ObjectFactory);

    public:

        template<typename T>
        shared_ptr<T> Instantiate()
        {
            static_assert(std::is_base_of<Object, T>::value, "class doesn't derive from the base");

        }

        template<typename T>
        shared_ptr<T> CreateObject()
        {
            static_assert(std::is_base_of<Object, T>::value, "class doesn't derive from the base");

            uuid _uuid = UUIDGenerator::Generate();

            shared_ptr<T> _object = make_shared<T>(_uuid);

            m_objectsMap[_object->GetType()].insert(make_pair(_uuid, _object));

            if (std::is_base_of<Component, T>::value && _object) ReserveComponent(_object);

            return _object;
        }

        template<typename T>
        shared_ptr<T> CreateObject(uuid uuid)
        {
            static_assert(std::is_base_of<Object, T>::value, "class doesn't derive from the base");

            shared_ptr<T> _object = make_shared<T>(uuid);

            m_objectsMap[_object->GetType()].insert(make_pair(_object->GetUUID(), _object));

            if (std::is_base_of<Component, T>::value && _object) ReserveComponent(_object);

            return _object;
        }

    private:
        
        //template<class T, bool condition> struct ReserveComponent_Impl;

        //template<class T>
        //struct ReserveComponent_Impl<T, true>
        //{
        //    static void Reserve(shared_ptr<T> component)
        //    {
        //        ComponentManager::GetInstance()->ReserveAddComponent(component);
        //    }
        //};

        //template<class T>
        //struct ReserveComponent_Impl<T, false>
        //{
        //    static void Reserve(shared_ptr<T> component)
        //    {
        //        // 컴포넌트가 아님
        //    }
        //};

        //template< class T >
        //struct ReserveComponent : public ReserveComponent_Impl<T, std::is_base_of_v<Component, T>> {};

        template<typename T>
        void ReserveComponent(shared_ptr<T> component) requires (!is_base_of_v<Component, T>)
        {
            // 컴포넌트가 아님
        }

        template<typename T>
        void ReserveComponent(shared_ptr<T> component) requires is_base_of_v<Component, T>
        {
            ComponentManager::GetInstance()->ReserveAddComponent(component);
        }

        map<tstring, map<uuid, shared_ptr<Object>>> m_objectsMap;
    };
}