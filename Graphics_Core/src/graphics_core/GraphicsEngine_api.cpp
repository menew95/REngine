﻿#include <graphics_core\GraphicsEngine_api.h>

namespace graphics
{
	static GraphicsEngine g_pEngine;

	GRAPHICS_API GraphicsEngine* CreateGraphicsEngine()
	{
		return &g_pEngine;
	}

	GRAPHICS_API void ReleaseGraphicsEngine(GraphicsEngine* engine)
	{
		engine->Release();
	}

	GRAPHICS_API bool UpdateApplication()
	{
		return false;
	}
}