#include <rengine\core\resource\Texture.h>

namespace rengine
{
	Texture::Texture(uuid uuid)
	: Resource(uuid, TEXT("Texture"))
	{

	}

	Texture::~Texture()
	{
		if (m_bIsLoad)
		{

		}
	}
}