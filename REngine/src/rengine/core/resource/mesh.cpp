#include <rengine\core\resource\mesh.h>

namespace rengine
{
	Mesh::Mesh(uuid uuid, tstring name)
	: Resource(uuid, name)
	{

	}

	Mesh::Mesh(uuid uuid, tstring name, tstring type)
	: Resource(uuid, name, type)
	{

	}

	Mesh::~Mesh()
	{
		if (m_bIsLoad)
		{

		}
	}
}