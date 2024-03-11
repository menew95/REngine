/**

    @file      GraphicsEngine.h
    @brief     그래픽스 엔진 클래스
    @details   모듈과 상호작용 하는 클래스
    @author    LWT
    @date      12.10.2023

**/
#pragma once

#include <common\singleton.h>

#include <graphics_core\Graphics_core_dllexport.h>
#include <graphics_core\GraphicsEngineFlags.h>

namespace graphics
{
	class GRAPHICS_API GraphicsEngine
	{
		DECLARE_SINGLETON_CLASS(GraphicsEngine);
	public:
		/**
			@brief 렌더링 모듈을 로드하고 초기화
			@param desc - graphics engine description
		**/
		void Init(const GraphicsEngineDesc& desc);

		/**
			@brief graphics resource load
		**/
		void LoadGraphicsResource();

		/**
		    @brief graphics engine 종료
		**/
		void ReleaseEngine();

		/**
		    @brief  device를 반환
		    @retval  - device pointer
		**/
		void* GetDevice();

		/**
		    @brief  device context를 반환
		    @retval  - conetxt pointer
		**/
		void* GetContext();
		
		/**
			@brief excute render data
		**/
		void Excute();

		void Present();

		void ResizeSwapchain(const Extent2D& resolution);

		const WindowInfo& GetWindowInfo() { return m_windowInfo; }

		void PushCameraBuffer(class CameraBuffer* cam) { m_cameraBuffers.push_back(cam); }

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