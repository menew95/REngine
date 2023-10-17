#include <common/common.h>
#include <common/Module.h>

#include <graphics_core/GraphicsEngine.h>

#include <graphics_module/Module.h>
#include <graphics_module/RenderSystem.h>

#include <errhandlingapi.h>

std::unique_ptr<Module> m_pGraphicsModule;

namespace graphics
{
	void GraphicsEngine::Init(const GraphicsEngineDesc& desc)
	{
		LoadModule(desc);

		CreateSwapChainAndCommandBuffer(desc);
	}

	void GraphicsEngine::Release()
	{
		if (m_pGraphicsModule != nullptr)
		{
			m_pGraphicsModule.reset();
		}
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
		AttachmentClear _clear{math::Color::Black, 0};

		m_pCommandBuffer->SetViewport({0, 0, 1280, 720, 0, 1});

		m_pCommandBuffer->SetRenderTarget(*m_pSwapChain, 1, &_clear);

		//m_pCommandBuffer->ClearState();

	}

	void GraphicsEngine::Present()
	{
		m_pSwapChain->Present();
	}

	void GraphicsEngine::LoadModule(const GraphicsEngineDesc& desc)
	{
		switch (desc._module)
		{
		case API::DirectX11:
		{
			m_pGraphicsModule = Module::Load(Module::GetModuleFilename("DX11_Module").c_str());
			break;
		}
		default:
			break;
		}

		auto _renderSystemAlloc = (RenderSystemAlloc)m_pGraphicsModule->LoadProcedure("RenderSystem_Alloc");

		auto _renderSystemFree = (RenderSystemFree)m_pGraphicsModule->LoadProcedure("RenderSystem_Free");

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
		_swapChainDesc._resolution = desc._extent;
		_swapChainDesc._windowDesc._hwnd = desc._hwnd;

		m_pSwapChain = m_pRenderSystem->CreateSwapChain(TEXT("MainSwapChain"), _swapChainDesc);

		assert(m_pSwapChain != nullptr);

		CommandBufferDesc _commandBufferDesc;

		m_pCommandBuffer = m_pRenderSystem->CreateCommandBuffer(TEXT("MainCommandBuffer"), _commandBufferDesc);

		assert(m_pCommandBuffer != nullptr);
	}
}