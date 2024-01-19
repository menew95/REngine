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

    private:
        bool m_bIsSkinned = false;

        graphics::MeshBuffer* m_pMeshBuffer = nullptr;

        RTTR_ENABLE(Resource);

        RTTR_REGISTRATION_FRIEND;
    };
}