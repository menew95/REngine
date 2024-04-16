/**

    @file      GridRenderPass.h
    @brief     debug를 그리는 패스
    @details   grid, debug mesh 등을 처리하는 렌더 패스
    @author    LWT
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

        void Init() override;

        void BeginExcute(class CommandBuffer* command, class CameraBuffer* camBuffer) override;

        void Excute(class CommandBuffer* command) override;

        void EndExcute(class CommandBuffer* command) override;

    private:

        class MeshObject* m_pDebugMesh = nullptr;

        class MaterialBuffer* m_pDebugMaterial = nullptr;

        class Buffer* m_pDebugBuffer = nullptr;

        class RenderTarget* m_pRenderTarget = nullptr;
    };
}