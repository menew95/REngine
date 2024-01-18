/**

    @file      texture.h
    @brief     texture class
    @author    LWT
    @date      4.12.2023

**/
#pragma once

#include <rengine\core\resource\Resource.h>

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

        Texture(const Texture& mesh) = default;

        Texture(Texture&& mesh) = default;

        virtual ~Texture() override;

        ResourceType GetResourceType() override
        {
            return ResourceType::TEXTURE;
        };

        //void* GetRawSRV()
        //{
        //    return m_textureBuffer
        //}

    private:
        graphics::TextureBuffer* m_textureBuffer = nullptr;

        RTTR_ENABLE(Resource);

        RTTR_REGISTRATION_FRIEND;
    };
}