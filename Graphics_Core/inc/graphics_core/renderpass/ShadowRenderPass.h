/**

    @file      ShadowRenderPass.h
    @brief     shadow render pass
    @details   ~
    @author    LWT
    @date      5.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <graphics_core\RenderPass.h>

namespace graphics
{
    class ShadowRenderPass : public RenderPass
    {
    public:
        ShadowRenderPass();
        ~ShadowRenderPass();

        void Init() override;

        /**
            @brief pass에 설정 된 pipeline를 바인드
            @param command - command buffer ptr
        **/
        void Bind(class CommandBuffer* command) override;

        void BeginExcute(class CommandBuffer* command, class CameraBuffer* camBuffer) override;

        void Excute(class CommandBuffer* command) override;

        void EndExcute(class CommandBuffer* command) override;
    };
}