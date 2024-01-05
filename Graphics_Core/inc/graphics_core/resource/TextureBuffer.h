/**

    @file      TextureBuffer.h
    @brief     texture buffer class
    @details   ~
    @author    LWT
    @date      4.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <graphics_core\resource\ResourceBuffer.h>

namespace graphics
{
    class Texture;

    class TextureBuffer : public ResourceBuffer
    {
    public:
        TextureBuffer();

        ~TextureBuffer();

        BufferType GetBufferType()
        {
            return BufferType::TEXTURE;
        }

        void* GetTextureID();

    private:
        Texture* m_pTexture = nullptr;
    };
}