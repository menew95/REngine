/**

    @file      MaterialBuffer.h
    @brief     graphics material buffer class
    @details   ~
    @author    LWT
    @date      5.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <graphics_core\resource\ResourceBuffer.h>

namespace graphics
{
    class MaterialBuffer : public ResourceBuffer
    {
    public:
        MaterialBuffer();
        ~MaterialBuffer();

        GRAPHICS_API virtual void SetName(const char* name);

        void SetRenderPass(class RenderPass* pass) { m_pRenderPass = pass; }

        /**
            @brief pass에 render object 등록
            @param obj - render object ptr
        **/
        GRAPHICS_API void AddRenderObject(class RenderObject* obj);

        /**
            @brief pass에 등록 된 render object 제거
            @param obj - render object ptr
        **/
        GRAPHICS_API void RemoveRenderObject(class RenderObject* obj);

        void BindResource(class CommandBuffer* command);

        BufferType GetBufferType()
        {
            return BufferType::MATERIAL;
        }

    private:
        class RenderPass* m_pRenderPass = nullptr;

        class PipelineLayout* m_pPipelineLayout = nullptr;
    };
}