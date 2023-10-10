#pragma once

#include "graphics_module\graphics_module_dllexport.h"

#include "graphics_module\RenderPassFlags.h"

namespace graphics
{
	class GRAPHICS_DLL_DECLSPEC RenderPass
	{
	public:
		RenderPass() = default;
		virtual ~RenderPass() = default;

	private:
	};
}