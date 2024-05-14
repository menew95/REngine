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

#include <common\math.h>

namespace rengine
{
    class LightMove : public Component
    {
    public:
        LightMove(const uuid& uuid);

        LightMove(const LightMove& component) = default;

        LightMove(LightMove&& component) = default;

        void Start() override;

        void Update() override;

        float GetSpeed() { return m_speed; }
        void SetSpeed(float val) { m_speed = val; }

        const Vector3& GetPos1() { return position[0]; }
        void SetPos1(const Vector3& val) { position[0] = val; }
        const Vector3& GetPos2() { return position[1]; }
        void SetPos2(const Vector3& val) { position[1] = val; }
        const Vector3& GetPos3() { return position[2]; }
        void SetPos3(const Vector3& val) { position[2] = val; }
        const Vector3& GetPos4() { return position[3]; }
        void SetPos4(const Vector3& val) { position[3] = val; }

    private:
        float m_speed = 1.0f;

        uint32 m_lightType = 0;

        int current = 0;

        Vector3 position[4];

        RTTR_ENABLE(Component);

        RTTR_REGISTRATION_FRIEND
    };
}