#pragma once

#include <common/common.h>

#include <common/singleton.h>

namespace graphics
{
	class GraphicsEngine;
}

namespace rengine
{
	class GraphicsSystem
	{
		DECLARE_SINGLETON_CLASS(GraphicsSystem);

	public:

		/**
		    @brief Graphics Module 초기화
		**/
		void Initialize();

		/**
		    @brief  Module로부터 생성한 Device를 반환
		    @retval  - Device pointer
		**/
		void* GetDevice();

		/**
		    @brief  Module로부터 생성한 Context를 반환
		    @retval  - Context pointer
		**/
		void* GetContext();

		/**
		    @brief Graphics Render
		**/
		void Render();

		void Present();

	private:
		graphics::GraphicsEngine* m_pGraphicsEngine = nullptr;
	};
}