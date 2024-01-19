#include <rengine\core\resource\Mesh.h>

#include <rengine\system\GraphicsSystem.h>

#include <graphics_core\ResourceManager.h>

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
		m_pMeshBuffer = graphics::ResourceManager::GetInstance()->CreateMeshBuffer(GetUUID());

		m_bIsLoad = true;

		return true;
	}
	bool Mesh::UnLoadMemory()
	{
		bool _ret = graphics::ResourceManager::GetInstance()->CreateMeshBuffer(GetUUID());

		if (_ret)
		{
			m_pMeshBuffer = nullptr;

			m_bIsLoad = false;
		}

		return _ret;
	}
}