/**

    @file      ResourceManager.h
    @brief     graphics resource factory class
    @details   각종 리소스들을 module의 rendersystem을 이용하여 생성하고 게임 엔진에서 사용될 리소스만 dll export
    @author    LWT
    @date      5.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <common\singleton.h>

#include <graphics_core\Graphics_core_dllexport.h>

namespace graphics
{
    class ResourceManager
    {
        DECLARE_SINGLETON_CLASS_EXPORT(GRAPHICS_API, ResourceManager)
    
    public:
        void Initialze(class RenderSystem* renderSystem);

        GRAPHICS_API class MeshBuffer*        CreateMeshBuffer(uuid uuid);
        GRAPHICS_API class MaterialBuffer*    CreateMaterialBuffer(uuid uuid);
        GRAPHICS_API class TextureBuffer*     CreateTextureBuffer(uuid uuid);
        GRAPHICS_API class CameraBuffer*      CreateCameraBuffer(uuid uuid);
        GRAPHICS_API class LightBuffer*       CreateLightBuffer(uuid uuid);

        GRAPHICS_API bool RelaseMeshBuffer(uuid uuid);
        GRAPHICS_API bool RelaseMaterialBuffer(uuid uuid);
        GRAPHICS_API bool RelaseTextureBuffer(uuid uuid);
        GRAPHICS_API bool RelaseCameraBuffer(uuid uuid);
        GRAPHICS_API bool RelaseLightBuffer(uuid uuid);
        /*GRAPHICS_API bool RelaseMeshBuffer(class MeshBuffer* meshBuffer);
        GRAPHICS_API bool RelaseMaterialBuffer(class MaterialBuffer* materialBuffer);
        GRAPHICS_API bool RelaseTextureBuffer(class TextureBuffer* textureBuffer);
        GRAPHICS_API bool RelaseCameraBuffer(class CameraBuffer* cameraBuffer);
        GRAPHICS_API bool RelaseLightBuffer(class LightBuffer* lightBuffer);*/

        class RenderPass*     CreateRenderPass(uuid uuid, struct RenderPassDesc& desc);

        class Buffer*         CreateBuffer(uuid uuid, struct BufferDesc& desc, const void* init = nullptr);
        class Shader*         CreateShader(uuid uuid, struct ShaderDesc& desc);
        class Sampler*        CreateSampler(uuid uuid, struct SamplerDesc& desc);
        class Texture*        CreateTexture(uuid uuid, struct TextureDesc& desc, const struct ImageDesc* image = nullptr);
        class ResourceView*   CreateResourceView(uuid uuid, struct ResourceViewDesc& desc);
        class RenderTarget*   CreateRenderTarget(uuid uuid, struct RenderTargetDesc& desc);
        class PipelineState*  CreatePipelineState(uuid uuid, struct GraphicsPipelineDesc& desc);
        class PipelineState*  CreatePipelineState(uuid uuid, struct ComputePipelineDesc& desc);
        class PipelineLayout* CreatePipelineLayout(uuid uuid, struct PipelineLayoutDesc& desc);

        bool ReleaseBuffer(class Buffer* buffer);
        bool ReleaseShader(class Shader* shader);
        bool ReleaseSampler(class Sampler* sampler);
        bool ReleaseTexture(class Texture* texture);
        bool ReleaseResourceView(class ResourceView* resourceView);
        bool ReleaseRenderTarget(class RenderTarget* renderTarget);
        bool ReleasePipelineState(class PipelineState* state);
        bool ReleasePipelineLayout(class PipelineLayout* layout);

    private:
        template <typename T>
        void ClearFromUnorderedMap(std::unordered_map<uuid, T*>& cont);

        class RenderSystem* m_pRenderSystem = nullptr;

        void ReleaseAll();

        // graphics core resource
        unordered_map<uuid, class MeshBuffer*>      m_meshBuffers;
        unordered_map<uuid, class LightBuffer*>     m_lightBuffers;
        unordered_map<uuid, class CameraBuffer*>    m_cameraBuffers;
        unordered_map<uuid, class TextureBuffer*>   m_textureBuffers;
        unordered_map<uuid, class MaterialBuffer*>  m_materialBuffers;

        unordered_map<uuid, class RenderPass*>      m_renderPassMap;

        // graphics module Resource
        unordered_map<uuid, class Shader*>          m_shaderMap;
        unordered_map<uuid, class Buffer*>          m_bufferMap;
        unordered_map<uuid, class Sampler*>         m_samplerMap;
        unordered_map<uuid, class Texture*>         m_textureMap;
        unordered_map<uuid, class ResourceView*>    m_resourceViewMap;
        unordered_map<uuid, class RenderTarget*>    m_renderTargetMap;
        unordered_map<uuid, class PipelineState*>   m_pipelineStateMap;
        unordered_map<uuid, class PipelineLayout*>  m_pipelineLayoutMap;
    };
}