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
    class RenderObject;
}

namespace rengine
{
    class Material;

    class RENGINE_API Renderer : public Component, public enable_shared_from_this<Renderer>
    {
    public:
        Renderer(uuid uuid, tstring type);

        Renderer(const Renderer& renderer) = default;

        Renderer(Renderer&& renderer) = default;

        virtual ~Renderer();

        vector<weak_ptr<Material>> GetMaterials() { return m_materials; }
        void SetMaterials(vector<weak_ptr<Material>> val) { m_materials = val; }

    private:
        vector<weak_ptr<Material>> m_materials;

        struct RenderInfo
        {
            bool _isVisible;
            uint32 _shadowCastingMode;
        };

        graphics::RenderObject* m_pRenderObject = nullptr;

        RTTR_ENABLE(Component);

        RTTR_REGISTRATION_FRIEND;
    };
}