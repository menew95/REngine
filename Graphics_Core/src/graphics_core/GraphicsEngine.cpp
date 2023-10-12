#include <common/common.h>

#include <graphics_core/GraphicsEngine.h>

#include <graphics_module/Module.h>
#include <graphics_module/RenderSystem.h>

#include <errhandlingapi.h>

namespace graphics
{
	void GraphicsEngine::Init(const GraphicsEngineDesc& desc)
	{
		LoadModule(desc);

		CreateSwapChainAndCommandBuffer(desc);
	}

	void GraphicsEngine::Release()
	{

	}
	void* GraphicsEngine::GetDevice()
	{
		return m_pRenderSystem->GetDevice();
	}
	void* GraphicsEngine::GetContext()
	{
		return m_pRenderSystem->GetDeviceContext();
	}
	void GraphicsEngine::Excute()
	{
		m_pSwapChain->Present();
	}

	void GraphicsEngine::LoadModule(const GraphicsEngineDesc& desc)
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

		auto _engineModule = LoadLibrary(_module.c_str());

		assert(_engineModule != NULL);

		auto _renderSystemAlloc = (RenderSystemAlloc)GetProcAddress(_engineModule, "RenderSystem_Alloc");

		assert(_renderSystemAlloc != NULL);

		auto _renderSystemFree = (RenderSystemFree)GetProcAddress(_engineModule, "RenderSystem_Free");

		assert(_renderSystemFree != NULL);

		RenderSystemDesc _renderSystemDesc;
		{
			_renderSystemDesc._moduleName = "DirectX11";
		}

		m_pRenderSystem = reinterpret_cast<RenderSystem*>(_renderSystemAlloc(&_renderSystemDesc, 0));
	}
	void GraphicsEngine::CreateSwapChainAndCommandBuffer(const GraphicsEngineDesc& desc)
	{
		SwapChainDesc _swapChainDesc;
		_swapChainDesc._fullScreen = false;
		_swapChainDesc._resolution = { 1280, 720 };
		_swapChainDesc._windowDesc._hwnd = desc._handle;

		m_pSwapChain = m_pRenderSystem->CreateSwapChain(TEXT("MainSwapChain"), _swapChainDesc);

		assert(m_pSwapChain != nullptr);

		CommandBufferDesc _commandBufferDesc;

		m_pCommandBuffer = m_pRenderSystem->CreateCommandBuffer(TEXT("MainCommandBuffer"), _commandBufferDesc);

		assert(m_pCommandBuffer != nullptr);
	}
}