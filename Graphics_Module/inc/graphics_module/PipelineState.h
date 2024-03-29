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

		/**
		    @brief 머티리얼한테 파이프라인 리소스 레이아웃을 알려주기 위해 사용
		**/
		virtual struct PropertyDesc& GetPropertyDesc() abstract;

	protected:
	};
}