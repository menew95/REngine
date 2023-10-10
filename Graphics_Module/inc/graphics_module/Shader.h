#pragma once

#include "graphics_module\RenderSystemObject.h"

#include "graphics_module\ShaderFlags.h"

namespace graphics
{
	class GRAPHICS_DLL_DECLSPEC Shader : public RenderSystemObject
	{
	public:
		Shader() = default;
		virtual ~Shader() = default;
	};
}