#include <rengine\core\resource\Material.h>

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