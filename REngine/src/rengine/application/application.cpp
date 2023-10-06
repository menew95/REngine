#include <rengine/application/application.h>
#include <rengine/core/window.h>

#include <log/log.h>

namespace rengine
{
	DEFINE_SINGLETON_CLASS(Application, {},
		{
			/*if (m_IsCmdOn)
			{
				m_IsCmdOn = false;
				FreeConsole();
			}*/
		})


	bool Application::Update()
	{
		return false;
	}

	bool Application::Quit()
	{
		return false;
	}

	bool Application::Initallize(bool showCmd, HINSTANCE hInstance, const tstring& windowClassName, const tstring& windowName, UINT width, UINT height)
	{
		if (Log::IsInitialized() == false)
		{
			Log::Initialize();
		}

		bool _hr = S_FALSE;

		m_window = Window::GetInstance();
		
		_hr = m_window->Initialize(showCmd, hInstance, windowClassName, windowName, width, height);

		return _hr == S_OK;
	}

}