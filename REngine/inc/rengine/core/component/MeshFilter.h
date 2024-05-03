/**

    @file      MeshFilter.h
    @brief     mesh filter component
    @details   ~
    @author    LWT
    @date      4.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <rengine\core\component\Component.h>

namespace rengine
{
    class RENGINE_API MeshFilter : public Component, public enable_shared_from_this<MeshFilter>
    {
    public:
        MeshFilter(const uuid& uuid);

        MeshFilter(const MeshFilter& filter) = default;

        MeshFilter(MeshFilter&& filter) = default;

        virtual ~MeshFilter();

        weak_ptr<class Mesh> GetMesh() { return m_pMesh; }
        void SetMesh(weak_ptr<class Mesh> val) { m_pMesh = val; }

    protected:
        weak_ptr<class Mesh> m_pMesh;

        RTTR_ENABLE(Component);

        RTTR_REGISTRATION_FRIEND;
    };
}