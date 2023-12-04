/**

    @file      Mesh.h
    @brief     mesh
    @details   ~
    @author    Cool Guy
    @date      4.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <rengine\core\resource\resource.h>

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

        Mesh(uuid uuid, tstring name);

        Mesh(uuid uuid, tstring name, tstring type);

        Mesh(const Mesh& mesh) = default;

        Mesh(Mesh&& mesh) = default;

        virtual ~Mesh();

        virtual ResourceType GetType();

    private:
        graphics::MeshBuffer* m_meshBuffer;

    };
}