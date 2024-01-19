#include <rengine\core\resource\Material.h>

namespace rengine
{
	Material::Material(uuid uuid)
	: Resource(uuid, TEXT("Material"))
	{

	}

	Material::~Material()
	{
	}

	bool Material::LoadMemory()
	{
		return false;
	}

	bool Material::UnLoadMemory()
	{
		return false;
	}

}