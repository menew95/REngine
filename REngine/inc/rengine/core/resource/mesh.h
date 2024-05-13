/**

    @file      Mesh.h
    @brief     mesh
    @details   ~
    @author    Cool Guy
    @date      4.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <rengine\core\resource\Resource.h>

#include <common\vertex_attribute.h>

namespace graphics
{
    class MeshBuffer;
}

namespace rengine
{
    class RENGINE_API Mesh : public Resource
    {
    public:
        Mesh(uuid uuid);

        Mesh(const Mesh& mesh) = default;

        Mesh(Mesh&& mesh) = default;

        ~Mesh();

        ResourceType GetResourceType() override
        {
            return m_bIsSkinned ?  ResourceType::SKINNED_MESH : ResourceType::MESH;
        }

        /**
            @brief  mesh 리소스의 메모리 로드
            @retval  - 로드 성공 여부
        **/
        bool LoadMemory();

        /**
            @brief  mesh 리소스의 메모리 해제
            @retval  - 해제 성공 여부
        **/
        bool UnLoadMemory();

        auto& GetVertices() { return m_vertices; }
        auto& Getindices() { return m_indices; }
        size_t GetSubMeshCount() const { return m_indices.size(); }
        auto& GetBoundingBoxMin() { return m_boundingBoxMin; }
        auto& GetBoundingBoxMax() { return m_boundingBoxMax; }
        auto& GetIsSkinned() { return m_bIsSkinned; }
        auto& GetBoneName() { return m_boneName; }

        void SetVertices(const auto& val) { m_vertices = val; }
        void Setindices(const auto& val) { m_indices = val; }
        void SetBoundingBoxMin(const auto& val) { m_boundingBoxMin = val; }
        void SetBoundingBoxMax(const auto& val) { m_boundingBoxMax = val; }
        void SetIsSkinned(const auto& val) { m_bIsSkinned = val; }
        void SetBoneName(const auto& val) { m_boneName = val; }

        graphics::MeshBuffer* GetMeshBuffer() { return m_pMeshBuffer; }

    private:

        // mesh bin
        std::vector<VertexAttribute>		m_vertices;
        std::vector<std::vector<uint32>>	m_indices;
        math::Vector3                       m_boundingBoxMin;
        math::Vector3                       m_boundingBoxMax;

        // skinning
        bool    m_bIsSkinned = false;
        tstring m_boneName;

        graphics::MeshBuffer* m_pMeshBuffer = nullptr;

        RTTR_ENABLE(Resource);

        RTTR_REGISTRATION_FRIEND;
    };
}