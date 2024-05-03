﻿/**

    @file      Renderer.h
    @brief     
    @details   ~
    @author    LWT
    @date      8.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\common.h>

#include <common\singleton.h>

namespace graphics
{
    class Renderer
    {
    public:
        static Renderer* s_pRenderer;

        static Renderer* GetInstance() { return s_pRenderer; }

        Renderer(class CommandBuffer* command);

        ~Renderer();

        void SetFrameResource();

        void SetCamera(const struct SceneInfo& sceneInfo, class CameraBuffer* cameraBuffer);

        void RenderMesh(vector<class RenderObject*>& renderObjects, vector<class RenderPass*>& renderPassList);

        void RenderMesh(class MeshObject* meshObject, uint32 subMeshIdx);

        void DrawRectangle();

        void RenderShadow(class MeshObject* meshObject);

        void RenderParticle(class ParticleObject* particleObject);

        void RenderSkyBox(class RenderPass* renderPass);

        CommandBuffer* GetCommandBuffer() const { return m_commandBuffer; }

    protected:
        class CommandBuffer* m_commandBuffer = nullptr;

        class Buffer* m_perFrameBuffer = nullptr;
        class Buffer* m_perCameraBuffer = nullptr;
        class Buffer* m_perObjectBuffer = nullptr;
        class Buffer* m_perSkinedBuffer = nullptr;

        class MeshBuffer* m_quadMeshBuffer = nullptr;
    };
}