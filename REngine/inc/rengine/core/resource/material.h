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
        enum class PropType
        {
            Color = 0,
            Vector = 1,
            Float = 2,
            Range = 3,
            Texture = 4,
            Int = 5,
        };

        MaterialProperty() = default;
        ~MaterialProperty() = default;

        MaterialProperty(tstring name, math::Color val)
            : m_type(PropType::Color)
            , m_name(name)
            , m_colorValue(val)
        {}

        MaterialProperty(tstring name, math::Vector4 val)
            : m_type(PropType::Vector)
            , m_name(name)
            , m_vectorValue(val)
        {}

        MaterialProperty(tstring name, float val)
            : m_type(PropType::Float)
            , m_name(name)
            , m_floatValue(val)
        {}

        MaterialProperty(tstring name, math::Vector2 val)
            : m_type(PropType::Range)
            , m_name(name)
            , m_rangeLimits(val)
        {}

        MaterialProperty(tstring name, shared_ptr<class Texture> val)
            : m_type(PropType::Texture)
            , m_name(name)
            , m_textureValue(val)
        {}

        MaterialProperty(tstring name, int val)
            : m_type(PropType::Int)
            , m_name(name)
            , m_intValue(val)
        {}

        tstring GetName() { return m_name; }
        PropType GetPropType() { return m_type; }

        math::Color             GetColor()      { return m_colorValue; }
        math::Vector4           GetVector4()    { return m_vectorValue; }
        float                   GetFloat()      { return m_floatValue; }
        math::Vector2           GetRange()      { return m_rangeLimits; }
        weak_ptr<class Texture> GetTexture()    { return m_textureValue; }
        int                     GetInt()        { return m_intValue; }

    private:
        /**
            @brief  material propery 타입
        **/
        PropType m_type;

        tstring m_name;

        /**
            @brief  material propery 타입에 따른 값
        **/
        math::Color m_colorValue;
        math::Vector4 m_vectorValue;
        float m_floatValue = 0.f;
        math::Vector2 m_rangeLimits;
        weak_ptr<class Texture> m_textureValue;
        int m_intValue = 0;

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

        /**
            @brief  머티리얼 버퍼에 등록된 렌더 패스 id
            @retval  - 렌더 패스 id
        **/
        const tstring& GetRenderPassID() { return m_renderPassID; }

        /**
            @brief 머티리얼 버퍼에 사용할 랜더 패스 id
            @param pass - 적용 시킬 렌더 패스 id
        **/
        void SetRenderPassID(const tstring& pass);
        
        /**
            @brief  머티리얼 버퍼에 적용된 파이프라인 uuid를 가져옴
            @retval  - uuid
        **/
        const tstring& GetPipelineID() { return m_pipelineID; }

        /**
            @brief 머티리얼 버퍼에 파이프라인 uuid에 해당하는 렌더 패스를 적용
            @param id - 새롭게 적용할 uuid
        **/
        void SetPipelineID(const tstring& id);

        /**
            @brief 머티리얼 프로퍼티를 추가
            @param val - 적용시킬 머티리얼 프로퍼티 리스트
        **/
        void AddProperties(vector<MaterialProperty> val);

        map<MaterialProperty::PropType, vector<MaterialProperty>> GetProperties() { return m_properties; }
        void SetProperties(map<MaterialProperty::PropType, vector<MaterialProperty>> val) { m_properties = val; }


        graphics::MaterialBuffer* GetMaterialBuffer() { return m_pMaterialBuffer; }

    private:
        tstring m_renderPassID;

        tstring m_pipelineID;
        
        map<MaterialProperty::PropType, vector<MaterialProperty>> m_properties;

        graphics::MaterialBuffer* m_pMaterialBuffer = nullptr;

        RTTR_ENABLE(Resource);

        RTTR_REGISTRATION_FRIEND;
    };
}