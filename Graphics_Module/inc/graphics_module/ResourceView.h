#pragma once

#include "graphics_module\ResourceViewFlags.h"
#include "graphics_module\Resource.h"

namespace graphics
{
	class GRAPHICS_DLL_DECLSPEC ResourceView : public Resource 
	{
	public:
		virtual ~ResourceView() = default;

		ResourceType GetResourceType() const override;
	};
}