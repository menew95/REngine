#include <rengine\core\resource\Mesh.h>

namespace rengine
{
	Mesh::Mesh(uuid uuid, tstring name)
	: Resource(uuid, name, TEXT("Mesh"))
	{

	}

	Mesh::~Mesh()
	{
		if (m_bIsLoad)
		{

		}
	}
}