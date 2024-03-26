/**

    @file      MaterialBuffer.h
    @brief     graphics material buffer class로 게임 엔진의 머티리얼 클래스와 1:1 대응
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
        
        /**
            @brief 머티리얼 버퍼를 등록할 렌더 패스
            @param id - 렌더 패스 id
        **/
        GRAPHICS_API void SetRenderPass(const tstring& id);

        /**
            @brief 머티리얼 버퍼에 적용할 파이프라인 id
            @param id - 파이프라인 id
        **/
        GRAPHICS_API void SetPipelineID(const tstring& id);

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

        const auto& GetRenderObjectList() { return m_renderObjectList; }

    private:
        class RenderPass* m_pRenderPass = nullptr;

        class PipelineState* m_pPipelineState = nullptr;

        class PipelineLayout* m_pPipelineLayout = nullptr;

        vector<class RenderObject*> m_renderObjectList;
    };
}