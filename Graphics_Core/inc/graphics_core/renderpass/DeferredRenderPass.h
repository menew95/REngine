/**

    @file      DeferredRenderPass.h
    @brief     디퍼드 렌더 패스
    @details   Gbuffer들을 생성해준다(Albedo, Emissive, Normal, WorldPos, Flag)
    @author    LWT
    @date      11.03.2024
    @copyright © LWT, 2024. All right reserved.

**/
#pragma once

#include <graphics_core\RenderPass.h>

namespace graphics
{
    class DeferredRenderPass : public RenderPass
    {
    public:
        DeferredRenderPass();
        ~DeferredRenderPass();

        void Init() override;

        void BeginExcute(class CommandBuffer* command, class CameraBuffer* camBuffer) override;

        void Excute(class CommandBuffer* command) override;

        void EndExcute(class CommandBuffer* command) override;

    private:
        class Buffer* m_pTransBuffer = nullptr;

        class RenderTarget* m_pRenderTarget = nullptr;
    };
}