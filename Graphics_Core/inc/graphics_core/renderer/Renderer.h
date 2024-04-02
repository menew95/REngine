/**

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

        void SetCamera(class CameraBuffer* cameraBuffer);

        void RenderMesh(vector<class RenderObject*>& renderObjects, vector<class RenderPass*>& renderPassList);

        void RenderMesh(class MeshObject* meshObject, uint32 subMeshIdx);

        void RenderShadow(class MeshObject* meshObject);

        void RenderParticle(class ParticleObject* particleObject);

        void RenderSkyBox(class RenderPass* renderPass);

    protected:
        class CommandBuffer* m_pCommandBuffer = nullptr;

        class Buffer* m_pFrameBuffer = nullptr;
        class Buffer* m_pTransBuffer = nullptr;
    };
}