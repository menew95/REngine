/**

    @file      GridRenderPass.h
    @brief
    @details   ~
    @author    Cool Guy
    @date      11.03.2024
    @copyright © LWT, 2024. All right reserved.

**/
#pragma once

#include <graphics_core\RenderPass.h>

namespace graphics
{
    class GridRenderPass : public RenderPass
    {
    public:
        GridRenderPass();
        ~GridRenderPass();

        void Init();

        /**
            @brief pass에 설정 된 pipeline를 바인드
            @param command - command buffer ptr
        **/
        void Bind(class CommandBuffer* command) override;

        void BeginExcute(class CommandBuffer* command, class CameraBuffer* camBuffer) override;

        void Excute(class CommandBuffer* command) override;

        void EndExcute(class CommandBuffer* command) override;

    private:

        class MeshObject* m_pDebugMesh = nullptr;

        class MaterialBuffer* m_pDebugMaterial = nullptr;

        class Buffer* m_pDebugBuffer = nullptr;
    };
}