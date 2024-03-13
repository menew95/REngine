#include <rengine\core\resource\Mesh.h>

#include <rengine\system\GraphicsSystem.h>

#include <graphics_core\ResourceManager.h>

#include <graphics_core\resource\MeshBuffer.h>

#include <rttr\registration.h>

std::weak_ptr<rengine::Mesh> converter_func_weak_container(const weak_ptr<rengine::Object>& value, bool& ok)
{
	ok = true;

	std::weak_ptr<rengine::Mesh> _ret = std::static_pointer_cast<rengine::Mesh>(value.lock());

	return _ret;
}

RTTR_REGISTRATION
{
	rttr::type::register_converter_func(converter_func_weak_container);
}


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