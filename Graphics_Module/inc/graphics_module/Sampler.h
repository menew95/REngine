#pragma once

#include "graphics_module\Resource.h"
#include "graphics_module\SamplerFlags.h"

namespace graphics
{
	class GRAPHICS_DLL_DECLSPEC Sampler : public Resource
	{
	public:
		Sampler() = default;
		virtual ~Sampler() = default;

		virtual void SetName(const char* name) abstract;

		ResourceType GetResourceType() const override final;
	};
}