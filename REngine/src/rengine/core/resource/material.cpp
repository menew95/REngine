#include <rengine\core\resource\material.h>

namespace rengine
{
	Material::Material(uuid uuid, tstring name)
	: Resource(uuid, name)
	{

	}
	Material::Material(uuid uuid, tstring name, tstring type)
	: Resource(uuid, name, type)
	{

	}

	Material::~Material()
	{
	}

}