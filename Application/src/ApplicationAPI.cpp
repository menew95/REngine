#include <ApplicationAPI.h>

#include <application/application.h>

namespace app
{
	APP_API Application* CreateApplication()
	{
		auto _application = Application::GetInstance();

		return _application;
	}

	APP_API bool QuitApplication()
	{
		return Application::GetInstance()->Quit();
	}

	APP_API bool UpdateApplication()
	{
		return Application::GetInstance()->Update();
	}
}