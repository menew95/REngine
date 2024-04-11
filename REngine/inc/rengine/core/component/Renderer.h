/**

    @file      Renderer.h
    @brief     renderer base
    @details   ~
    @author    LWT
    @date      4.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <rengine\core\component\Component.h>

namespace graphics
{
    class MeshObject;
}

namespace rengine
{
    class Mesh;
    class Material;

    class RENGINE_API Renderer : public Component, public enable_shared_from_this<Renderer>
    {
    public:
        Renderer(uuid uuid, tstring type);

        Renderer(const Renderer& renderer) = default;

        Renderer(Renderer&& renderer) = default;

        virtual ~Renderer();

        vector<shared_ptr<Material>> GetMaterials();
        void SetMaterials(vector<shared_ptr<Material>> val);

        void SetMaterial(size_t i, shared_ptr<Material>& mat);

        shared_ptr<Mesh> GetMesh() { return m_mesh.lock(); }
        void SetMesh(shared_ptr<Mesh> mesh);

        void Update() override;

        void OnEnable() override;

        void OnDisable() override;

    protected:
        weak_ptr<Mesh> m_mesh;
        
        vector<weak_ptr<Material>> m_materials;

        struct RenderInfo
        {
            bool _isVisible;
            uint32 _shadowCastingMode;
        };

        graphics::MeshObject* m_pRenderObject = nullptr;

        RTTR_ENABLE(Component);

        RTTR_REGISTRATION_FRIEND;
    };
}