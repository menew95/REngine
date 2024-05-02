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

#include <common\math.h>

#include <common\singleton.h>

#include <graphics_core\Graphics_core_dllexport.h>

#define NUM_CASCADES 4
#define NUM_FRUSTUM_CORNERS 8
#define NUM_SPOTLIGHT 24
#define NUM_POINTLIGHT 12
#define SHADOWMAP_ATLAS_SIZE 8192

namespace graphics
{
    class CommandBuffer;

    class Texture;
    class Buffer;
    class TextureAtlas;
    class RenderTarget;

    class LightBuffer;
    class MaterialBuffer;

    struct LightInfo;

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

        void GetPerFrame(struct PerFrame& perFrame);

        /**
            @brief  라이트에서 사용될 렌더 타겟을 만들기 위해 남아있는 텍스처 array layer idx를
                    찾고 light info에 인덱스를 기록
            @param  light - 라이트 정보
            @retval       - 사용가능한 인덱스
        **/
        uint32 GetShadowMapTextureIndex(const LightInfo& light);

        /**
            @brief  더이상 사용되지 않는 라이트의 텍스처 인덱스를 풀에 넣음
            @param  light - 라이트 정보
            @retval       - 반환 여부
        **/
        bool ReturnShadowMapSpace(const LightInfo& light);

        /**
            @brief  라이트버퍼에 그림자맵 아틀라스로부터 비어있는 공간을 찾아 해당 영역을 소유하도록 만듬
            @param  lightbuffer - 라이트 버퍼
            @retval             - 소유 성공 여부
        **/
        bool SetShadowMapSpace(LightBuffer* lightbuffer);

        /**
            @brief  라이트버퍼에 그림자맵 아틀라스로부터 할당된 영역을 해제함
            @param  lightbuffer - 라이트 버퍼
            @retval             - 해제 성공 여부
        **/
        bool ReleaseShadowMapSpace(LightBuffer* lightbuffer);

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

        /**
            @brief spot light 렌더 타겟을 만들기 위해 비어있는 인덱스를 가지고옴
        **/
        uint32 GetTextureArrayLayerForSpotRenderTarget();

        /**
            @brief point light 렌더 타겟을 만들기 위해 비어있는 인덱스를 가지고옴
        **/
        uint32 GetTextureArrayLayerForPointRenderTarget();

        uint32 m_curLightCnt;

        bool m_cascadedSet = false;
        LightBuffer* m_cascadedLightBuffer = nullptr;
        Vector3 m_cascadedDir;

        unordered_map<uuid, LightBuffer*> m_lightBufferMap;

        Buffer* m_lightBuffer = nullptr;

        std::stack<uint32> _texture2DUsableSpace;

        std::stack<uint32> _cubeMapUsableSpace;

        Texture* m_spotLightShadowMap = nullptr;
        Texture* m_pointLightShadowMap = nullptr;
        Texture* m_cascadedShadowMap = nullptr;

        Vector4 m_directionalLightPos;
        Vector4 m_directionalLightColor;

        Texture* m_shadowMapAtlasTexture = nullptr;

        RenderTarget* m_shadowMapAtlasRT = nullptr;
        
        shared_ptr<TextureAtlas> m_shadowMapAtlas;

        friend class ShadowRenderPass;
        friend class DeferredLightPass;
        friend class LightBuffer;
        friend class Renderer;
    };
}
