#pragma once

#include "common\common.h"

#include "graphics_module\Window.h"

#include "graphics_module\graphics_module_dllexport.h"

#include "graphics_module\SwapChainFlags.h"

#include "graphics_module\RenderTarget.h"

struct IDXGIFactory;

namespace graphics
{
	class GRAPHICS_DLL_DECLSPEC SwapChain : public RenderTarget
	{
	public:
		SwapChain();
		SwapChain(SwapChainDesc& desc);
		virtual ~SwapChain();

		virtual void Present() abstract;

		virtual bool ResizeBuffer(const Extent2D& resolution) abstract;

		virtual bool SwitchFullscreen(bool enable) abstract;

		Window& GetWindow() const;

	private:
		struct Pimpl;
		Pimpl* m_Pimpl;
	};
}