#include <rengine\core\resource\Mesh.h>

namespace rengine
{
	Mesh::Mesh(uuid uuid)
	: Resource(uuid, TEXT("Mesh"))
	{

	}

	Mesh::~Mesh()
	{
		if (m_bIsLoad)
		{

		}
	}
	bool Mesh::LoadMemory()
	{
		return false;
	}
	bool Mesh::UnLoadMemory()
	{
		return false;
	}
}