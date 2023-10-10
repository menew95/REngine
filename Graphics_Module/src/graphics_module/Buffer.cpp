#include "graphics_module\Buffer.h"

namespace graphics
{
	Buffer::Buffer(uint32 bindFlags)
		: m_BindFlags(bindFlags)
	{

	}

	ResourceType Buffer::GetResourceType() const
	{
		return ResourceType::Buffer;
	}

}