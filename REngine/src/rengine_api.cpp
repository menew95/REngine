#include <rengine_api.h>

namespace rengine
{

	RENGINE_API Application* CreateApplication()
	{
		auto _application = Application::GetInstance();

		return _application;
	}

	RENGINE_API void DeleteApplication(Application* application)
	{
		
	}

}
