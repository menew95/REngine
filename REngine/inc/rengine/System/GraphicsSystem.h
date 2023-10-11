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


		void Initialize();

	private:
		std::shared_ptr<graphics::GraphicsEngine*> m_pGraphicsEngine;

	};
}