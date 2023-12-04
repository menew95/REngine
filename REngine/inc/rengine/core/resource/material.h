/**

    @file      material.h
    @brief     material class
    @details   ~
    @author    LWT
    @date      4.12.2023
    @copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include <rengine\core\resource\resource.h>

namespace graphics
{
    class MaterialBuffer;
}

namespace rengine
{
    class RENGINE_API Material : public Resource
    {
        Material(uuid uuid);

        Material(uuid uuid, tstring name);

        Material(uuid uuid, tstring name, tstring type);

        Material(const Material& mat) = default;

        Material(Material&& mat) = default;

        virtual ~Material();

        inline virtual ResourceType GetType()
        {
            return ResourceType::MATERIAL;
        };

    private:
        graphics::MaterialBuffer* m_materialBuffer;
    };
}