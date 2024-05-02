/**

    @file      CameraBuffer.h
    @brief     camera buffer class
    @details   per frame buffer에 업데이트할 내용을 담고 있는 카메라 버퍼
    @author    LWT
    @date      5.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <graphics_core\resource\ResourceBuffer.h>

#include <common\math.h>

namespace graphics
{
    class Buffer;
    class CommandBuffer;

    struct alignas(16) CameraInfo
    {
        math::Vector3 _cameraWorldPos;
        float _pad;

        math::Matrix _view;

        math::Matrix _proj;

        math::Matrix _projInv;

        math::Matrix _viewToTexSpace;

        math::Matrix _worldViewProj;

        math::Matrix _viewProj;

        math::Matrix _viewProjInv;

        math::Matrix _viewProjInvTranspose;

        float _near = 0.1f;
        float _far = 1000.f;
        float _fov = 60.f * math::Deg2Rad;
        float _aspectRatio = 1.0f;
    };

    struct alignas(16) CascadedInfo
    {
        math::Matrix _lightTransform[4];
        Vector4 _cascadeOffset[5];
        math::Vector4 _cascadeScale;
    };

    class CameraBuffer : public ResourceBuffer
    {
    public:
        CameraBuffer();
        ~CameraBuffer();

        GRAPHICS_API void Init();

        GRAPHICS_API void OnResize(const Extent2D& resolution);

        GRAPHICS_API virtual void SetName(const char* name);

        GRAPHICS_API void Update(CameraInfo& info)
        {
            m_cameraInfo = info;
        }

        void UpdateBuffer(CommandBuffer* command, Buffer* buffer);

        void UpdateCascadeShadow(math::Vector3 directionalLightDir);

        BufferType GetBufferType()
        {
            return BufferType::CAMERA;
        }

        auto* GetRenderTarget() const { return m_pRenderTarget; }
        auto* GetColorTexture() const { return m_pColorTexture; }
        auto* GetDepthTexture() const { return m_pDepthTexture; }

        GRAPHICS_API const auto& GetCameraInfo() { return m_cameraInfo; }
        GRAPHICS_API const auto& GetCascadedInfo() { return m_cascadedInfo; }

        GRAPHICS_API const bool& GetEnable() { return m_bIsEnable; }

        GRAPHICS_API void* GetTextureID();

        void SetEnable(auto& val) { m_bIsEnable = val; }

        GRAPHICS_API const auto& GetRenderPassList() { return m_renderPassList; }

        void PushRenderPass(auto* pass) { m_renderPassList.push_back(pass); }
        void RemoveRenderPass(auto idx) { m_renderPassList.erase(idx); }

        math::Matrix GetView() { return m_cameraInfo._view; }
        math::Matrix GetProj() { return m_cameraInfo._proj; }

    private:
        bool m_bIsEnable = true;

        CameraInfo m_cameraInfo;

        CascadedInfo m_cascadedInfo;

        class RenderTarget* m_pRenderTarget = nullptr;

        class Texture* m_pColorTexture = nullptr;

        class Texture* m_pDepthTexture = nullptr;

        vector<class RenderPass*> m_renderPassList;

        friend class CullingHelper;
    };
}