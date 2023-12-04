/**

    @file      Transform.h
    @brief     트랜스폼 컴포넌트
    @author    LWT
    @date      4.12.2023

**/
#pragma once

#include <common\math.h>

#include <rengine\core\component\Component.h>


namespace rengine
{
    class GameObject;
    
    class RENGINE_API Transform : public Component, public enable_shared_from_this<Transform>
    {
    public:
        Transform(std::shared_ptr<GameObject>& gameObj);

        Transform(std::shared_ptr<GameObject>& gameObj, uuid uuid);

        Transform(std::shared_ptr<GameObject>& gameObj, uuid uuid, tstring name);

        Transform(const Transform& component) = default;

        Transform(Transform&& component) = default;

        virtual ~Transform();


        shared_ptr<Transform> GetParent() { return m_parent; }

        shared_ptr<Transform> GetChild(uint32 idx)
        { 
            if(m_childs.size() < idx) assert(false);

            return m_childs.at(idx);
        }

        void SetParent(shared_ptr<Transform> parent);

        void DetachParent();

        void AddChild(shared_ptr<Transform> child);

        void DetachChild(shared_ptr<Transform> child);

        void DetachChildren();

        math::Matrix GetWorld() { return m_world; }

        math::Matrix GetLocal() { return m_local; }

        math::Vector3 GetLocalPosition()
        {
            Vector3 _s, _t;
            Quaternion _r;

            if(!m_local.Decompose(_s, _r, _t))
                assert(false);

            return _t;
        }

        math::Quaternion GetLocalRotation()
        {
            Vector3 _s, _t;
            Quaternion _r;

            if (!m_local.Decompose(_s, _r, _t))
                assert(false);

            return _r;
        }

        math::Vector3 GetLocalEulerAngle()
        {
            Vector3 _s, _t;
            Quaternion _r;

            if (!m_local.Decompose(_s, _r, _t))
                assert(false);

            return _r.ToEuler();
        }

        math::Vector3 GetLocalScale()
        {
            Vector3 _s, _t;
            Quaternion _r;

            if (!m_local.Decompose(_s, _r, _t))
                assert(false);

            return _s;
        }


        void SetWolrd(Matrix& m);

        void SetLocal(Matrix& m);

    private:
        shared_ptr<Transform> m_parent;

        vector<shared_ptr<Transform>> m_childs;

        math::Matrix m_world;

        math::Matrix m_local;
    };
}
