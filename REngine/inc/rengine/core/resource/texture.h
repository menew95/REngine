/**

    @file      texture.h
    @brief     texture class
    @author    LWT
    @date      4.12.2023

**/
#pragma once

#include <rengine\core\resource\resource.h>

namespace graphics
{
    class TextureBuffer;
}

namespace rengine
{
    class RENGINE_API Texture : public Resource
    {
    public:
        Texture(uuid uuid);

        Texture(uuid uuid, tstring name);

        Texture(uuid uuid, tstring name, tstring type);

        Texture(const Texture& mesh) = default;

        Texture(Texture&& mesh) = default;

        virtual ~Texture();

        inline virtual ResourceType GetType()
        {
            return ResourceType::TEXTURE;
        };

    private:
        graphics::TextureBuffer* m_textureBuffer;
    };
}