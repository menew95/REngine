/**

    @file      material.h
    @brief     material class
    @details   ~
    @author    LWT
    @date      4.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <common\math.h>

#include <rengine\core\resource\Resource.h>

namespace graphics
{
    class MaterialBuffer;
}

namespace rengine
{
    class MaterialProperty
    {
    public:
        MaterialProperty() = default;
        ~MaterialProperty() = default;

    private:
        enum class PropType
        {
            Color   = 0,
            Vector  = 1,
            Float   = 3,
            Range   = 4,
            Texture = 5,
            Int     = 6,
        };

        /**
            @brief  material propery 타입
        **/
        PropType m_type;

        tstring m_name;

        /**
            @brief  material propery 타입에 따른 값
        **/
        math::Vector4 m_colorValue;
        math::Vector4 m_vectorValue;
        float m_floatValue;
        math::Vector2 m_rangeLimits;
        weak_ptr<class Texture> m_textureValue;
        int m_intValue;

    };

    class RENGINE_API Material : public Resource
    {
    public:
        Material(uuid uuid);

        Material(const Material& mat) = default;

        Material(Material&& mat) = default;

        virtual ~Material();

        ResourceType GetResourceType() override
        {
            return ResourceType::MATERIAL;
        };

        /**
            @brief  material 리소스의 메모리 로드
            @retval  - 로드 성공 여부
        **/
        bool LoadMemory();

        /**
            @brief  material 리소스의 메모리 해제
            @retval  - 해제 성공 여부
        **/
        bool UnLoadMemory();

        vector<MaterialProperty> GetProperties() { return m_properties; }
        void SetProperties(vector<MaterialProperty> val) { m_properties = val; }

    private:
        vector<MaterialProperty> m_properties;

        graphics::MaterialBuffer* m_pMaterialBuffer = nullptr;

        RTTR_ENABLE(Resource);

        RTTR_REGISTRATION_FRIEND;
    };
}