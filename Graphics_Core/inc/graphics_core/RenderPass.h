/**

    @file      RenderPass.h
    @brief     render pass class
    @details   한 개의 pipeline 관련 정보들을 관리하는 class
    @author    LWT
    @date      8.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

namespace graphics
{
    class RenderPass
    {
    public:
        RenderPass(tstring passName);
        virtual ~RenderPass();

        virtual void Bind(class CommandBuffer* command);

        class PipelineState* GetPipelineState() { return m_pPipelineState; }
        class PipelineLayout* GetPipelineLayout() { return m_pPipelineLayout; }

        void SetPipelineState(class PipelineState* state) { m_pPipelineState = state; }
        void SetPipelineLayout(class PipelineLayout* layout) { m_pPipelineLayout = layout; }

    private:
        tstring m_passName;

        class PipelineState* m_pPipelineState = nullptr;
        class PipelineLayout* m_pPipelineLayout =  nullptr;
    };
}