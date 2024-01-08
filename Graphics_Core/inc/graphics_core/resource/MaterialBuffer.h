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

        BufferType GetBufferType()
        {
            return BufferType::MATERIAL;
        }

        void SetRenderPass(class RenderPass* pass) { m_pRenderPass = pass; }

        /**
            @brief pass에 render object 등록 
            @param obj - render object ptr
        **/
        void AddRenderObject(class RenderObject* obj);

        /**
            @brief pass에 등록 된 render object 제거
            @param obj - render object ptr
        **/
        void RemoveRenderObject(class RenderObject* obj);

    private:
        class RenderPass* m_pRenderPass = nullptr;
    };
}