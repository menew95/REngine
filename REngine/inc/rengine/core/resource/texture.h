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

        virtual ~Texture();

        ResourceType GetResourceType() override
        {
            return ResourceType::TEXTURE;
        };

        void* GetTextureID();

        /**
            @brief  texture 리소스의 메모리 로드
            @retval  - 로드 성공 여부
        **/
        bool LoadMemory();

        /**
            @brief  texture 리소스의 메모리 해제
            @retval  - 해제 성공 여부
        **/
        bool UnLoadMemory();

        inline bool   GetSRGB()         const { return m_textureImportSetting._bIsSRGB; }
        inline bool   GetMipMap()       const { return m_textureImportSetting._mipmaps; }
        inline uint32 GetFormat()       const { return m_textureImportSetting._format; }
        inline uint32 GetComperssion()  const { return m_textureImportSetting._comperssion; }
        inline uint32 GetArrayLayers()  const { return m_textureImportSetting._arrayLayers; }
        inline uint32 GetMipLevels()    const { return m_textureImportSetting._mipLevels; }
        inline uint32 GetSamples()      const { return m_textureImportSetting._samples; }

        inline void SetSRGB(bool val)           { m_textureImportSetting._bIsSRGB = val; }
        inline void SetMipMap(bool val)         { m_textureImportSetting._mipmaps = val; }
        inline void SetFormat(uint32 val)       { m_textureImportSetting._format = val; }
        inline void SetComperssion(uint32 val)  { m_textureImportSetting._comperssion= val; }
        inline void SetArrayLayers(uint32 val)  { m_textureImportSetting._arrayLayers= val; }
        inline void SetMipLevels(uint32 val)    { m_textureImportSetting._mipLevels = val; }
        inline void SetSamples(uint32 val)      { m_textureImportSetting._samples = val; }

    private:
        graphics::TextureBuffer* m_pTextureBuffer = nullptr;

        struct TextureImportSetting
        {
            bool _bIsSRGB = false;
            bool _mipmaps = false;
            uint32 _format = 0;
            uint32 _comperssion = 0;
            uint32 _arrayLayers = 1;
            uint32 _mipLevels = 0;
            uint32 _samples = 1;
        };

        TextureImportSetting m_textureImportSetting;

        RTTR_ENABLE(Resource);

        RTTR_REGISTRATION_FRIEND;
    };
}