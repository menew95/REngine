#include "graphics_module\Texture.h"

namespace graphics
{
	ResourceType Texture::GetResourceType() const
	{
		return ResourceType::Texture;
	}

	Texture::Texture(const TextureType& type, uint32 bindFlags)
		: m_TextureType(type)
		, m_BindFlags(bindFlags)
	{

	}
}