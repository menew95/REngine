#include <rengine\core\resource\Mesh.h>

#include <rengine\system\GraphicsSystem.h>

#include <graphics_core\ResourceManager.h>

#include <graphics_core\resource\MeshBuffer.h>

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
			UnLoadMemory();
		}
	}
	bool Mesh::LoadMemory()
	{
		m_pMeshBuffer = graphics::ResourceManager::GetInstance()->CreateMeshBuffer(GetUUID());

		m_bIsLoad = true;

		m_pMeshBuffer->CreateVertexBuffer(GetUUID(), m_vertices.data(), static_cast<uint32>(sizeof(VertexAttribute) * m_vertices.size()), sizeof(VertexAttribute));

		for (auto& _indies : m_indices)
		{
			m_pMeshBuffer->CreateSubMeshBuffer(GetUUID(), _indies.data(), static_cast<uint32>(sizeof(uint32) * _indies.size()), sizeof(uint32));
		}

		m_pMeshBuffer->SetName(GetNameStr().c_str());

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