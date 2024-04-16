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

        virtual void Init() {}

        /**
            @brief 렌더 패스를 시작하기 전에 필요한 내용 처리
            @param command   - command buffer
            @param camBuffer - 패스를 처리할 camera buffer
        **/
        virtual void BeginExcute(class CommandBuffer* command, class CameraBuffer* camBuffer);

        /**
            @brief 패스 실행
            @param command - command buffer 
        **/
        virtual void Excute(class CommandBuffer* command);

        /**
            @brief 렌더 패스가 끝난후 필요한 내용 처리
            @param command - command buffer
        **/
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

        const tstring& GetRenderPassName() { return m_passName; }

    protected:
        tstring m_passName;

        vector<class MaterialBuffer*> m_materialBufferList;
    };
}