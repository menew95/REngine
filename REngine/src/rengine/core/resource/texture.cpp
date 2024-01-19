#include <rengine\core\resource\Texture.h>
#include <graphics_core\resource\TextureBuffer.h>

#include <rttr\registration.h>

RTTR_REGISTRATION
{
	rttr::registration::class_<rengine::Texture>("Texture")
	.constructor<tstring>()
	.property("sRGBTexture", &rengine::Texture::GetSRGB, &rengine::Texture::SetSRGB)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::BOOL)
	)
	.property("mipmaps", &rengine::Texture::GetMipMap, &rengine::Texture::SetMipMap)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::BOOL)
	)
	.property("format", &rengine::Texture::GetFormat, &rengine::Texture::SetFormat)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::UINT32)
	)
	.property("comperssion", &rengine::Texture::GetComperssion, &rengine::Texture::SetComperssion)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::UINT32)
	)
	.property("arraylayer", &rengine::Texture::GetArrayLayers, &rengine::Texture::SetArrayLayers)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::UINT32)
	)
	.property("miplevels", &rengine::Texture::GetMipLevels, &rengine::Texture::SetMipLevels)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::UINT32)
	)
	.property("samples", &rengine::Texture::GetSamples, &rengine::Texture::SetSamples)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::UINT32)
	)
	;
}

namespace rengine
{
	Texture::Texture(uuid uuid)
	: Resource(uuid, TEXT("Texture"))
	{

	}

	Texture::~Texture()
	{
		if (m_bIsLoad)
		{

		}
	}
	void* Texture::GetTextureID()
	{
		return m_textureBuffer->GetTextureID();
	}

	bool Texture::LoadMemory()
	{
		return true;
	}

	bool Texture::UnLoadMemory()
	{
		return true;
	}
}