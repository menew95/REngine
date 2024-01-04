#include <rengine\core\resource\texture.h>

namespace rengine
{
	Texture::Texture(uuid uuid, tstring name)
	: Resource(uuid, name)
	{

	}

	Texture::Texture(uuid uuid, tstring name, tstring type)
	: Resource(uuid, name, type)
	{
		
	}

	Texture::~Texture()
	{
		if (m_bIsLoad)
		{

		}
	}
}