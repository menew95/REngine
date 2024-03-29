﻿#include <common\Module.h>

#include <rengine\system\GraphicsSystem.h>
//#include <rengine\core\window.h>

#include <graphics_core\GraphicsEngine_api.h>
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

	void GraphicsSystem::Initialize(const WindowInfo& wininfo)
	{
		g_pGraphicsCoreModule = Module::Load(Module::GetModuleFilename("Graphics_Core").c_str());

		auto _graphicsConstructor = (GraphicsConstructor)g_pGraphicsCoreModule->LoadProcedure("CreateGraphicsEngine");

		auto _graphicsDestructor = (GraphicsDestructor)g_pGraphicsCoreModule->LoadProcedure("ReleaseGraphicsEngine");

		m_pGraphicsEngine = _graphicsConstructor();

		GraphicsEngineDesc _desc;
		_desc._module = API::DirectX11;
		_desc._info = wininfo;

		m_pGraphicsEngine->Init(_desc);
	}

	void GraphicsSystem::LoadGraphicsResource()
	{
		m_pGraphicsEngine->LoadGraphicsResource();
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

	void rengine::GraphicsSystem::ResizeWindow(uint32 width, uint32 height)
	{
		Extent2D _resolution{ width , height};

		m_pGraphicsEngine->ResizeSwapchain(_resolution);
	}
}