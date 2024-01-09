#include <rengine\core\resource\Texture.h>

namespace rengine
{
	Texture::Texture(uuid uuid, tstring name)
	: Resource(uuid, name, TEXT("Texture"))
	{

	}

	Texture::~Texture()
	{
		if (m_bIsLoad)
		{

		}
	}
}