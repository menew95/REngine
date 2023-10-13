#pragma once

#include <common/common.h>

namespace graphics
{
	enum class Module
	{
		DirectX11 = 1,
	};

	struct GraphicsEngineDesc
	{
		Module _module;

		void* _hwnd;
		
		Extent2D _extent;
	};
}