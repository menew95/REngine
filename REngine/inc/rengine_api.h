#include <rengine/application/application.h>

namespace rengine
{
	extern "C"
	{
		RENGINE_API Application* CreateApplication();
		RENGINE_API void DeleteApplication(Application*);

		RENGINE_API bool UpdateApplication();

		using ApplicationConstructor = Application* (*)();
		using ApplicationDestructor = void (*)(Application*);
	}
}