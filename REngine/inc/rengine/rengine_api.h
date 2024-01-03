#include <rengine\rengine.h>

extern "C"
{
	RENGINE_API rengine::REngine* CreateREngine();

	RENGINE_API bool QuitREngine();

	RENGINE_API bool UpdateREngine();

	using REngineConstructor = rengine::REngine* (*)();

	using REngineDestructor = bool (*)();
}