﻿/**

    @file      Camera.h
    @brief     Camera component
    @details   ~
    @author    LWT
    @date      4.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <rengine\core\component\Component.h>

namespace rengine
{
    class RENGINE_API Camera : public Component, public enable_shared_from_this<Camera>
    {
    public:
        Camera(std::shared_ptr<GameObject>& gameObj);

        Camera(std::shared_ptr<GameObject>& gameObj, uuid uuid);

        Camera(const Camera& filter) = default;

        Camera(Camera&& filter) = default;

        virtual ~Camera();

        float   GetNear() { return m_fNear; }
        void    SetNear(float val) { m_fNear = val; }

        float   GetFar() { return m_fFar; }
        void    SetFar(float val) { m_fFar = val; }

        float   GetFOV() { return m_fFieldOfView; }
        void    SetFOV(float val) { m_fFieldOfView = val; }

        float   GetAspect() { return m_fAspectRadio; }
        void    SetAspect(float val) { m_fAspectRadio = val; }

    private:
        float m_fNear = 0.1f;
        float m_fFar = 1000.0f;
        float m_fFieldOfView = 90;
        float m_fAspectRadio = 1.0f;

        static std::weak_ptr<Camera> m_MainCamera;

        RTTR_ENABLE(Component)

        RTTR_REGISTRATION_FRIEND
    };
}