#pragma once

#include "graphics_core\GraphicsEngine.h"
#include "graphics_core\graphics_core_dllexport.h"
#include "graphics_core\GraphicsEngineFlags.h"

namespace graphics
{
	extern "C"
	{
		GRAPHICS_API GraphicsEngine* CreateGraphicsEngine();
		GRAPHICS_API void ReleaseGraphicsEngine(GraphicsEngine*);

		GRAPHICS_API bool UpdateApplication();

		using GraphicsConstructor = GraphicsEngine * (*)();
		using GraphicsDestructor = void (*)(GraphicsEngine*);
	}
}