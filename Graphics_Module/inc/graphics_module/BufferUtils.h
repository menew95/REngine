#pragma once

#include "graphics_module\BufferFlags.h"
#include <graphics_module/graphics_module_dllexport.h>

namespace graphics
{
	class Buffer;

	GRAPHICS_DLL_DECLSPEC uint32 GetStorageBufferStride(const BufferDesc& desc);
	GRAPHICS_DLL_DECLSPEC long GetCombinedBindFlags(uint32 numBuffers, Buffer* const* bufferArray);

	inline bool IsBufferViewEnalbe(const BufferViewDesc& viewDesc)
	{
		return (viewDesc._format != Format::UNKNOWN || viewDesc._offset != 0 || viewDesc._size != -1);
	}
}