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

    struct TextureImportSetting
    {
        bool _bIsSRGB = false;
        bool _mipmaps = false;
        uint32 _format = 0;
        uint32 _comperssion = 0;
        uint32 _arrayLayers = 1;
        uint32 _mipLevels = 0;
        uint32 _samples = 1;

        uint32 _flags = 0;

        Extent3D _extent = {1, 1, 1};
    };

    class TextureBuffer : public ResourceBuffer
    {
    public:
        TextureBuffer();

        ~TextureBuffer();

        GRAPHICS_API void CreateTexture(uuid uuid, const TextureImportSetting& setting);

        GRAPHICS_API void LoadTexture(uuid uuid, const tstring& path);

        GRAPHICS_API virtual void SetName(const char* name);

        GRAPHICS_API BufferType GetBufferType()
        {
            return BufferType::TEXTURE;
        }

        GRAPHICS_API void* GetTextureID();

    private:
        Texture* m_pTexture = nullptr;
    };
}