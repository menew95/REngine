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
	}

	void SubMeshBuffer::CreateIndexBuffer(uuid uuid, const void* data, uint32 size, uint32 stride)
	{
		BufferDesc _desc;

		_desc._bindFlags = BindFlags::IndexBuffer;
		_desc._size = size;
		_desc._format = Format::R32_UINT;
		_desc._stride = stride;

		m_pIndexBuffer = ResourceManager::GetInstance()->CreateBuffer(uuid, _desc, data);

		assert(m_pIndexBuffer != nullptr);

		m_indexCount = size / stride;
	}

	MeshBuffer::MeshBuffer()
	{

	}

	MeshBuffer::~MeshBuffer()
	{
		/*ResourceManager::GetInstance()->ReleaseBuffer(m_pVertexBuffer);

		for (auto& _subMesh : m_subMeshBuffers)
		{
			ResourceManager::GetInstance()->ReleaseBuffer(_subMesh.GetBuffer());
		}*/
	}

	void MeshBuffer::CreateVertexBuffer(uuid uuid, const void* data, uint32 size, uint32 stride)
	{
		BufferDesc _desc;

		_desc._bindFlags = BindFlags::VertexBuffer;
		_desc._size = size;
		_desc._stride = stride;

		m_pVertexBuffer = ResourceManager::GetInstance()->CreateBuffer(uuid, _desc, data);

		assert(m_pVertexBuffer != nullptr);
	}

	void MeshBuffer::CreateSubMeshBuffer(uuid uuid, const void* data, uint32 size, uint32 stride)
	{
		SubMeshBuffer _subMeshBuf;

		_subMeshBuf.CreateIndexBuffer(uuid, data, size, stride);

		m_subMeshBuffers.emplace_back(_subMeshBuf);
	}

	void MeshBuffer::SetName(const char* name)
	{
		assert(m_pVertexBuffer != nullptr);

		string _name(name);

		m_pVertexBuffer->SetName((_name + "_vertex").c_str());

		for (size_t i = 0; i < m_subMeshBuffers.size(); i++)
		{
			m_subMeshBuffers[i].GetBuffer()->SetName((_name + "_index" + std::to_string(i)).c_str());
		}
	}
	bool MeshBuffer::UnloadBuffer()
	{
		bool _ret = ResourceManager::GetInstance()->ReleaseBuffer(m_pVertexBuffer);

		if(_ret) m_pVertexBuffer = nullptr;

		for (auto& _subMesh : m_subMeshBuffers)
		{
			_ret = ResourceManager::GetInstance()->ReleaseBuffer(_subMesh.GetBuffer());
		}

		m_subMeshBuffers.clear();

		return _ret;
	}
}