/**

    @file      MeshRenderer.h
    @brief     Mesh Renderer Component
    @details   ~
    @author    LWT
    @date      4.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <rengine\core\component\Renderer.h>

namespace rengine
{
    class RENGINE_API MeshRenderer : public Renderer, public enable_shared_from_this<MeshRenderer>
    {
    public:
        //MeshRenderer(std::shared_ptr<GameObject>& gameObj);

        MeshRenderer(uuid uuid);

        //MeshRenderer(std::shared_ptr<GameObject>& gameObj, uuid uuid, tstring name);

        MeshRenderer(const MeshRenderer& renderer) = default;

        MeshRenderer(MeshRenderer&& renderer) = default;

        virtual ~MeshRenderer();

        void Awake() override;

        //void Render() override;

    protected:
        weak_ptr<Mesh> m_mesh;

        vector<weak_ptr<Material>> m_materials;

        RTTR_ENABLE(Renderer);

        RTTR_REGISTRATION_FRIEND;
    };
}
