/**

    @file      LightMode.h
    @brief     
    @details   ~
    @author    Cool Guy
    @date      14.05.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <rengine\core\component\Component.h>

namespace rengine
{
    class LightMove : public Component
    {
    public:
        LightMove(const uuid& uuid);

        LightMove(const LightMove& component) = default;

        LightMove(LightMove&& component) = default;

        void Update() override;

        float GetSpeed() { return m_speed; }
        void SetSpeed(float val) { m_speed = val; }

    private:
        float m_speed = 1.0f;

        RTTR_ENABLE(Component);

        RTTR_REGISTRATION_FRIEND
    };
}