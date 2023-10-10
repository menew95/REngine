#pragma once

#include "graphics_core\GraphicsEngine.h"
#include "graphics_core\graphics_core_dllexport.h"

namespace graphics
{
	extern "C"
	{
		GRAPHICS_API GraphicsEngine* CreateGraphicsEngine();
		GRAPHICS_API void ReleaseGraphicsEngine(GraphicsEngine*);

		GRAPHICS_API bool UpdateApplication();

		using ApplicationConstructor = GraphicsEngine * (*)();
		using ApplicationDestructor = void (*)(GraphicsEngine*);
	}
}