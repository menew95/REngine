#pragma once

#include <common/common.h>

namespace graphics
{
	enum class API
	{
		DirectX11 = 1,
	};

	struct GraphicsEngineDesc
	{
		API _module;

		void* _hwnd;
		
		Extent2D _extent;
	};
}