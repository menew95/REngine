#include <graphics_core\resource\TextureBuffer.h>

#include <graphics_module\Texture.h>

#include <graphics_core\ResourceManager.h>

#include <DirectXTex\DirectXTex.h>


namespace graphics
{
	FileFormat CheckFileFormat(const tstring& path)
	{
		if (static_cast<uint32>(path.length()) > 1)
		{
			size_t extensionStartPoint = path.find_last_of('.') + (size_t)1;

			auto fileFormat = path.substr(extensionStartPoint);

			if (fileFormat == TEXT("dds") || fileFormat == TEXT("DDS"))
			{
				return FileFormat::DDS;
			}
			else if (fileFormat == TEXT("tga") || fileFormat == TEXT("TGA"))
			{
				return FileFormat::TGA;
			}
			else if (fileFormat == TEXT("hdr") || fileFormat == TEXT("HDR"))
			{
				return FileFormat::HDR;
			}
			else
			{
				return FileFormat::WIC;
			}
		}

		return FileFormat::WIC;
	}

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
		using namespace DirectX;

		ScratchImage image;

		switch (CheckFileFormat(path))
		{
			case FileFormat::DDS:
			{
				LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, image);
				break;
			}
			case FileFormat::TGA:
			{
				LoadFromTGAFile(path.c_str(), TGA_FLAGS_NONE, nullptr, image);
				break;
			}
			case FileFormat::HDR:
			{
				LoadFromHDRFile(path.c_str(), nullptr, image);
				break;
			}
			case FileFormat::WIC:
			default:
			{
				LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, image);
				break;
			}
		}

		TextureDesc _texDesc;
		_texDesc._extend = { 
			static_cast<uint32>(image.GetMetadata().width), 
			static_cast<uint32>(image.GetMetadata().height),
			static_cast<uint32>(image.GetMetadata().depth)
		};
		_texDesc._arrayLayers = static_cast<uint32>(image.GetMetadata().arraySize);
		_texDesc._mipLevels = static_cast<uint32>(image.GetMetadata().mipLevels);
		
		_texDesc._format = (graphics::Format)image.GetMetadata().format;

		//_texDesc._samples = 1;
		//_texDesc._bindFlags;
		//_texDesc._miscFlags;

		if (image.GetMetadata().IsCubemap())
		{
			_texDesc._textureType = TextureType::TextureCube;
		}
		else
		{
			switch (image.GetMetadata().dimension)
			{
				case TEX_DIMENSION::TEX_DIMENSION_TEXTURE1D:
				{
					_texDesc._textureType = TextureType::Texture1D;
					break;
				}
				case TEX_DIMENSION::TEX_DIMENSION_TEXTURE2D:
				{
					_texDesc._textureType = TextureType::Texture2D;
					break;
				}
				case TEX_DIMENSION::TEX_DIMENSION_TEXTURE3D:
				{
					_texDesc._textureType = TextureType::Texture3D;
					break;
				}
			}
		}

		image.GetImage(0, 0, 0)->rowPitch;
		image.GetImage(0, 0, 0)->slicePitch;

		AssertMessageBox(image.GetImageCount() != 0, "D3D11Texture LoadFaile Error");

		vector<ImageDesc> _imageDescs;

		for (size_t _mip = 0; _mip < image.GetMetadata().mipLevels; _mip++)
		{
			for (size_t _slice = 0; _slice < image.GetMetadata().arraySize; _slice++)
			{
				ImageDesc _imageDesc;

				_imageDesc._data = image.GetImage(_mip, _slice, 0)->pixels;
				_imageDesc._rowStride = static_cast<uint32>(image.GetImage(_mip, _slice, 0)->rowPitch);
				_imageDesc._layerStride = static_cast<uint32>(image.GetImage(_mip, _slice, 0)->slicePitch);

				_imageDescs.emplace_back(_imageDesc);
			}
		}

		m_pTexture = ResourceManager::GetInstance()->CreateTexture(uuid, _texDesc, _imageDescs.data());
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