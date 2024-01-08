/**

    @file      RenderPassFlags.h
    @brief     
    @details   ~
    @author    Cool Guy
    @date      8.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

namespace graphics
{
	struct RenderPassDesc
	{
		tstring					_passName = TEXT("Render Pass");

		class PipelineState*	_pipelineState = nullptr;
		class PipelineLayout*	_pipelineLayout = nullptr;
	};
}