#include <graphics_core\resource\MeshBuffer.h>

#include <graphics_module\Buffer.h>

#include <graphics_core\ResourceManager.h>

namespace graphics
{
	SubMeshBuffer::SubMeshBuffer()
	{

	}

	SubMeshBuffer::~SubMeshBuffer()
	{
		ResourceManager::GetInstance()->ReleaseBuffer(m_pIndexBuffer);
	}

	MeshBuffer::MeshBuffer()
	{

	}

	MeshBuffer::~MeshBuffer()
	{
		ResourceManager::GetInstance()->ReleaseBuffer(m_pVertexBuffer);
	}

	void MeshBuffer::CreateVertexBuffer(uuid uuid, const void* data, uint32 size, uint32 stride)
	{
		BufferDesc _desc;


	}
}