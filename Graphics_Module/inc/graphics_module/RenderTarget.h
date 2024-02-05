#pragma once

#include "graphics_module\RenderSystemObject.h"

#include "graphics_module\RenderTargetFlags.h"

namespace graphics
{

	class GRAPHICS_DLL_DECLSPEC RenderTarget : public RenderSystemObject
	{
	public:
		RenderTarget() = default;
		virtual ~RenderTarget() = default;

		enum class Type
		{
			SwapChain,
			RT
		};

		inline Type GetType() { return m_Type; }

	protected:
		Type m_Type = Type::RT;
	};
}