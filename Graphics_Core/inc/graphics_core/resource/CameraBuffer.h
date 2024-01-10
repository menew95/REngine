﻿/**

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

#define NUM_CASCADES 4

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

        float _near = 0.1f;
        float _far = 1000.f;
        float pad2[2];
    };

    struct alignas(16) CascadedInfo
    {
        math::Matrix _lightTransform[4];
        float _cascadeEndClipSpace[NUM_CASCADES];
    };

    class CameraBuffer : public ResourceBuffer
    {
    public:
        CameraBuffer();
        ~CameraBuffer();

        BufferType GetBufferType()
        {
            return BufferType::CAMERA;
        }

        void Update(CameraInfo& info)
        {
            m_cameraInfo = info;
        }

        void UpdateBuffer(CommandBuffer* command, Buffer* buffer);

        void UpdateCascadeShadow(math::Vector3 directionalLightDir);

    private:
        CameraInfo m_cameraInfo;

        CascadedInfo m_cascadedInfo;
    };
}