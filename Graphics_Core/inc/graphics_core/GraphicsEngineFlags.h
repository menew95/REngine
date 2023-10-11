#pragma once

namespace graphics
{
	enum class Module
	{
		DirectX11 = 1,
	};

	struct GraphicsEngineDesc
	{
		Module _module;
	};
}