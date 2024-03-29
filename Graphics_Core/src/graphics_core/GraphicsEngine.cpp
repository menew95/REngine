﻿#include <common/common.h>
#include <common/Module.h>

#include <graphics_core\GraphicsEngine.h>
#include <graphics_core\ResourceManager.h>
#include <graphics_core\renderer\Renderer.h>
#include <graphics_core\resource\CameraBuffer.h>
#include <graphics_core\renderpass\SkyBoxRenderPass.h>

#include <graphics_module\Module.h>
#include <graphics_module\RenderSystem.h>

#include <errhandlingapi.h>

std::unique_ptr<Module> m_pGraphicsModule;

namespace graphics
{
	DEFINE_SINGLETON_CLASS(GraphicsEngine,
		{

		},
		{

		},
		{
			if (m_pGraphicsModule != nullptr)
			{
				m_pGraphicsModule.reset();

				GraphicsEngine::GetInstance()->ReleaseEngine();
			}
		});

	void GraphicsEngine::Init(const GraphicsEngineDesc& desc)
	{
		LoadModule(desc);

		CreateSwapChainAndCommandBuffer(desc);
		
		m_windowInfo = desc._info;

		ResourceManager::GetInstance()->Initialze(m_pRenderSystem);
	}

	void GraphicsEngine::LoadGraphicsResource()
	{
		ResourceManager::GetInstance()->LoadGraphicsResource();

		m_pRenderer = new Renderer(m_pCommandBuffer);
	}

	void GraphicsEngine::ReleaseEngine()
	{
		delete m_pRenderer;
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
		m_pCommandBuffer->SetViewport({0, 0, (float)m_windowInfo._width , (float)m_windowInfo._height });

		m_pRenderer->SetFrameResource();

		for (auto* _camBuf : m_cameraBuffers)
		{
			// 카메라가 꺼져 있을 경우 렌더링을 하지 않음
			if(!_camBuf->GetEnable())
				continue;

			Renderer::GetInstance()->SetCamera(_camBuf);

			for (auto& _renderPass : _camBuf->GetRenderPassList())
			{
				_renderPass->BeginExcute(m_pCommandBuffer, _camBuf);

				_renderPass->Excute(m_pCommandBuffer);

				_renderPass->EndExcute(m_pCommandBuffer);
			}

			/*auto* _skyBox = ResourceManager::GetInstance()->GetRenderPass(TEXT("SkyBox"));

			_skyBox->BeginExcute(m_pCommandBuffer, _camBuf);

			_skyBox->Excute(m_pCommandBuffer);

			_skyBox->EndExcute(m_pCommandBuffer);*/
		}

		AttachmentClear _clear{ math::Color::Black, 0 };

		m_pCommandBuffer->SetViewport({ 0, 0, static_cast<float>(m_windowInfo._width), static_cast<float>(m_windowInfo._height), 0, 1 });

		m_pCommandBuffer->SetRenderTarget(*m_pSwapChain, 1, &_clear);

		m_cameraBuffers.clear();
	}

	void GraphicsEngine::Present()
	{
		m_pSwapChain->Present();

		m_pCommandBuffer->ClearState();
	}

	void GraphicsEngine::ResizeSwapchain(const Extent2D& resolution)
	{
		m_windowInfo._width = resolution._width;
		m_windowInfo._height = resolution._height;

		m_pSwapChain->ResizeBuffer(resolution);
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
		_swapChainDesc._resolution = { desc._info._width, desc._info._height } ;
		_swapChainDesc._windowDesc._hwnd = desc._info._hWnd;

		m_pSwapChain = m_pRenderSystem->CreateSwapChain(TEXT("MainSwapChain"), _swapChainDesc);

		m_pSwapChain->SetName("MainSwapChain");

		assert(m_pSwapChain != nullptr);

		CommandBufferDesc _commandBufferDesc;

		m_pCommandBuffer = m_pRenderSystem->CreateCommandBuffer(TEXT("MainCommandBuffer"), _commandBufferDesc);

		assert(m_pCommandBuffer != nullptr);
	}
}