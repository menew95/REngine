#pragma once

#include "common\common.h"
#include "common\math.h"
#include "graphics_module\Format.h"
#include "graphics_module\ResourceFlags.h"
#include "graphics_module\CommandBufferFlags.h"
#include <graphics_module/graphics_module_dllexport.h>

namespace graphics
{
	enum class TextureType
	{
		Texture1D,
		Texture2D,
		Texture3D,
		TextureCube,
		Texture1DArray,
		Texture2DArray,
		TextureCubeArray,
		Texture2DMS,
		Texture2DMSArray,
	};

	//enum class TextureSwizzle
	//{
	//	Zero,
	//	One,
	//	Red,
	//	Green,
	//	Blue,
	//	Alpha
	//};

	enum class FileFormat
	{
		DDS = 0,
		TGA = 1,
		HDR = 2,
		WIC = 3,
	};

	struct ImageDesc
	{
		void* _data = nullptr;

		uint32 _rowStride = 0;
		uint32 _layerStride = 0;
	};

	struct TextureSubresource
	{
		TextureSubresource() = default;
		TextureSubresource(const TextureSubresource&) = default;

		inline TextureSubresource(uint32 _baseArrayLayer, uint32 _baseMipLevel) :
			_baseArrayLayer{ _baseArrayLayer },
			_baseMipLevel{ _baseMipLevel }
		{
		}

		inline TextureSubresource(uint32 _baseArrayLayer, uint32 _numArrayLayers, uint32 _baseMipLevel, uint32 _numMipLevels) :
			_baseArrayLayer{ _baseArrayLayer },
			_numArrayLayers{ _numArrayLayers },
			_baseMipLevel{ _baseMipLevel },
			_numMipLevels{ _numMipLevels }
		{
		}

		uint32   _baseArrayLayer = 0;
		uint32   _numArrayLayers = 1;
		uint32   _baseMipLevel = 0;
		uint32   _numMipLevels = 1;
	};

	struct TextureRegion
	{
		TextureRegion() = default;
		TextureRegion(const TextureRegion&) = default;

		inline TextureRegion(const Offset3D& _offset, const Extent3D& _extent) :
			_offset{ _offset },
			_extent{ _extent }
		{
		}

		inline TextureRegion(const TextureSubresource& _subresource, const Offset3D& _offset, const Extent3D& _extent) :
			_subresource{ _subresource },
			_offset{ _offset },
			_extent{ _extent }
		{
		}
		TextureSubresource  _subresource;

		Offset3D		_offset;

		Extent3D		_extent;
	};

	struct TextureDesc
	{
		TextureType _textureType = TextureType::Texture2D;
		uint32 _bindFlags = (BindFlags::ShaderResource | BindFlags::RenderTarget);
		uint32 _miscFlags = (MiscFlags::FixedSamples | MiscFlags::GenerateMips);
		Format _format = Format::R8G8B8A8_UNORM;
		Extent3D _extend = { 1, 1, 1 };
		uint32 _arrayLayers = 1;
		uint32 _mipLevels = 0;
		uint32 _samples = 1;
		//ClearValue _clearValue;
	};

	GRAPHICS_DLL_DECLSPEC bool IsCubeTexture(const TextureType& type);
	GRAPHICS_DLL_DECLSPEC bool IsMultiSampleTexture(const TextureType& desc);
	GRAPHICS_DLL_DECLSPEC bool IsMipMappedTexture(const TextureDesc& desc);
	GRAPHICS_DLL_DECLSPEC uint32 NumMipLevels(std::uint32_t width, std::uint32_t height, std::uint32_t depth);
}