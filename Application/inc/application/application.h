#pragma once

#include <common/common.h>
#include <common/singleton.h>

#include <common/AppBase.h>

#include <application_api.h>

#define EDITOR

#ifndef APPCLASS
#ifdef EDITOR
#define APPCLASS AppBase
#elif !defined(EDITOR)
#define APPCLASS rengine::REngine
#endif // DEBUG
#endif

using appConstructor = APPCLASS* (*)();
using appDestructor = void (*)(APPCLASS&);
using appInitialize = void (*)(APPCLASS&, void*);
using appUpdate = bool (*)(APPCLASS&);
using appQuit = bool (*)(APPCLASS&);
using appWndProc = bool (*)(APPCLASS&, HWND, UINT, WPARAM, LPARAM);

namespace app
{
	class Application
	{
		DECLARE_SINGLETON_CLASS(Application)

	public:
		APP_API bool Update();

		APP_API bool Quit();

		APP_API bool Initallize(bool showCmd,
			HINSTANCE hInstance,
			const tstring& windowClassName,
			const tstring& windowName,
			UINT width,
			UINT height);

		AppBase* GetApp() const
		{
			return m_pApp;
		}

	protected:
		double m_dTickTime = 0;
		double m_TickCnt = 0;

		AppBase* m_pApp;

		class Window* m_pWindow;
	};
}