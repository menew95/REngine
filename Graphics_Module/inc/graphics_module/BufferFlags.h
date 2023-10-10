#pragma once

#include <common\common.h>
#include <common\vertex_attribute.h>
#include <graphics_module\Format.h>
#include <graphics_module\ResourceFlags.h>
#include <graphics_module\graphics_module_dllexport.h>

namespace graphics
{
	struct BufferDesc
	{
		uint32 _size = 0;
		uint32 _stride = 0;
		uint32 _bindFlags = 0;
		uint32 _cpuAccessFlags = 0;
		uint32 _slot = 0;
		uint32 _miscFlags = 0;
		Format _format = Format::UNKNOWN;

		std::vector<VertexAttribute>  _vertexAttribs;
	};

	struct BufferViewDesc
	{
		Format _format = Format::UNKNOWN;

		uint32 _offset = 0;

		uint32 _size = -1;
	};

	GRAPHICS_DLL_DECLSPEC bool IsTypedBuffer(const BufferDesc& desc);
	GRAPHICS_DLL_DECLSPEC bool IsStructuredBuffer(const BufferDesc& desc);
	GRAPHICS_DLL_DECLSPEC bool IsByteAddressBuffer(const BufferDesc& desc);
}