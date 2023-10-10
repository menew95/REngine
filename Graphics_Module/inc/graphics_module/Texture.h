#pragma once

#include "graphics_module\Resource.h"
#include "graphics_module\TextureFlags.h"

namespace graphics
{
	class GRAPHICS_DLL_DECLSPEC Texture : public Resource
	{
	public:
		virtual ~Texture() = default;

		ResourceType GetResourceType() const override;

		TextureType GetType() { return m_TextureType; }
	protected:
		Texture(const TextureType& type, uint32 bindFlags);

		TextureType	m_TextureType;
		uint32		m_BindFlags = 0;
	};
}