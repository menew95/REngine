/**

    @file      Behaviour.h
    @brief     
    @details   ~
    @author    LWT
    @date      3.05.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <rengine\core\component\Component.h>

namespace rengine
{
    class Behaviour : public Component
    {
    public:
        Behaviour(const uuid& uuid, const tstring& type = TEXT("Behaviour"));

        Behaviour(const Behaviour& component) = default;

        Behaviour(Behaviour&& component) = default;

        virtual ~Behaviour();

        bool GetEnable() { return m_enable; }
        void SetEnable(bool value);

        bool GetIsActiveAndEnabled() { return m_isActiveAndEnabled; }
        void SetIsActiveAndEnabled(bool value) { m_isActiveAndEnabled = value; }

        /**
            @brief 게임 오브젝트가 씬에서 활성화 상태이고 객체가 활성화되면 호출
        **/
        virtual void OnEnable() {}

        /**
            @brief 객체가 비활성화되면 호출
        **/
        virtual void OnDisable() {}

    protected:

        bool m_enable = false;

        bool m_isActiveAndEnabled = false;

        friend class GameObject;

        RTTR_ENABLE(Component);

        RTTR_REGISTRATION_FRIEND
    };
}
