/**

    @file      SkyBoxRenderPass.h
    @brief     
    @details   ~
    @author    Cool Guy
    @date      6.02.2024
    @copyright © LWT, 2024. All right reserved.

**/
#pragma once

#include <graphics_core\RenderPass.h>

namespace graphics
{
    class SkyBoxRenderPass : public RenderPass
    {
    public:
        SkyBoxRenderPass();
        ~SkyBoxRenderPass();

        void Init() override;

        /**
            @brief pass에 설정 된 pipeline를 바인드
            @param command - command buffer ptr
        **/
        void Bind(class CommandBuffer* command) override;

        void BeginExcute(class CommandBuffer* command, class CameraBuffer* camBuffer) override;

        void Excute(class CommandBuffer* command) override;

        void EndExcute(class CommandBuffer* command) override;

        void SetSkyBox(class TextureBuffer* texture);

    private:
        class MaterialBuffer* m_pSkyBoxMatBuffer = nullptr;

        class MeshObject* m_pSkyBoxMesh = nullptr;

        class TextureBuffer* m_pSkyBoxTexture = nullptr;

        class Buffer* m_pTransBuffer = nullptr;

        class RenderTarget* m_pRenderTarget = nullptr;
    };
}