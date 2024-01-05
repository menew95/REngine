#include <graphics_core\resource\TextureBuffer.h>

#include <graphics_module\texture.h>

namespace graphics
{
	TextureBuffer::TextureBuffer()
	: ResourceBuffer()
	{

	}

	TextureBuffer::~TextureBuffer()
	{

	}
	
	void* TextureBuffer::GetTextureID()
	{
		return m_pTexture->GetTextureID();
	}
}