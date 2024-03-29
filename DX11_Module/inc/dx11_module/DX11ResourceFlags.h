﻿#pragma once

#include "graphics_module\BufferFlags.h"
#include "graphics_module\TextureFlags.h"
#include <dx11_module/Direct3D11.h>

namespace graphics
{
	uint32 DXGetBufferBindFlags(long bindFlags);
	uint32 DXGetTextureBindFlags(const TextureDesc& desc);

	bool DXBindFlagsNeedBufferWithRV(long bindFlags);

	uint32 DXGetCPUAccessFlagsForMiscFlags(long miscFlags);
	uint32 DXGetCPUAccessFlags(long cpuAccessFlags);

	uint32 DXGetBufferMiscFlags(const BufferDesc& desc);
	uint32 DXGetTextureMiscFlags(const TextureDesc& desc);

	D3D11_USAGE DXGetBufferUsage(const BufferDesc& desc);
	D3D11_USAGE DXGetCPUAccessBufferUsage(const BufferDesc& desc);
	D3D11_USAGE DXGetTextureUsage(const TextureDesc& desc);

	D3D11_MAP DXGetMapWrite(bool writeDiscard);
}