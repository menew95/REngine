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
        uint32 _lightype;
        float _range;
        float _fallOff;
        float _pad;

        math::Vector3 _lightPosition;
        float _spotAngle;

        math::Vector3 _direction;
        float _fallOffAngle;

        math::Vector3 _color;
        float _intensity;

        math::Matrix _shadowTransform;
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

        inline uint32 GetLightType() const { return m_lightInfo._lightype; }
        GRAPHICS_API void SetLightType(uint32 value);

        inline const math::Color& GetLightColor() const { return math::Color(m_lightInfo._color); }
        GRAPHICS_API void SetLightColor(const math::Color& value);

        inline float GetIntensity() const { return m_lightInfo._intensity; }
        GRAPHICS_API void SetIntensity(float value);

        inline float GetRange() const { return m_lightInfo._range; }
        GRAPHICS_API void SetRange(float value);

        inline float GetSpotAnlge() const { return m_lightInfo._spotAngle; }
        GRAPHICS_API void SetSpotAnlge(float value);

        inline float GetInnerSpotAngle() const { return m_lightInfo._fallOffAngle; }
        GRAPHICS_API void SetInnerSpotAngle(float value);

        inline uint32 GetResolution() const { return static_cast<uint32>(m_shadowInfo._resolution); }
        GRAPHICS_API void SetResolution(uint32 value);

    private:
        void CreateRenderTarget();

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