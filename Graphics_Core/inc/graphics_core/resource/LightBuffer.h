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
        Vector3 _direction;

        Vector3 _position;
        float _attenuationRadius;

        Vector3 _color;
        float _intensity;

        float _angle;
        float _innerAngle;
        float _fallOffExponential;
        float _width;

        Vector3 _up;
        float _height;

        int _staticShadowMapIdx;
        int _dynamicShadowMapIdx;
        uint _shadowState;
        int _pad;

        Matrix _shadowMatrix[6];
    };

    struct ShadowInfo
    {
        uint32 _resolution;
        uint32 _bias;
        uint32 _normalBias;
        uint32 _near;
    };

    class LightBuffer : public ResourceBuffer
    {
    public:
        LightBuffer();
        ~LightBuffer();

        GRAPHICS_API virtual void SetName(const char* name);

        BufferType GetBufferType()
        {
            return BufferType::LIGHT;
        }

        GRAPHICS_API bool GetEnable() { return m_enable; }
        GRAPHICS_API void SetEnable(bool value) { m_enable = value; }

        GRAPHICS_API uint32 GetLightType() const { return m_lightInfo._type; }
        GRAPHICS_API void SetLightType(uint32 value);

        GRAPHICS_API const Vector3& GetDirection() { return m_lightInfo._direction; }
        GRAPHICS_API void SetDirection(const Vector3& value) { m_lightInfo._direction = value; }

        GRAPHICS_API const Vector3& GetPosition() { return m_lightInfo._position; }
        GRAPHICS_API void SetPosition(const Vector3& value) { m_lightInfo._position = value; }

        GRAPHICS_API float GetRange() const { return m_lightInfo._attenuationRadius; }
        GRAPHICS_API void SetRange(float value) { m_lightInfo._attenuationRadius = value; }

        GRAPHICS_API const math::Vector3& GetLightColor() const { return m_lightInfo._color; }
        GRAPHICS_API void SetLightColor(const math::Vector3& value) { m_lightInfo._color = value; }

        GRAPHICS_API float GetIntensity() const { return m_lightInfo._intensity; }
        GRAPHICS_API void SetIntensity(float value) { m_lightInfo._intensity = value; }

        GRAPHICS_API float GetSpotAnlge() const { return m_lightInfo._angle; }
        GRAPHICS_API void SetSpotAnlge(float value) { m_lightInfo._angle  = value; }

        GRAPHICS_API float GetInnerSpotAngle() const { return m_lightInfo._innerAngle; }
        GRAPHICS_API void SetInnerSpotAngle(float value) { m_lightInfo._innerAngle; }

        GRAPHICS_API float GetFallOffExponential() const { return m_lightInfo._fallOffExponential; }
        GRAPHICS_API void SetFallOffExponential(float value) { m_lightInfo._fallOffExponential = value; }

        GRAPHICS_API float GetWidth() const { return m_lightInfo._width; }
        GRAPHICS_API void SetWidth(float value) { m_lightInfo._width = value; }

        GRAPHICS_API const Vector3& GetUp() { return m_lightInfo._up; }
        GRAPHICS_API void SetUp(const Vector3& value) { m_lightInfo._up = value; }

        GRAPHICS_API float GetHeight() { return m_lightInfo._height; }
        GRAPHICS_API void SetHeight(float value) { m_lightInfo._height = value; }

        GRAPHICS_API uint32 GetResolution() const { return static_cast<uint32>(m_shadowInfo._resolution); }
        GRAPHICS_API void SetResolution(uint32 value);

        GRAPHICS_API void SetWorld(Matrix& world);


        const LightInfo& GetLightInfo() { return m_lightInfo; }

        const ShadowInfo& GetShadowInfo() { return m_shadowInfo; }

    private:
        void CreateRenderTarget();

        bool m_enable = false;

        uint32 m_lightIdx;

        LightInfo m_lightInfo;

        ShadowInfo m_shadowInfo;

        // shadow map texture
        class Texture* m_pTexture = nullptr;

        class Texture* m_pDepthTexture = nullptr;

        // shadow map render target
        class RenderTarget* m_pRenderTarget = nullptr;
    };
}