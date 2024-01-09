#include <rengine\core\resource\material.h>

namespace rengine
{
	Material::Material(uuid uuid, tstring name)
	: Resource(uuid, name, TEXT("Material"))
	{

	}

	Material::~Material()
	{
	}

}