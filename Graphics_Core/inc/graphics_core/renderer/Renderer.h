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

namespace graphics
{
    class Renderer
    {
    public:
        Renderer(class CommandBuffer* command);

        void RenderMesh(vector<class RenderObject*>& renderObjects, vector<class RenderPass*>& renderPassList);

        void RenderMesh(class MeshObject* meshObject);

        void RenderShadow(class MeshObject* meshObject);

        void RenderParticle(class ParticleObject* particleObject);

        void RenderSkyBox(class RenderPass* renderPass);

    protected:
        class CommandBuffer* m_pCommandBuffer = nullptr;
    };
}