#pragma once

#include <graphics_module/graphics_module_dllexport.h>

namespace graphics
{
	class GRAPHICS_DLL_DECLSPEC RenderSystemObject
	{
	public:
		virtual void SetName(const char* name) {}
	};
}