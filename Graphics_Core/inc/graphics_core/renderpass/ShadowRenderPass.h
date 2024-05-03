/**

    @file      ShadowRenderPass.h
    @brief     shadow render pass
    @details   라이트의 타입별 그림자 맵을 생성하는 렌더 패스
    @author    LWT
    @date      5.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <graphics_core\RenderPass.h>

namespace graphics
{
    class Buffer;

    /**
        @struct ProjectedShadowInfo
        @brief  그림자를 그리기 위한 라이트 버퍼와 해당 그림자를 그리기 위한 정보 구조체
    **/
    struct ProjectedShadowInfo
    {
        class LightBuffer* _lightBuffer;
        vector<class RenderObject*> _primitives;

        // point와 cascaded의 인스턴스 데이터를 채워넣기 위한 변수
        vector<vector<int>> _instanceData;
    };

    class ShadowRenderPass : public RenderPass
    {
    public:
        ShadowRenderPass();
        ~ShadowRenderPass();

        void Init() override;

        void BeginExcute(class CommandBuffer* command, class CameraBuffer* camBuffer) override;

        void Excute(class CommandBuffer* command) override;

        void EndExcute(class CommandBuffer* command) override;

    private:
        

        /**
            @brief 라이트에 그릴 이번 프레임에 처리할 매쉬 선별
            @param shadowInfo - 그림자 정보 구조체
        **/
        void BeginCulling(ProjectedShadowInfo* shadowInfo);

        /**
            @brief CSM에서 사용될 프리미티브 데이터 선별
            @param projectedShadowInfo - 그림자 정보 구조체
        **/
        void BeginGatherCSMShadowPrimitives(CameraBuffer* camBuffer);

        /**
            @brief 그림자 맵을 그림
            @param command             - command buffer
            @param projectedShadowInfo - 그림자 정보 구조체
        **/
        void RenderDepth(CommandBuffer* command, ProjectedShadowInfo& projectedShadowInfo);

        void CopyToShadowAtlas(CommandBuffer* command, ProjectedShadowInfo& projectedShadowInfo);

        MaterialBuffer* m_spotLightShadow = nullptr;
        MaterialBuffer* m_cascadedLightShadow = nullptr;
        MaterialBuffer* m_pointLightShadow = nullptr;

        MaterialBuffer* m_copy2DDepth = nullptr;
        MaterialBuffer* m_copyCubeDepth = nullptr;

        Buffer* m_perObjectBuffer = nullptr;
        Buffer* m_perMaterialBuffer = nullptr;
        Buffer* m_perInstanceBuffer = nullptr;

        vector<ProjectedShadowInfo> m_projectedShadowInfos;
    };
}