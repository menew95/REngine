/**

    @file      PostProcessRenderPass.h
    @brief     post process render pass
    @details   ssr, tonemap, blur 등 포스트 프로세스를 적용하는 렌더 패스
    @author    LWT
    @date      9.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <graphics_core\RenderPass.h>

namespace graphics
{
    class PostProcessRenderPass : public RenderPass
    {
    public:
        PostProcessRenderPass();
        ~PostProcessRenderPass();

        void Init() override;

        void BeginExcute(class CommandBuffer* command, class CameraBuffer* camBuffer) override;

        void Excute(class CommandBuffer* command) override;

        void EndExcute(class CommandBuffer* command) override;

    private:
        class MaterialBuffer* m_pMatBuffer = nullptr;

        class MeshObject* m_pScreenMesh = nullptr;

        class RenderTarget* m_pRenderTarget = nullptr;

        class Texture* m_pMainFrameColorTexture = nullptr;

        class Texture* m_pCameraColorTexture = nullptr;

        friend class LightManager;
    };
}