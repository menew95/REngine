#pragma once

#include "graphics_module\ResourceFlags.h"
#include "graphics_module\TextureFlags.h"

namespace graphics
{
	class Resource;

	struct ResourceViewDesc
	{
		long _bindFlags;
		std::vector<Resource*> _resources;
		std::vector<TextureSubresource> _texSubresources;
	};
}