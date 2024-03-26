#pragma once

#include "common\common.h"

#include "graphics_module\RenderSystemObject.h"

#include "graphics_module\PipelineStateFlags.h"

namespace graphics
{
	class GRAPHICS_DLL_DECLSPEC PipelineState : public RenderSystemObject
	{
	public:
		PipelineState() = default;
		virtual ~PipelineState() = default;

		virtual void SetShaderObjects(const ShaderProgram& shaderProgram) abstract;

	protected:

	};
}