/**

    @file      ShadowRenderPass.h
    @brief     shadow render pass
    @details   라이트의 타입별 그림자 맵을 생성하는 렌더 패스
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

        void BeginExcute(class CommandBuffer* command, class CameraBuffer* camBuffer) override;

        void Excute(class CommandBuffer* command) override;

        void EndExcute(class CommandBuffer* command) override;

    private:
        MaterialBuffer* m_spotLightShadow = nullptr;
        MaterialBuffer* m_cascadedLightShadow = nullptr;
        MaterialBuffer* m_pointLightShadow = nullptr;
    };
}