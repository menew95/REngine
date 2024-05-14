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

#include <rengine\rengine_dllexport.h>

#include <rengine\core\object\object.h>
#include <rengine\core\component\Component.h>
#include <rengine\core\ComponentManager.h>

#include <common\UUIDGenerator.h>

namespace rengine
{
    class ObjectFactory
    {
        DECLARE_SINGLETON_CLASS_EXPORT(RENGINE_API, ObjectFactory);

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

            auto _iter = m_objectsMap[_object->GetType()].find(_object->GetUUID());

            assert(_iter == m_objectsMap[_object->GetType()].end());

            m_objectsMap[_object->GetType()].insert(make_pair(_uuid, _object));

            if (std::is_base_of<Component, T>::value && _object) ReserveComponent(_object);

            return _object;
        }

        template<typename T>
        shared_ptr<T> CreateObject(uuid uuid)
        {
            static_assert(std::is_base_of<Object, T>::value, "class doesn't derive from the base");

            shared_ptr<T> _object = make_shared<T>(uuid);

            auto _iter = m_objectsMap[_object->GetType()].find(_object->GetUUID());

            assert(_iter == m_objectsMap[_object->GetType()].end());

            m_objectsMap[_object->GetType()].insert(make_pair(_object->GetUUID(), _object));

            if (std::is_base_of<Component, T>::value && _object) ReserveComponent(_object);

            return _object;
        }

        /**
            @brief 삭제 예약된 오브젝트를 삭제
        **/
        void Update();

        /**
            @brief  rttr를 이용하여 오브젝트 생성하고 반환
            @param  type  - rttr type name
            @param  uuid  - object uuid 
            @retval       - 생성된 오브젝트
        **/
        RENGINE_API shared_ptr<Object> CreateObject(string type, uuid uuid);

        /**
            @brief 오브젝트를 삭제 예약을 걸어둔다. 오브젝트 맵에 있는 오브젝트를 찾고, 삭제 대기 큐에 푸쉬, 만약 이미 삭제 예약 대기중이면 무시
            @param deleteObject - 삭제할 오브젝트
        **/
        RENGINE_API void ReserveDestroyObject(Object* deleteObject, float t = 0.f);

        /**
            @brief 오브젝트를 즉시 삭제한다.
            @param deleteObject - 삭제할 오브젝트
        **/
        RENGINE_API void DestroyImmediate(Object* deleteObject);

        RENGINE_API shared_ptr<Object> Find(uuid uuid);

        RENGINE_API const auto& FindObjectsOfType(tstring type)
        {
            return m_objectsMap[type];
        }

    private:
        /**
            @brief 종료전에 모든 오브젝트를 삭제
        **/
        void DeleteAllObjects();

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

        //vector<pair<float, Object*>> m_reserveDestroyObjectsQueue;
        //vector<pair<float, shared_ptr<Object>>> m_reserveDestroyObjectsQueue;
        //list<pair<float, shared_ptr<Object>>> m_reserveDestroyObjectsQueue;
        list<pair<float, Object*>> m_reserveDestroyObjectsQueue;
    };
}