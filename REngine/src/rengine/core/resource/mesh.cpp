#include <rengine\core\resource\Mesh.h>

#include <rengine\system\GraphicsSystem.h>

#include <graphics_core\ResourceManager.h>

#include <graphics_core\resource\MeshBuffer.h>

#include <rttr\registration.h>

std::weak_ptr<rengine::Mesh> converter_func_weak_container(const weak_ptr<rengine::Object>& value, bool& ok)
{
	std::weak_ptr<rengine::Mesh> _ret = std::static_pointer_cast<rengine::Mesh>(value.lock());

	ok = _ret.lock() != nullptr;

	return _ret;
}

std::shared_ptr<rengine::Mesh> converter_func_shared_container(const shared_ptr<rengine::Object>& value, bool& ok)
{
	std::shared_ptr<rengine::Mesh> _ret = std::static_pointer_cast<rengine::Mesh>(value);

	ok = _ret != nullptr;

	return _ret;
}

RTTR_REGISTRATION
{
	rttr::registration::class_<rengine::Mesh>("Mesh")
	.constructor<tstring>()
	;
	rttr::type::register_converter_func(converter_func_weak_container);
	rttr::type::register_converter_func(converter_func_shared_container);
}


namespace rengine
{
	Mesh::Mesh(uuid uuid)
	: Resource(uuid, TEXT("Mesh"))
	{
		m_pMeshBuffer = graphics::ResourceManager::GetInstance()->CreateMeshBuffer(GetUUID());
	}

	Mesh::~Mesh()
	{
		if (m_bIsLoad)
		{
			UnLoadMemory();
		}

		graphics::ResourceManager::GetInstance()->RelaseMeshBuffer(GetUUID());
	}

	bool Mesh::LoadMemory()
	{
		m_bIsLoad = true;

		m_pMeshBuffer->CreateVertexBuffer(TEXT("Vertex_") + GetUUID(), m_vertices.data(), static_cast<uint32>(sizeof(VertexAttribute) * m_vertices.size()), sizeof(VertexAttribute));

		for (uint32 i = 0; i < m_indices.size(); i++)
		{
			uuid _indexUUID = TEXT("Index") + to_wstring(i) + TEXT("_") + GetUUID();

			m_pMeshBuffer->CreateSubMeshBuffer(_indexUUID, m_indices[i].data(), static_cast<uint32>(sizeof(uint32) * m_indices[i].size()), sizeof(uint32));
		}

		m_pMeshBuffer->SetName(GetNameStr().c_str());

		return true;
	}

	bool Mesh::UnLoadMemory()
	{
		m_bIsLoad = false;

		return m_pMeshBuffer->UnloadBuffer();;
	}
}