#include <graphics_core\resource\TextureBuffer.h>

#include <graphics_module\Texture.h>

#include <graphics_core\ResourceManager.h>

#include <common\math.h>

#include <DirectXTex\DirectXTex.h>


namespace graphics
{
	TextureBuffer* TextureBuffer::White = nullptr;
	TextureBuffer* TextureBuffer::Black = nullptr;
	TextureBuffer* TextureBuffer::Gray = nullptr;
	TextureBuffer* TextureBuffer::Bump = nullptr;
	TextureBuffer* TextureBuffer::Red = nullptr;

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

	void TextureBuffer::Setting(const TextureImportSetting& setting)
	{
		TextureDesc _texDesc;
		_texDesc._arrayLayers = setting._arrayLayers;
		_texDesc._mipLevels = setting._mipLevels;
		_texDesc._samples = setting._samples;
		_texDesc._format = (graphics::Format)setting._format;

		_texDesc._extend = setting._extent;

		_texDesc._bindFlags = setting._flags;

	}

	void TextureBuffer::LoadTexture(uuid uuid, const tstring& path)
	{
		using namespace DirectX;

		ScratchImage image;

		HRESULT _hr = S_FALSE;

		// directxtex를 thread를 사용하기 위해서는 따로 초기화가 필요함
		static bool _isInitThread = false;

		static mutex _mutex;

		_mutex.lock();
		if (!_isInitThread)
		{
			HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

			_isInitThread = hr == S_OK;
		}
		_mutex.unlock();

		switch (CheckFileFormat(path))
		{
			case FileFormat::DDS:
			{
				_hr = LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, image);
				break;
			}
			case FileFormat::TGA:
			{
				_hr = LoadFromTGAFile(path.c_str(), TGA_FLAGS_NONE, nullptr, image);
				break;
			}
			case FileFormat::HDR:
			{
				_hr = LoadFromHDRFile(path.c_str(), nullptr, image);

				break;
			}
			case FileFormat::WIC:
			default:
			{
				_hr = LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, image);

				break;
			}
		}

		assert(_hr == S_OK);

		TextureDesc _texDesc;

		_texDesc._extend = {
			static_cast<uint32>(image.GetMetadata().width),
			static_cast<uint32>(image.GetMetadata().height),
			static_cast<uint32>(image.GetMetadata().depth)
		};
		_texDesc._arrayLayers = static_cast<uint32>(image.GetMetadata().arraySize);
		_texDesc._mipLevels = static_cast<uint32>(image.GetMetadata().mipLevels);
		_texDesc._bindFlags = BindFlags::ShaderResource;
		_texDesc._miscFlags = 0;

		_texDesc._format = (graphics::Format)image.GetMetadata().format;

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

		_mutex.lock();

		m_pTexture = ResourceManager::GetInstance()->CreateTexture(uuid, _texDesc, _imageDescs.data());

		//m_pSampler = ResourceManager::GetInstance()->GetSampler(TEXT("WrapLinear"));

		_mutex.unlock();
	}

	void TextureBuffer::LoadTexture(uuid uuid, TextureDesc& texDesc, const ImageDesc& imageDesc)
	{
		m_pTexture = ResourceManager::GetInstance()->CreateTexture(uuid, texDesc, &imageDesc);
	}

	bool TextureBuffer::UnLoadTexture()
	{
		return ResourceManager::GetInstance()->ReleaseTexture(m_pTexture);
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

	void TextureBuffer::CreateDefaultTextureBuffer()
	{
		White = ResourceManager::GetInstance()->CreateTextureBuffer(TEXT("white"))->CreateDefaultTexture(Color::White);
		Black = ResourceManager::GetInstance()->CreateTextureBuffer(TEXT("black"))->CreateDefaultTexture(Color::Black);
		Gray = ResourceManager::GetInstance()->CreateTextureBuffer(TEXT("gray"))->CreateDefaultTexture({ 0.5f, 0.5f, 0.5f, 0.5f });
		Bump = ResourceManager::GetInstance()->CreateTextureBuffer(TEXT("bump"))->CreateDefaultTexture({ 0.5f, 0.5f, 1.f, 1.f });
		Red = ResourceManager::GetInstance()->CreateTextureBuffer(TEXT("red"))->CreateDefaultTexture(Color::Red);
	}

	TextureBuffer* TextureBuffer::CreateDefaultTexture(const math::Color& color)
	{
		TextureDesc _texDesc;
		_texDesc._extend = {
			static_cast<uint32>(1),
			static_cast<uint32>(1),
			static_cast<uint32>(1)
		};
		_texDesc._arrayLayers = 1;
		_texDesc._mipLevels = 1;

		_texDesc._format = graphics::Format::R8G8B8A8_UNORM;
		_texDesc._textureType = TextureType::Texture2D;

		uint8 _color[4] = 
		{
			static_cast<uint8>(color.x * 255), 
			static_cast<uint8>(color.y * 255),
			static_cast<uint8>(color.z * 255),
			static_cast<uint8>(color.w * 255)
		};

		ImageDesc _imageDesc;
		_imageDesc._data = _color;
		_imageDesc._rowStride = 1;
		_imageDesc._layerStride = 1;

		m_pTexture = ResourceManager::GetInstance()->CreateTexture(m_uuid, _texDesc, &_imageDesc);

		return this;
	}
}