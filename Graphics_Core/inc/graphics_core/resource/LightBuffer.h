/**

    @file      LightBuffer.h
    @brief     graphics light buffer class
    @details   ~
    @author    LWT
    @date      5.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\math.h>

#include <graphics_core\resource\ResourceBuffer.h>

namespace graphics
{
    enum class LightType
    {
        Spot = 0,
        Directional = 1,
        Point = 2,
    };

    struct alignas(16)LightInfo
    {
        uint32 _type;
        Vector3 _direction = Vector3::Forward;

        Vector3 _position = Vector3::Zero;
        float _attenuationRadius = 5.0f;

        Vector3 _color = Vector3::One;
        float _intensity = 2.5f;

        float _angle = 30.f * Deg2Rad;      // radian
        float _innerAngle = 21.80208f * Deg2Rad; // radian
        float _fallOffExponential;
        float _width = 1.f;

        Vector3 _up = Vector3::Up;
        float _height = 1.0f;

        int _staticShadowMapIdx;
        int _dynamicShadowMapIdx;
        uint _shadowState;
        int _pad;

        Matrix _shadowMatrix[6];

        Vector2 _uv0[6];
        Vector2 _uv1[6];
    };

    struct ShadowInfo
    {
        uint32 _resolution = 3;
        uint32 _bias;
        uint32 _normalBias;
        uint32 _near;

        Matrix _view[6];
        Matrix _proj;
    };

    class LightBuffer : public ResourceBuffer
    {
    public:
        LightBuffer();
        ~LightBuffer();

        GRAPHICS_API virtual void SetName(const char* name);

        GRAPHICS_API void Initialize();

        BufferType GetBufferType()
        {
            return BufferType::LIGHT;
        }

        GRAPHICS_API bool GetEnable() { return m_enable; }
        GRAPHICS_API void SetEnable(bool value) { m_enable = value; }

        GRAPHICS_API uint32 GetLightType() const { return m_lightInfo._type; }
        GRAPHICS_API void SetLightType(uint32 value);

        GRAPHICS_API const Vector3& GetDirection() { return m_lightInfo._direction; }
        GRAPHICS_API void SetDirection(const Vector3& value);

        GRAPHICS_API const Vector3& GetPosition() { return m_lightInfo._position; }
        GRAPHICS_API void SetPosition(const Vector3& value);

        GRAPHICS_API float GetRange() const { return m_lightInfo._attenuationRadius; }
        GRAPHICS_API void SetRange(float value);

        GRAPHICS_API const math::Vector3& GetLightColor() const { return m_lightInfo._color; }
        GRAPHICS_API void SetLightColor(const math::Vector3& value);

        GRAPHICS_API float GetIntensity() const { return m_lightInfo._intensity; }
        GRAPHICS_API void SetIntensity(float value);

        GRAPHICS_API float GetSpotAnlge() const { return m_lightInfo._angle; }
        GRAPHICS_API void SetSpotAnlge(float value);

        GRAPHICS_API float GetInnerSpotAngle() const { return m_lightInfo._innerAngle; }
        GRAPHICS_API void SetInnerSpotAngle(float value);

        GRAPHICS_API float GetFallOffExponential() const { return m_lightInfo._fallOffExponential; }
        GRAPHICS_API void SetFallOffExponential(float value);

        GRAPHICS_API float GetWidth() const { return m_lightInfo._width; }
        GRAPHICS_API void SetWidth(float value);

        GRAPHICS_API const Vector3& GetUp() { return m_lightInfo._up; }
        GRAPHICS_API void SetUp(const Vector3& value);

        GRAPHICS_API float GetHeight() { return m_lightInfo._height; }
        GRAPHICS_API void SetHeight(float value);

        GRAPHICS_API uint32 GetResolution() const { return static_cast<uint32>(m_shadowInfo._resolution); }
        GRAPHICS_API void SetResolution(uint32 value);

        GRAPHICS_API void SetWorld(Matrix& world);

        const LightInfo& GetLightInfo() { return m_lightInfo; }

        const ShadowInfo& GetShadowInfo() { return m_shadowInfo; }

        math::Vector2 GetResolution();

        void BakeShadow();

        inline bool IsDirty() { return m_isDirty; }

    private:
        void CreateShadowMap();

        void ReleaseShadowMap();

        void UpdateLightTransform();

        bool m_enable = false;

        bool m_isDirty = false;

        uint32 m_lightIdx;

        LightInfo m_lightInfo;

        ShadowInfo m_shadowInfo;

        bool m_isSetShadowMapSpace = false;

        // shadow map texture
        class Texture* m_pTexture = nullptr;

        class Texture* m_pDepthTexture = nullptr;

        // shadow map render target
        class RenderTarget* m_pRenderTarget = nullptr;

        friend class ShadowRenderPass;
        friend class LightManager;
    };
}