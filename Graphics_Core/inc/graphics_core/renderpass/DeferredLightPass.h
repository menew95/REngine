/**

    @file      DeferredLightPass.h
    @brief     deferred light passs
    @details    deferred light를 계산 하는 패스
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

        /**
            @brief pass에 설정 된 pipeline를 바인드
            @param command - command buffer ptr
        **/
        void Bind(class CommandBuffer* command) override;

        void BeginExcute(class CommandBuffer* command, class CameraBuffer* camBuffer) override;

        void Excute(class CommandBuffer* command) override;

        void EndExcute(class CommandBuffer* command) override;

    private:
        class MaterialBuffer* m_pMatBuffer = nullptr;

        class MeshObject* m_pScreenMesh = nullptr;

        class RenderTarget* m_pRenderTarget = nullptr;
    };
}