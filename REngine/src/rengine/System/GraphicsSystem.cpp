#include <common\Module.h>

#include <rengine\System\GraphicsSystem.h>
//#include <rengine\core\window.h>

#include <graphics_core\GraphicsEngineAPI.h>
using namespace graphics;

std::unique_ptr<Module> g_pGraphicsCoreModule;

namespace rengine
{
	DEFINE_SINGLETON_CLASS(GraphicsSystem,
	{

	},
	{

	},
	{
		if (g_pGraphicsCoreModule != nullptr)
		{
			g_pGraphicsCoreModule.reset();
		}
	});

	void GraphicsSystem::Initialize()
	{
		g_pGraphicsCoreModule = Module::Load(Module::GetModuleFilename("Graphics_Core").c_str());

		auto _graphicsConstructor = (GraphicsConstructor)g_pGraphicsCoreModule->LoadProcedure("CreateGraphicsEngine");

		auto _graphicsDestructor = (GraphicsDestructor)g_pGraphicsCoreModule->LoadProcedure("ReleaseGraphicsEngine");

		m_pGraphicsEngine = _graphicsConstructor();

		GraphicsEngineDesc _desc;
		_desc._module = API::DirectX11;
		//_desc._hwnd = Window::GetInstance()->GetWindowInfo()._hWnd;
		//_desc._extent._height = Window::GetInstance()->GetWindowInfo()._height;
		//_desc._extent._width = Window::GetInstance()->GetWindowInfo()._width;

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