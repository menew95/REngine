#include <application/application.h>
#include <application/window.h>

#if defined(EDITOR)
#include <editor/Editor.h>
#else
#include <rengine/rengine.h>
#endif

#include <common/Module.h>

std::unique_ptr<Module> g_pModule;

using appConstructor = AppBase * (*)();

appConstructor _constructor;

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
		return m_pApp->Update();
	}

	bool Application::Quit()
	{
		return m_pApp->Quit();
	}

	bool Application::Initallize(bool showCmd, HINSTANCE hInstance, const tstring& windowClassName, const tstring& windowName, UINT width, UINT height)
	{
		HRESULT _hr = S_OK;

#ifdef EDITOR
		g_pModule = Module::Load(Module::GetModuleFilename("Editor").c_str());

		_constructor = (appConstructor)g_pModule->LoadProcedure("CreateEditor");

		editor::EditorDesc _desc;
#elif !defined(EDITOR)
		g_pModule = Module::Load(Module::GetModuleFilename("REngine").c_str());

		_constructor = (appConstructor)g_pModule->LoadProcedure("CreateREngine");

		rengine::REngineDesc _desc;
#endif // DEBUG

		m_pWindow = Window::GetInstance();

		_hr = Window::GetInstance()->Initialize(showCmd, hInstance, windowClassName, windowName, width, height);

		_desc._windowInfo = Window::GetInstance()->GetWindowInfo();

		m_pApp = _constructor();
		
		m_pApp->Initialize(&_desc);

		return _hr == S_OK;
	}

}