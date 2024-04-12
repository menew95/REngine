/**

    @file      GraphicsEngine.h
    @brief     그래픽스 엔진 클래스
    @details   모듈과 상호작용 하는 클래스
    @author    LWT
    @date      12.10.2023

**/
#pragma once

#include <common\singleton.h>
#include <common\math.h>

#include <graphics_core\Graphics_core_dllexport.h>
#include <graphics_core\GraphicsEngineFlags.h>

namespace graphics
{
	class GraphicsEngine
	{
		DECLARE_SINGLETON_CLASS_EXPORT(GRAPHICS_API, GraphicsEngine);
	public:
		/**
			@brief 렌더링 모듈을 로드하고 초기화
			@param desc - graphics engine description
		**/
		GRAPHICS_API void Init(const GraphicsEngineDesc& desc);

		/**
			@brief graphics resource load
		**/
		GRAPHICS_API void LoadGraphicsResource();

		GRAPHICS_API void InitRenderPass();

		/**
		    @brief graphics engine 종료
		**/
		GRAPHICS_API void ReleaseEngine();

		/**
		    @brief  device를 반환
		    @retval  - device pointer
		**/
		GRAPHICS_API void* GetDevice();

		/**
		    @brief  device context를 반환
		    @retval  - conetxt pointer
		**/
		GRAPHICS_API void* GetContext();
		
		/**
			@brief excute render data
		**/
		GRAPHICS_API void Excute();

		GRAPHICS_API void Present();

		GRAPHICS_API void ResizeSwapchain(const Extent2D& resolution);

		const WindowInfo& GetWindowInfo() { return m_windowInfo; }

		GRAPHICS_API void PushCameraBuffer(class CameraBuffer* cam) { m_cameraBuffers.push_back(cam); }

		GRAPHICS_API void SetLightSetting(const LightSetting& setting);

		/**
			@brief  object id frame buffer로부터 해당 픽셀 값을 읽고 renderobject hash 값을 반환
			@param pixel	- 읽을 픽셀 위치
			@retval			- renderobject hash 값을 반환
		**/
		GRAPHICS_API uint64 ObjectPicking(const math::Vector2& pixel);

	private:
		
		/**
		    @brief module의 종류에 따른 dll 로드 및 render system 생성
		    @param desc - graphics engine description 
		**/
		void LoadModule(const GraphicsEngineDesc& desc);

		/**
		    @brief 생성한 렌더 시스템에서 swapchain과 commnad buffer 생성
		    @param desc - graphics engine description
		**/
		void CreateSwapChainAndCommandBuffer(const GraphicsEngineDesc& desc);

		class RenderSystem* m_pRenderSystem = nullptr;

		class SwapChain* m_pSwapChain = nullptr;

		class CommandBuffer* m_pCommandBuffer = nullptr;

		class Renderer* m_pRenderer = nullptr;

		vector<class CameraBuffer*> m_cameraBuffers;

		WindowInfo m_windowInfo;
	};
}