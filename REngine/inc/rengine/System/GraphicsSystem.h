#pragma once

#include <common\common.h>

#include <common\math.h>

#include <common\singleton.h>

#include <rengine\rengine_dllexport.h>

#include <rengine\System\GraphicsSetting.h>

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

		void UnInitialize();

		/**
			@brief 그래픽스 파이프라인에서 사용될 그래픽스 리소스들을 로드
		**/
		void LoadGraphicsResource();

		/**
		    @brief 렌더 패스들을 생성하고 초기화
		**/
		void InitRenderPass();
		
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
			@brief  object id frame buffer로부터 해당 픽셀 값을 읽고 renderobject hash 값을 반환
			@param imageSize     - image view size
			@param mousePosition - click position in image view
			@retval  - renderobject hash 값을 반환
		**/
		RENGINE_API uint64 ObjectPicking(const math::Vector2& imageSize, const math::Vector2& mousePosition);

		/**
		    @brief 현재 씬에 적용된 graphics setting를 엔진에 적용
		**/
		RENGINE_API void SetGraphicsSetting(const GraphicsSetting& setting);

		/**
		    @brief Graphics Render
		**/
		void Render();

		void Present();

		void ResizeWindow(uint32 width, uint32 height);

	private:
		Vector2 m_screenSize = { 1280, 720 };

		graphics::GraphicsEngine* m_pGraphicsEngine = nullptr;

		//queue<CameraBuffer*> m_cameraQueue;
	};
}