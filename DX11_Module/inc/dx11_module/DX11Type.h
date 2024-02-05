#pragma once

#include "common/vertex_attribute.h"
#include "graphics_module\SwapChainFlags.h"
#include "graphics_module\PipelineStateFlags.h"
#include "graphics_module\TextureFlags.h"
#include "graphics_module\BufferFlags.h"
#include "graphics_module\SamplerFlags.h"
#include "graphics_module\RenderSystemFlags.h"
#include "dx11_module\Direct3D11.h"


namespace graphics
{
	namespace DX11
	{
		DXGI_FORMAT                 MapFormat(const DataType dataType);
		DXGI_FORMAT                 MapFormat(const Format format);
		D3D_PRIMITIVE_TOPOLOGY      MapPrimitive(const PrimitiveTopology topology);
		D3D11_FILL_MODE             MapFillMode(const FillMode polygonMode);
		D3D11_CULL_MODE             MapCullMode(const CullMode cullMode);
		D3D11_BLEND                 MapBlend(const Blend blend);
		D3D11_BLEND_OP              MapBlendOp(const BlendOp blendOp);
		D3D11_COMPARISON_FUNC       MapCompareOp(const CompareOp compareOp);
		D3D11_STENCIL_OP            MapStencilOp(const StencilOp stencilOp);
		D3D11_FILTER                MapFilter(const Filter samplerDesc);
		D3D11_TEXTURE_ADDRESS_MODE  MapSamplerMode(const SamplerAddressMode	addressMode);
		D3D11_MAP                   MapCpuFlag(const CPUAccess cpuAccess);

		uint32						MapBufferBindFlag(const uint32 bindFlags);
		uint32						MapTextureBindFlag(const uint32 bindFlags);
		uint32						MapCPUAcessFlagsForMiscFlags(const uint32 miscFlags);
		Format                      UnmapFormat(const DXGI_FORMAT format);

		//TODO: remove and use DXTypes namespace directly
		DXGI_FORMAT                 ToDXGIFormatDSV(const DXGI_FORMAT format);
		DXGI_FORMAT                 ToDXGIFormatSRV(const DXGI_FORMAT format);
		DXGI_FORMAT                 ToDXGIFormatUAV(const DXGI_FORMAT format);
		DXGI_FORMAT					ToDXGIFormatUINT(const DXGI_FORMAT format);

		bool IsTypelessDXGIFormat(const DXGI_FORMAT format);

		bool IsDepthStencilDXGIFormat(const DXGI_FORMAT format);

		void Convert(D3D11_DEPTH_STENCIL_DESC& dst, const DepthDesc& srcDepth, const StencilDesc& srcStencil);

		void Convert(D3D11_RASTERIZER_DESC& dst, const RasterizerDesc& src);

		void Convert(D3D11_BLEND_DESC& dst, const BlendDesc& src);

	}
}