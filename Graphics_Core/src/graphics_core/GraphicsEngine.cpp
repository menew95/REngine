#include <common/common.h>

#include <graphics_core/GraphicsEngine.h>

#include <graphics_module/Module.h>
#include <graphics_module/RenderSystem.h>

#include <errhandlingapi.h>

namespace graphics
{
	void GraphicsEngine::Init(const GraphicsEngineDesc& desc)
	{
		tstring _module;

		switch (desc._module)
		{
			case Module::DirectX11:
			{
#ifdef _DEBUG
#ifdef x64
				_module = _T("DX11_Module_x64_Debug.dll");
#else
				_module = _T("DX11_Module_x86_Debug.dll");
#endif // x64
#else
#ifdef x64
				_module = _T("DX11_Module_x64_Release.dll");
#else
				_module = _T("DX11_Module_x86_Release.dll");
#endif // x64
#endif
				break;
			}
			default:
				break;
		}

		//auto _engineModule = LoadLibrary(_module.c_str());
		auto _engineModule = LoadLibrary(_T("DX11_Module_x64_Debug.dll"));

		DWORD error_code;

		error_code = GetLastError();

		assert(_engineModule != NULL);

		auto _renderSystemAlloc = (RenderSystemAlloc)GetProcAddress(_engineModule, "RenderSystem_Alloc");

		assert(_renderSystemAlloc != NULL);

		auto _renderSystemFree = (RenderSystemFree)GetProcAddress(_engineModule, "RenderSystem_Free");

		assert(_renderSystemFree != NULL);

		RenderSystemDesc _renderSystemDesc;
		{
			_renderSystemDesc._moduleName = "DirectX11";
		}

		auto* _application = _renderSystemAlloc(&_renderSystemDesc, 0);
	}

	void GraphicsEngine::Release()
	{

	}
}