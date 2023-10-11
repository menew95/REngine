#pragma once

#include <graphics_core/graphics_core_dllexport.h>
#include <graphics_core\GraphicsEngineFlags.h>

namespace graphics
{
	class GRAPHICS_API GraphicsEngine
	{
	public:
		void Init(const GraphicsEngineDesc& desc);

		void Release();

	private:
		
	};
}