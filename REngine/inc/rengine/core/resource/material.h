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

        RENGINE_API const tstring& GetName() const { return m_name; }
        RENGINE_API string GetNameStr() const { return StringHelper::WStringToString(m_name); }

        RENGINE_API PropType GetPropType() const { return m_type; }

        RENGINE_API math::Color                 GetColor()   const { return m_colorValue; }
        RENGINE_API math::Vector4               GetVector4() const { return m_vectorValue; }
        RENGINE_API float                       GetFloat()   const { return m_floatValue; }
        RENGINE_API math::Vector2               GetRange()   const { return m_rangeLimits; }
        RENGINE_API shared_ptr<class Texture>   GetTexture() const { return m_textureValue.lock(); }
        RENGINE_API int                         GetInt()     const { return m_intValue; }

        void SetColor(const math::Color& val) { m_colorValue = val; }
        void SetVector4(const math::Vector4& val) { m_vectorValue = val; }
        void SetFloat(float val) { m_floatValue = val; }
        void SetRange(const math::Vector2& val) { m_rangeLimits = val; }
        void SetTexture(const shared_ptr<class Texture>& texture) { m_textureValue = texture; }
        void SetInt(int val) { m_intValue = val; }

    private:
        /**
            @brief  material propery 타입
        **/
        PropType m_type;

        tstring m_name;

        /**
            @brief  material propery 타입에 따른 값
        **/
        math::Color m_colorValue = math::Color::Black;
        math::Vector4 m_vectorValue = math::Vector4::Zero;
        float m_floatValue = 0.f;
        math::Vector2 m_rangeLimits = {0.f, 1.f};
        weak_ptr<class Texture> m_textureValue;
        int m_intValue = 0;

        friend class Material;
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

        map<MaterialProperty::PropType, vector<MaterialProperty>>& GetProperties() { return m_properties; }
        void SetProperties(map<MaterialProperty::PropType, vector<MaterialProperty>>& val);

        void SetPropertyBlock(const  vector<MaterialProperty>& properties);

        /**
            @brief material의 color값 설정
            @param name  - property name
            @param value - color value
        **/
        void SetColor(const tstring& name, const Color& value);

        /**
            @brief material의 vector값 설정
            @param name  - property name
            @param value - vector4 value
        **/
        void SetVector4(const tstring& name, const Vector4& value);

        /**
            @brief material의 float 값 설정
            @param name  - property name
            @param value - float value
        **/
        void SetFloat(const tstring& name, float value);

        /**
            @brief material의 새 텍스처 할당
            @param name    - property name
            @param texture - texture
        **/
        void SetTexture(const tstring& name, const shared_ptr<Texture>& texture);

        /**
            @brief material의 int 값 설정
            @param name  - property name
            @param value - int value
        **/
        void SetInteger(const tstring& name, int value);

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