#include <graphics_core\GraphicsEngine_api.h>

namespace graphics
{
	GRAPHICS_API GraphicsEngine* CreateGraphicsEngine()
	{
		return GraphicsEngine::GetInstance();
	}

	GRAPHICS_API void ReleaseGraphicsEngine(GraphicsEngine* engine)
	{
		GraphicsEngine::Release();
	}

	GRAPHICS_API bool UpdateApplication()
	{
		return false;
	}
}