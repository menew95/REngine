﻿/**

    @file      FinalRenderPass.h
    @brief     최종 렌더링 패스
    @details   최종 결과물을 카메라 버퍼의 프레임 버퍼로 출력
    @author    LWT
    @date      11.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <graphics_core\RenderPass.h>

namespace graphics
{
    class FinalRenderPass : public RenderPass
    {
    public:
        FinalRenderPass();
        ~FinalRenderPass();

        void Init() override;

        void BeginExcute(class CommandBuffer* command, class CameraBuffer* camBuffer) override;

        void Excute(class CommandBuffer* command) override;

        void EndExcute(class CommandBuffer* command) override;

    private:
        class MaterialBuffer* m_pMatBuffer = nullptr;

        class RenderTarget* m_pRenderTarget = nullptr;
    };
}