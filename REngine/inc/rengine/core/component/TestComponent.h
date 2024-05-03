#pragma once

#include <common\common.h>
#include <common\math.h>

#include <rengine\core\component\Component.h>

namespace rengine
{
    class GameObject;

    class TestComponent : public Component, public enable_shared_from_this<TestComponent>
    {
    public:
        TestComponent(const uuid& uuid);

        TestComponent(const TestComponent& component) = default;

        TestComponent(TestComponent&& component) = default;

        virtual ~TestComponent();

    //private:
        tstring m_tstring = TEXT("");

        Vector2 m_vec2 = Vector2::Zero;
        Vector3 m_vec3 = Vector3::Zero;
        Vector4 m_vec4 = Vector4::Zero;

        uint32 m_uint32 = 0;
        int32 m_int32 = 0;

        float m_float = 0;
        double m_double = 0;


        enum class TestEnum
        {
            ENUM1,
            ENUM2,
            ENUM3,
        };

        struct TestStruct
        {
            TestStruct()
            {
                int a = 0;
            };

            ~TestStruct()
            {
                int a = 0;
            };

            Color _color = Color::Black;
            uint32 _enum = 0;

            Color GetColor() { return _color; }
            void SetColor(Color val) { _color = val; }

            uint32 GetEnum() { return _enum; }
            void SetEnum(uint32 val) { _enum = val; }
        };

        TestStruct* GetStruct() { return &m_struct; }
        void SetStruct(TestStruct* val) { m_struct = *val; }

        TestStruct m_struct;


        RTTR_ENABLE(Component)

        RTTR_REGISTRATION_FRIEND
    };
}
