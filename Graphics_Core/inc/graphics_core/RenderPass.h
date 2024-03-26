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

#include <graphics_core\RenderPassFlags.h>

namespace graphics
{
    class RenderPass
    {
    public:
        RenderPass(tstring passName);
        virtual ~RenderPass();

        /**
            @brief pass에 설정 된 pipeline를 바인드
            @param command - command buffer ptr
        **/
        virtual void Bind(class CommandBuffer* command);

        virtual void BeginExcute(class CommandBuffer* command, class CameraBuffer* camBuffer);

        virtual void Excute(class CommandBuffer* command);

        virtual void EndExcute(class CommandBuffer* command);

        /**
            @brief 머티리얼 버퍼에 패스가 정해지면 해당 렌더패스에 머티리얼 버퍼 캐싱
            @param matBuffer - 등록할 머티리얼 버퍼
        **/
        void AddMaterialBuffer(class MaterialBuffer* matBuffer);

        /**
            @brief 머티리얼 버퍼의 패스가 달라지면 캐싱된 머티리얼 버퍼 삭제
            @param matBuffer - 삭제할 머티리얼 버퍼
        **/
        void RemoveMaterialBuffer(class MaterialBuffer* matBuffer);

        // 머티리얼 버퍼가 렌더 오브젝트를 가지고 있도록 설계 변경
        ///**
        //    @brief pass에 render object 등록
        //    @param obj - render object ptr
        //**/
        //void AddRenderObject(class RenderObject* obj);
        //
        ///**
        //    @brief pass에 등록 된 render object 제거
        //    @param obj - render object ptr
        //**/
        //void RemoveRenderObject(class RenderObject* obj);

        const tstring& GetRenderPassName() { return m_passName; }
        class PipelineState* GetPipelineState() { return m_pPipelineState; }
        class PipelineLayout* GetPipelineLayout() { return m_pPipelineLayout; }
        vector<class RenderObject*>& GetRenderObjects() { return m_renderObjects; }

        void SetPipelineState(class PipelineState* state) { m_pPipelineState = state; }
        void SetPipelineLayout(class PipelineLayout* layout) { m_pPipelineLayout = layout; }

    protected:
        tstring m_passName;

        class PipelineState* m_pPipelineState = nullptr;
        class PipelineLayout* m_pPipelineLayout =  nullptr;

        vector<class RenderObject*> m_renderObjects;

        vector<class MaterialBuffer*> m_materialBufferList;
    };
}