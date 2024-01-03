/**

    @file      GraphicsEngine.h
    @brief     그래픽스 엔진 클래스
    @details   모듈과 상호작용 하는 클래스
    @author    LWT
    @date      12.10.2023

**/
#pragma once

#include <graphics_core/graphics_core_dllexport.h>
#include <graphics_core\GraphicsEngineFlags.h>

namespace graphics
{
	class GRAPHICS_API GraphicsEngine
	{
	public:
		/**
			@brief 렌더링 모듈을 로드하고 초기화
			@param desc - graphics engine description
		**/
		void Init(const GraphicsEngineDesc& desc);

		/**
		    @brief graphics engine 종료
		**/
		void Release();

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

		WindowInfo m_windowInfo;
	};
}