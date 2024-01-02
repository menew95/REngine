#include <rengine/rengine.h>

namespace rengine
{
	extern "C"
	{
		RENGINE_API rengine::REngine* CreateApplication();
		RENGINE_API void DeleteApplication(rengine::REngine*);

		RENGINE_API bool UpdateApplication();

		using ApplicationConstructor = rengine::REngine* (*)();
		using ApplicationDestructor = void (*)(rengine::REngine*);
	}
}