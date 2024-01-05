/**

    @file      MeshBuffer.h
    @brief     mesh buffer class
    @details   ~
    @author    LWT
    @date      5.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <graphics_core\resource\ResourceBuffer.h>

namespace graphics
{
    class Buffer;

    class SubMeshBuffer
    {
    public:
        SubMeshBuffer();
        ~SubMeshBuffer();

        inline uint32 GetIndexCount() const
        {
            return m_indexCount;
        }

        inline Buffer* GetBuffer() const
        {
            return m_pIndexBuffer;
        }

    private:
        uint32 m_indexCount = 0;
        Buffer* m_pIndexBuffer = nullptr;
    };

    class MeshBuffer : public ResourceBuffer
    {
    public:
        MeshBuffer();
        ~MeshBuffer();

        BufferType GetBufferType()
        {
            return BufferType::MESH;
        }

        inline size_t GetSubMeshCount() const
        {
            return m_subMeshBuffers.size();
        }

        inline Buffer* GetBuffer() const
        {
            return m_pVertexBuffer;
        }

        inline SubMeshBuffer& GetSubMesh(size_t index)
        {
            return m_subMeshBuffers[index];
        }

        void CreateVertexBuffer(uuid uuid, const void* data, uint32 size, uint32 stride);

    private:
        Buffer* m_pVertexBuffer = nullptr;
        std::vector<SubMeshBuffer> m_subMeshBuffers;
    };
}