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

namespace rengine
{
    class Material;

    class RENGINE_API Renderer : public Component
    {
    public:
        Renderer(std::shared_ptr<GameObject>& gameObj);

        Renderer(std::shared_ptr<GameObject>& gameObj, uuid uuid);

        Renderer(std::shared_ptr<GameObject>& gameObj, uuid uuid, tstring name);

        Renderer(const Renderer& renderer) = default;

        Renderer(Renderer&& renderer) = default;

        virtual ~Renderer();

    private:
        vector<shared_ptr<Material>> m_materials;

        struct RenderInfo
        {
            bool _isVisible;
            uint32 _shadowCastingMode;
        };
    };
}