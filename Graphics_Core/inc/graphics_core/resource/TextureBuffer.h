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

namespace math
{
    struct Color;
}

namespace graphics
{
    class Texture;
    class Sampler;

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

        uint32 _x_axis_addressMode;
        uint32 _y_axis_addressMode;
        uint32 _z_axis_addressMode;
        uint32 _filter;
    };

    class TextureBuffer : public ResourceBuffer
    {
    public:
        TextureBuffer();

        ~TextureBuffer();

        GRAPHICS_API void CreateTexture(uuid uuid, const TextureImportSetting& setting);

        GRAPHICS_API void Setting(const TextureImportSetting& setting);

        GRAPHICS_API void LoadTexture(uuid uuid, const tstring& path);

        GRAPHICS_API void LoadTexture(uuid uuid, struct TextureDesc& texDesc, const struct ImageDesc& imageDesc);

        GRAPHICS_API bool UnLoadTexture();

        GRAPHICS_API virtual void SetName(const char* name);

        GRAPHICS_API BufferType GetBufferType()
        {
            return BufferType::TEXTURE;
        }

        GRAPHICS_API void* GetTextureID();

        auto* GetTexture() { return m_pTexture; }

        auto* GetSampler() { return m_pSampler; }


        static void CreateDefaultTextureBuffer();

        static TextureBuffer* White;
        static TextureBuffer* Black;
        static TextureBuffer* Gray;
        static TextureBuffer* Bump;
        static TextureBuffer* Red;

    private:
        TextureBuffer* CreateDefaultTexture(const struct math::Color& color);
        
        Texture* m_pTexture = nullptr;

        Sampler* m_pSampler = nullptr;
    };
}