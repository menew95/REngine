#include <rengine\rengine_api.h>

#include <rengine\rengine.h>

static std::unique_ptr<rengine::REngine> g_pEngine = nullptr;

RENGINE_API rengine::REngine* CreateREngine()
{
	g_pEngine = std::make_unique<rengine::REngine>();

	return g_pEngine.get();
}

RENGINE_API bool QuitREngine()
{
	return g_pEngine->Quit();
}

bool UpdateREngine()
{
	return g_pEngine->Update();
}