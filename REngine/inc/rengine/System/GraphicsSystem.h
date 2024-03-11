#pragma once

#include <common\common.h>

#include <common\singleton.h>

#include <rengine\rengine_dllexport.h>

namespace graphics
{
	class GraphicsEngine;
	class Texture;
	class CameraBuffer;
}

namespace rengine
{
	class GraphicsSystem
	{
		DECLARE_SINGLETON_CLASS_EXPORT(RENGINE_API, GraphicsSystem);

	public:

		/**
		    @brief Graphics Module 초기화
		**/
		void Initialize(const WindowInfo& wininfo);

		/**
			@brief 그래픽스 파이프라인에서 사용될 그래픽스 리소스들을 로드
		**/
		void LoadGraphicsResource();
		
		/**
		    @brief  Module로부터 생성한 Device를 반환
		    @retval  - Device pointer
		**/
		RENGINE_API void* GetDevice();

		/**
		    @brief  Module로부터 생성한 Context를 반환
		    @retval  - Context pointer
		**/
		RENGINE_API void* GetContext();

		/**
		    @brief Graphics Render
		**/
		void Render();

		void Present();

		void ResizeWindow(uint32 width, uint32 height);

	private:
		graphics::GraphicsEngine* m_pGraphicsEngine = nullptr;

		//queue<CameraBuffer*> m_cameraQueue;
	};
}