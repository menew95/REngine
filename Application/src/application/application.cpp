#include <application/application.h>
#include <application/window.h>

#include <editor/Editor.h>

#include <rengine/application/application.h>

#include <common/Module.h>

std::unique_ptr<Module> g_pModule;

APPCLASS* g_pApp;

appConstructor _constructor;
appDestructor _destructor;
appInitialize _initialize;
appUpdate _update;
appQuit _quit;
appWndProc _wndProc;

namespace app
{
	DEFINE_SINGLETON_CLASS(Application, 
		: m_pWindow(nullptr)
		{
		}, 

		{
		},

		{
			/*if (m_IsCmdOn)
			{
				m_IsCmdOn = false;
				FreeConsole();
			}*/
		})


	bool Application::Update()
	{
		return _update(*g_pApp);
	}

	bool Application::Quit()
	{
		return _quit(*g_pApp);
	}

	bool Application::Initallize(bool showCmd, HINSTANCE hInstance, const tstring& windowClassName, const tstring& windowName, UINT width, UINT height)
	{
		HRESULT _hr = S_OK;

#ifdef EDITOR
		g_pModule = Module::Load(Module::GetModuleFilename("Editor").c_str());

		_initialize = (void (*)(APPCLASS&, const editor::EditorDesc&))g_pModule->LoadProcedure("?Initialize@Editor@editor@@QEAAXAEBUEditorDesc@2@@Z");
		_update = (bool (*)(APPCLASS&))g_pModule->LoadProcedure("?Update@Editor@editor@@QEAAXXZ");
		_quit = (bool (*)(APPCLASS&))g_pModule->LoadProcedure("?Quit@Editor@editor@@QEAAXXZ");
		_wndProc = (bool (*)(APPCLASS&, HWND, UINT, WPARAM, LPARAM))g_pModule->LoadProcedure("?WndProc@Editor@editor@@QEAA_NPEAUHWND__@@I_K_J@Z");

		_constructor = (APPCLASS* (*)())g_pModule->LoadProcedure("?WndProc@Editor@editor@@QEAA_NPEAUHWND__@@I_K_J@Z");
		_destructor = (void (*)(APPCLASS&))g_pModule->LoadProcedure("?WndProc@Editor@editor@@QEAA_NPEAUHWND__@@I_K_J@Z");
#elif !defined(EDITOR)
		g_pModule = Module::Load(Module::GetModuleFilename("REngine").c_str());

		_Initialize = (void (*)(APPCLASS&, const editor::EditorDesc&))g_pModule->LoadProcedure("?Initialize@REngine@rengine@@QEAAXAEBUEditorDesc@2@@Z");
		_Update = (bool (*)(APPCLASS&))g_pModule->LoadProcedure("?Update@REngine@rengine@@QEAAXXZ");
		_quit = (bool (*)(APPCLASS&))g_pModule->LoadProcedure("?Quit@Editor@editor@@QEAAXXZ");
		_WndProc = (bool (*)(APPCLASS&, HWND, UINT, WPARAM, LPARAM))g_pModule->LoadProcedure("?WndProc@REngine@rengine@@QEAA_NPEAUHWND__@@I_K_J@Z");

		_constructor = (APPCLASS* (*)())g_pModule->LoadProcedure("?WndProc@Editor@editor@@QEAA_NPEAUHWND__@@I_K_J@Z");
		_destructor = (void (*)(APPCLASS&))g_pModule->LoadProcedure("?WndProc@Editor@editor@@QEAA_NPEAUHWND__@@I_K_J@Z");
#endif // DEBUG

		m_pWindow = Window::GetInstance();

		_hr = m_pWindow->Initialize(showCmd, hInstance, windowClassName, windowName, width, height, _wndProc);

		g_pApp = _constructor();


		return _hr == S_OK;
	}

}