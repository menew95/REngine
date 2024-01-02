#include <rengine_api.h>

#include <rengine/rengine.h>


static std::unique_ptr<rengine::REngine> g_pEngine = nullptr;

namespace rengine
{
	RENGINE_API rengine::REngine* CreateApplication()
	{
		g_pEngine = std::make_unique<rengine::REngine>();

		return g_pEngine.get();
	}

	RENGINE_API void DeleteApplication(rengine::REngine* application)
	{
		application->Quit();
	}

}
