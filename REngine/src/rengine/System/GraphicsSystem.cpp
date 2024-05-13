#include <common\Module.h>

#include <rengine\system\GraphicsSystem.h>
#include <rengine\system\Time.h>

#include <rengine\core\EventManager.h>

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

		m_screenSize = { (float)wininfo._width, (float)wininfo._height };

		m_pGraphicsEngine->Init(_desc);
	}

	void GraphicsSystem::LoadGraphicsResource()
	{
		m_pGraphicsEngine->LoadGraphicsResource();
	}

	void GraphicsSystem::InitRenderPass()
	{
		m_pGraphicsEngine->InitRenderPass();
	}

	void* GraphicsSystem::GetDevice()
	{
		return m_pGraphicsEngine->GetDevice();
	}

	void* GraphicsSystem::GetContext()
	{
		return m_pGraphicsEngine->GetContext();
	}

	uint64 GraphicsSystem::ObjectPicking(const Vector2& imageSize, const Vector2& mousePosition)
	{
		if ((mousePosition.x <= 0 || mousePosition.y <= 0.f))
			return uint64();

		const math::Vector2 sizeRatio = Vector2{1280, 720} / imageSize;

		uint64 _renderObjID = m_pGraphicsEngine->ObjectPicking(mousePosition * sizeRatio);

		return _renderObjID;
	}

	void rengine::GraphicsSystem::SetGraphicsSetting(const GraphicsSetting& setting)
	{
		graphics::GraphicsSetting _setting;

#pragma region ScreenSpaceReflectionSetting

		_setting._postProcessingSetting._screenSpaceReflectionSetting._stride =
			setting._postProcessingSetting._screenSpaceReflectionSetting._stride;

		_setting._postProcessingSetting._screenSpaceReflectionSetting._maxSteps =
			setting._postProcessingSetting._screenSpaceReflectionSetting._maxSteps;

		_setting._postProcessingSetting._screenSpaceReflectionSetting._maxDistance =
			setting._postProcessingSetting._screenSpaceReflectionSetting._maxDistance;

		_setting._postProcessingSetting._screenSpaceReflectionSetting._strideZCutoff =
			setting._postProcessingSetting._screenSpaceReflectionSetting._strideZCutoff;

		_setting._postProcessingSetting._screenSpaceReflectionSetting._numMips =
			setting._postProcessingSetting._screenSpaceReflectionSetting._numMips;

		_setting._postProcessingSetting._screenSpaceReflectionSetting._fadeStart =
			setting._postProcessingSetting._screenSpaceReflectionSetting._fadeStart;

		_setting._postProcessingSetting._screenSpaceReflectionSetting._fadeEnd =
			setting._postProcessingSetting._screenSpaceReflectionSetting._fadeEnd;

		_setting._postProcessingSetting._screenSpaceReflectionSetting._zThickness =
			setting._postProcessingSetting._screenSpaceReflectionSetting._zThickness;

#pragma endregion

#pragma region ExponentialHeightFogSettig

		_setting._postProcessingSetting._exponentialHeightFogSettig._fogDensity =
			setting._postProcessingSetting._exponentialHeightFogSettig._fogDensity;

		_setting._postProcessingSetting._exponentialHeightFogSettig._fogMaxOpacity =
			setting._postProcessingSetting._exponentialHeightFogSettig._fogMaxOpacity;

		_setting._postProcessingSetting._exponentialHeightFogSettig._fogStartDistance =
			setting._postProcessingSetting._exponentialHeightFogSettig._fogStartDistance;

		_setting._postProcessingSetting._exponentialHeightFogSettig._fogCutOffDistance =
			setting._postProcessingSetting._exponentialHeightFogSettig._fogCutOffDistance;

		_setting._postProcessingSetting._exponentialHeightFogSettig._fogHeightFalloff =
			setting._postProcessingSetting._exponentialHeightFogSettig._fogHeightFalloff;

		_setting._postProcessingSetting._exponentialHeightFogSettig._fogHeight =
			setting._postProcessingSetting._exponentialHeightFogSettig._fogHeight;

		_setting._postProcessingSetting._exponentialHeightFogSettig._fogScatteringColor =
			setting._postProcessingSetting._exponentialHeightFogSettig._fogScatteringColor;

		_setting._postProcessingSetting._exponentialHeightFogSettig._lightScatterFlag =
			setting._postProcessingSetting._exponentialHeightFogSettig._lightScatterFlag;
			
#pragma endregion

#pragma region RimLightSetting

		_setting._postProcessingSetting._rimLightSetting._clipPoint =
			setting._postProcessingSetting._rimLightSetting._clipPoint;

		_setting._postProcessingSetting._rimLightSetting._rimDecrease =
			setting._postProcessingSetting._rimLightSetting._rimDecrease;

#pragma endregion

#pragma region BloomSetting

		_setting._postProcessingSetting._bloomSetting._intensity =
			setting._postProcessingSetting._bloomSetting._intensity;

		_setting._postProcessingSetting._bloomSetting._threshhold =
			setting._postProcessingSetting._bloomSetting._threshhold;

#pragma endregion

#pragma region ToneMappingSetting

		_setting._postProcessingSetting._toneMappingSetting._exposure = 
			setting._postProcessingSetting._toneMappingSetting._exposure;

#pragma endregion

		m_pGraphicsEngine->SetGraphicsSetting(_setting);
	}

	void GraphicsSystem::Render()
	{
		graphics::SceneInfo _currentSceneInfo;

		_currentSceneInfo._deltaTime = (float)Time::GetDeltaTime();
		_currentSceneInfo._timeStep = (float)Time::GetGameTime();

		// 1. SceneRender Event Invoke
		EventManager::GetInstance()->InvokeEvent(TEXT("SceneRendering"));

		// 2. SceneLighting Event Invoke
		EventManager::GetInstance()->InvokeEvent(TEXT("SceneLighting"));

		// 2. SceneLighting Event Invoke
		EventManager::GetInstance()->InvokeEvent(TEXT("SceneDebug"));

		m_pGraphicsEngine->Excute(_currentSceneInfo);
	}

	void GraphicsSystem::Present()
	{
		m_pGraphicsEngine->Present();
	}

	void rengine::GraphicsSystem::ResizeWindow(uint32 width, uint32 height)
	{
		Extent2D _resolution{ width , height};

		m_screenSize = { (float)width , (float)height };

		m_pGraphicsEngine->ResizeSwapchain(_resolution);
	}
}