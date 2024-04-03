/**

    @file      LightBuffer.h
    @brief     graphics light buffer class
    @details   ~
    @author    LWT
    @date      5.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <graphics_core\resource\ResourceBuffer.h>

namespace graphics
{
    enum class LightType
    {
        Spot = 0,
        Directional = 1,
        Point = 2,
    };

    struct LightInfo
    {
        uint32 _lightype;
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

    private:
        void CreateRenderTarget();

        LightInfo m_lightInfo;

        ShadowInfo m_shadowInfo;

        // shadow map texture
        class Texture* m_pTexture = nullptr;

        // shadow map render target
        class RenderTarget* m_pRenderTarget = nullptr;
    };
}