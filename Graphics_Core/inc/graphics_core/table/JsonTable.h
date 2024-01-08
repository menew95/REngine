/**

    @file      JsonTable.h
    @brief     json table loader class
    @details   그래픽스 리소스 테이블 로딩
    @author    lWT
    @date      8.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\math.h>

namespace graphics
{
	class ResourceManager;

	class TableLoader
	{
	public:
		static bool LoadRenderTargetTable(ResourceManager* resourceManager, const math::Vector2& screenSize);

		static bool LoadShaderTable(ResourceManager* resourceManager);

		static bool LoadPipelineStateTable(ResourceManager* resourceManager);

		static bool LoadRenderingPipelineTable(ResourceManager* resourceManager);

		static bool LoadTextureTable(ResourceManager* resourceManager);

		static bool LoadSamplerTable(ResourceManager* resourceManager);

		static bool LoadBufferTable(ResourceManager* resourceManager);

		static bool LoadRenderPassTable(ResourceManager* resourceManager);
	};
}