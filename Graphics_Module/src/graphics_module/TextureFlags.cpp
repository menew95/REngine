#include <common/common.h>
#include "graphics_module\TextureFlags.h"

namespace graphics
{
	
	GRAPHICS_DLL_DECLSPEC bool IsCubeTexture(const TextureType& type)
	{
		return (type == TextureType::TextureCube || type == TextureType::TextureCubeArray);
	}

	GRAPHICS_DLL_DECLSPEC bool IsMultiSampleTexture(const TextureType& type)
	{
		return (type >= TextureType::Texture2DMS);
	}

	GRAPHICS_DLL_DECLSPEC bool IsMipMappedTexture(const TextureDesc& desc)
	{
		return (!IsMultiSampleTexture(desc._textureType) && (desc._mipLevels == 0 || desc._mipLevels > 1));
	}

	GRAPHICS_DLL_DECLSPEC uint32 NumMipLevels(uint32 width, uint32 height, uint32 depth)
	{
		const auto maxSize = std::max({ width, height, depth });
		const auto log2Size = static_cast<uint32>(std::log2(maxSize));
		return (1u + log2Size);
	}

	GRAPHICS_DLL_DECLSPEC Offset3D CalcTextureOffset(const TextureType type, const Offset3D& offset, uint32 baseArrayLayer)
	{
		switch (type)
		{
		case TextureType::Texture1D:
			return Offset3D{ offset.x, 0, 0 };

		case TextureType::Texture1DArray:
			return Offset3D{ offset.x, static_cast<int32>(baseArrayLayer), 0 };

		case TextureType::Texture2D:
		case TextureType::Texture2DMS:
			return Offset3D{ offset.x, offset.y, 0 };

		case TextureType::Texture2DArray:
		case TextureType::TextureCube:
		case TextureType::TextureCubeArray:
		case TextureType::Texture2DMSArray:
			return Offset3D{ offset.x, offset.y, static_cast<int32>(baseArrayLayer) };

		case TextureType::Texture3D:
			return offset;
		}
		return {};
	}
}