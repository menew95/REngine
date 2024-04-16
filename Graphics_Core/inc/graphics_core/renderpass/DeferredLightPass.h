/**

    @file      DeferredLightPass.h
    @brief     deferred light passs
    @details   Gbuffer를 사용하여 light를 계산 하는 패스(Linear space)
    @author    LWT
    @date      3.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <graphics_core\RenderPass.h>

namespace graphics
{
    class DeferredLightPass : public RenderPass
    {
    public:
        DeferredLightPass();
        ~DeferredLightPass();

        void Init() override;

        void BeginExcute(class CommandBuffer* command, class CameraBuffer* camBuffer) override;

        void Excute(class CommandBuffer* command) override;

        void EndExcute(class CommandBuffer* command) override;

    private:
        class MaterialBuffer* m_pMatBuffer = nullptr;

        class MeshObject* m_pScreenMesh = nullptr;

        class RenderTarget* m_pRenderTarget = nullptr;

        friend class LightManager;
    };
}