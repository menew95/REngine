#include <Editor_pch.h>
#include <editor\Core\EnginePlugin.h>
#include <common\Module.h>

std::unique_ptr<Module> g_engineModule;

namespace editor
{

	DEFINE_SINGLETON_CLASS(EnginePlugin, 
		{

		},
		{

		},
		{

		})

	void EnginePlugin::Initialize(rengine::Application* app)
	{
		/*g_engineModule = Module::Load(Module::GetModuleFilename("Editor").c_str());

		auto _graphicsConstructor = g_engineModule->LoadProcedure("CreateGraphicsEngine");

		auto _graphicsDestructor = g_engineModule->LoadProcedure("ReleaseGraphicsEngine");

		m_pGraphicsEngine = _graphicsConstructor();

		GraphicsEngineDesc _desc;
		_desc._module = API::DirectX11;
		_desc._hwnd = Window::GetInstance()->GetWindowInfo()._hWnd;
		_desc._extent._height = Window::GetInstance()->GetWindowInfo()._height;
		_desc._extent._width = Window::GetInstance()->GetWindowInfo()._width;

		m_pGraphicsEngine->Init(_desc);*/
	}
}