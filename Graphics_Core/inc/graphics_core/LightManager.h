/**

    @file      LightManager.h
    @brief     light buffer관리 manager class
    @details   ~
    @author    LWT
    @date      8.04.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <common\singleton.h>

#include <graphics_core\Graphics_core_dllexport.h>

namespace graphics
{
    class CommandBuffer;

    class Texture;
    class Buffer;

    class LightBuffer;
    class MaterialBuffer;

    class LightManager
    {
        DECLARE_SINGLETON_CLASS_EXPORT(GRAPHICS_API, LightManager);

    public:
        void Initialize();
        /**
            @brief  light buffer를 하나 생성
            @param  uuid - light buffer에 적용될 uuid
            @retval      - 생성된 light buffer ptr
        **/
        GRAPHICS_API LightBuffer* CreateLightBuffer(uuid uuid);

        /**
            @brief  light buffer를 반환
            @param  uuid - 반환할 light buffer에 적용된 uuid
            @retval      - 반환 성공 여부
        **/
        GRAPHICS_API bool ReleaseLightBuffer(uuid uuid);

        /**
            @brief light 관련 모든 작업을 실행함
            @param command - command buffer ptr
        **/
        void Excute(CommandBuffer* command);

    private:
        /**
            @brief light관련 리소스들을 생성
        **/
        void CreateResource();

        /**
            @brief 모든 리소스를 해제
        **/
        void ReleaseAllResource();

        /**
            @brief 모든 라이트 정보를 텍스처에 기록
        **/
        void UpdateLightBuffer(CommandBuffer* command);

        uint32 m_curLightCnt;
        unordered_map<uuid, LightBuffer*> m_lightBufferMap;

        Buffer* m_lightBuffer = nullptr;

        Texture* m_spotLightShadowMap = nullptr;
        Texture* m_pointLightShadowMap = nullptr;
        Texture* m_cascadedShadowMap = nullptr;

        friend class ShadowRenderPass;
        friend class DeferredLightPass;
    };
}
