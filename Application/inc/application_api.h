/**

    @file      ApplicationAPI.h
    @brief     export
    @author    LWT
    @date      12.10.2023

**/
#pragma once

#ifdef APP_EXPORT
#ifndef APP_API
#define APP_API __declspec(dllexport)
#endif
#else
#ifndef APP_API
#define APP_API __declspec(dllimport)
#endif
#endif

#include <application\application.h>

namespace app
{
	class Application;

	extern "C"
	{
		APP_API Application* CreateApplication();

		APP_API bool QuitApplication();

		APP_API bool UpdateApplication();

		using ApplicationConstructor = Application * (*)();

		using ApplicationDestructor = bool (*)();
	}
}
