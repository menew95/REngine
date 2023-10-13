#include <rengine\System\GraphicsSystem.h>
#include <rengine\core\window.h>

#include <graphics_core\GraphicsEngineAPI.h>
using namespace graphics;

namespace rengine
{
	DEFINE_SINGLETON_CLASS(GraphicsSystem, {}, {});

	void GraphicsSystem::Initialize()
	{
#ifdef _DEBUG
#ifdef x64
		auto _engineModule = LoadLibrary(_T("Graphics_Core_x64_Debug.dll"));
#else
		auto _engineModule = LoadLibrary(_T("Graphics_Core_x86_Debug.dll"));
#endif // x64
#else
#ifdef x64
		auto _engineModule = LoadLibrary(_T("Graphics_Core_x64_Release.dll"));
#else
		auto _engineModule = LoadLibrary(_T("Graphics_Core_x86_Release.dll"));
#endif // x64
#endif

		assert(_engineModule != NULL);

		auto _graphicsConstructor = (GraphicsConstructor)GetProcAddress(_engineModule, "CreateGraphicsEngine");

		assert(_graphicsConstructor != NULL);

		auto _graphicsDestructor = (GraphicsDestructor)GetProcAddress(_engineModule, "ReleaseGraphicsEngine");

		assert(_graphicsDestructor != NULL);

		m_pGraphicsEngine = _graphicsConstructor();

		GraphicsEngineDesc _desc;
		_desc._module = Module::DirectX11;
		_desc._hwnd = Window::GetInstance()->GetWindowInfo()._hWnd;
		_desc._extent._height = Window::GetInstance()->GetWindowInfo()._height;
		_desc._extent._width = Window::GetInstance()->GetWindowInfo()._width;

		m_pGraphicsEngine->Init(_desc);
	}

	void* GraphicsSystem::GetDevice()
	{
		return m_pGraphicsEngine->GetDevice();
	}

	void* GraphicsSystem::GetContext()
	{
		return m_pGraphicsEngine->GetContext();
	}

	void GraphicsSystem::Render()
	{
		m_pGraphicsEngine->Excute();
	}

	void GraphicsSystem::Present()
	{
		m_pGraphicsEngine->Present();
	}
}