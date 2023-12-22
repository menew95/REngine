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
    
    class Transform : public Component, public enable_shared_from_this<Transform>
    {
    public:
        Transform(std::shared_ptr<GameObject>& gameObj);

        Transform(std::shared_ptr<GameObject>& gameObj, uuid uuid);

        Transform(const Transform& component) = default;

        Transform(Transform&& component) = default;

        virtual ~Transform();

        RENGINE_API shared_ptr<Transform> GetParent() { return m_parent.lock(); }

        RENGINE_API shared_ptr<Transform> GetChild(uint idx)
        { 
            if(m_childs.size() < idx) assert(false);

            return m_childs.at(idx);
        }

        inline RENGINE_API uint32 GetChildSize()
        {
            return static_cast<uint32>(m_childs.size());
        }

        RENGINE_API void SetParent(shared_ptr<Transform> parent);

        RENGINE_API void DetachParent();

        RENGINE_API void AddChild(shared_ptr<Transform> child);

        RENGINE_API void DetachChild(shared_ptr<Transform> child);

        RENGINE_API void DetachChildren();

        RENGINE_API math::Matrix GetWorld() { return m_world; }

        RENGINE_API math::Matrix GetLocal() { return m_local; }

        RENGINE_API math::Vector3 GetLocalPosition()
        {
            Vector3 _s, _t;
            Quaternion _r;

            if(!m_local.Decompose(_s, _r, _t))
                assert(false);

            return _t;
        }

        RENGINE_API math::Quaternion GetLocalRotation()
        {
            Vector3 _s, _t;
            Quaternion _r;

            if (!m_local.Decompose(_s, _r, _t))
                assert(false);

            return _r;
        }

        RENGINE_API math::Vector3 GetLocalEulerAngle()
        {
            Vector3 _s, _t;
            Quaternion _r;

            if (!m_local.Decompose(_s, _r, _t))
                assert(false);

            return _r.ToEuler();
        }

        RENGINE_API math::Vector3 GetLocalScale()
        {
            Vector3 _s, _t;
            Quaternion _r;

            if (!m_local.Decompose(_s, _r, _t))
                assert(false);

            return _s;
        }

        RENGINE_API void SetWorld(Matrix m);

        RENGINE_API void SetLocal(Matrix m);

    private:
        weak_ptr<Transform> m_parent;

        vector<shared_ptr<Transform>> m_childs;

        bool m_bIsDirty = true;

        math::Matrix m_world = math::Matrix::Identity;

        math::Matrix m_local = math::Matrix::Identity;

        RTTR_ENABLE(Component)

        RTTR_REGISTRATION_FRIEND
    };
}
