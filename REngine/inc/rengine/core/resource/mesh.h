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

        virtual ~Mesh();

        ResourceType GetResourceType() override
        {
            return m_bIsSkinned ?  ResourceType::SKINNED_MESH : ResourceType::MESH;
        }

    private:
        bool m_bIsSkinned = false;

        graphics::MeshBuffer* m_meshBuffer = nullptr;

        RTTR_ENABLE(Resource);

        RTTR_REGISTRATION_FRIEND;
    };
}