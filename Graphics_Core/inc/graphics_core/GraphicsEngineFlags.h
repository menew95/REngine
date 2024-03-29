/**

    @file      GraphicsEngineFlags.h
    @brief     graphics engine setting flags
    @details   ~
    @author    LWT
    @date      5.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

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
		API _module = API::DirectX11;

		WindowInfo _info;
	};

	struct LightSetting
	{
		
	};
}