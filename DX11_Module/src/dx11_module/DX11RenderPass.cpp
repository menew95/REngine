#include "dx11_module_pch.h"

#include "dx11_module\DX11RenderPass.h"

namespace graphics
{
	namespace DX11
	{
		DX11RenderPass::DX11RenderPass(const RenderPassDesc& desc)
		{
			m_PipelineState = reinterpret_cast<DX11PipelineState*>(desc._pipelineState);
			m_PipelineLayout = reinterpret_cast<DX11PipelineLayout*>(desc._pipelineLayout);
			m_RenderTarget = reinterpret_cast<DX11RenderTarget*>(desc._renderTarget);
		}
	}
}