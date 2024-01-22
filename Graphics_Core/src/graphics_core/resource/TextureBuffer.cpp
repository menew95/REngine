#include <graphics_core\resource\TextureBuffer.h>

#include <graphics_module\texture.h>

#include <graphics_core\ResourceManager.h>

namespace graphics
{
	TextureBuffer::TextureBuffer()
	: ResourceBuffer()
	{

	}

	TextureBuffer::~TextureBuffer()
	{

	}

	void TextureBuffer::CreateTexture(uuid uuid, const TextureImportSetting& setting)
	{
		TextureDesc _texDesc;
		_texDesc._arrayLayers = setting._arrayLayers;
		_texDesc._mipLevels = setting._mipLevels;
		_texDesc._samples = setting._samples;
		_texDesc._format = (graphics::Format)setting._format;

		_texDesc._extend = setting._extent;

		_texDesc._bindFlags = setting._flags;

		if(!setting._mipmaps)
			_texDesc._miscFlags |= MiscFlags::GenerateMips;

		m_pTexture = ResourceManager::GetInstance()->CreateTexture(uuid, _texDesc, nullptr);
	}

	void TextureBuffer::LoadTexture(uuid uuid, const tstring& path)
	{
		TextureDesc _texDesc;

		ImageDesc _imageDesc;

		_imageDesc._filePath = path;

		m_pTexture = ResourceManager::GetInstance()->CreateTexture(uuid, _texDesc, &_imageDesc);
	}

	void TextureBuffer::SetName(const char* name)
	{
		assert(m_pTexture != nullptr);

		m_pTexture->SetName(name);
	}
	
	void* TextureBuffer::GetTextureID()
	{
		return m_pTexture->GetTextureID();
	}
}