#include <graphics_core\renderer\Renderer.h>
#include <graphics_core\RenderPass.h>
#include <graphics_core\ResourceManager.h>

#include <graphics_module\Buffer.h>
#include <graphics_module\CommandBuffer.h>

#include <graphics_core\object\MeshObject.h>

#include <graphics_core\resource\CameraBuffer.h>
#include <graphics_core\resource\MeshBuffer.h>
#include <graphics_core\resource\MaterialBuffer.h>

namespace graphics
{
	Renderer* Renderer::s_pRenderer = nullptr;

	Renderer::Renderer(CommandBuffer* command)
	: m_pCommandBuffer(command)
	{
		assert(s_pRenderer == nullptr);

		s_pRenderer = this;

		m_pFrameBuffer = ResourceManager::GetInstance()->GetBuffer(TEXT("PerFrame"));
		m_pTransBuffer = ResourceManager::GetInstance()->GetBuffer(TEXT("PerObject"));
	}

	Renderer::~Renderer()
	{
		s_pRenderer = nullptr;
	}

	void Renderer::SetFrameResource()
	{
		m_pCommandBuffer->SetResource(*m_pFrameBuffer, 0, BindFlags::ConstantBuffer, StageFlags::AllStages);
		m_pCommandBuffer->SetResource(*m_pTransBuffer, 1, BindFlags::ConstantBuffer, StageFlags::AllStages);
	}

	void Renderer::SetCamera(CameraBuffer* cameraBuffer)
	{
		cameraBuffer->UpdateBuffer(m_pCommandBuffer, m_pFrameBuffer);
	}

	void Renderer::RenderMesh(vector<RenderObject*>& renderObjects, vector<RenderPass*>& renderPassList)
	{
		

	}

	void Renderer::RenderMesh(MeshObject* meshObject, uint32 subMeshIdx)
	{
		MeshBuffer* _meshBuf = meshObject->GetMeshBuffer();

		if(_meshBuf == nullptr)
			return;

		m_pCommandBuffer->SetVertexBuffer(*_meshBuf->GetBuffer());

		auto& _subMeshBuf = _meshBuf->GetSubMesh(subMeshIdx);

		m_pCommandBuffer->SetIndexBuffer(*_subMeshBuf.GetBuffer());

		m_pCommandBuffer->DrawIndexed(_subMeshBuf.GetIndexCount(), 0, 0);
	}

	void Renderer::RenderShadow(MeshObject* meshObject)
	{
		MeshBuffer* _meshBuf = meshObject->GetMeshBuffer();

		m_pCommandBuffer->SetVertexBuffer(*_meshBuf->GetBuffer());

		for (size_t i = 0; i < _meshBuf->GetSubMeshCount(); i++)
		{
			auto& _subMeshBuf = _meshBuf->GetSubMesh(i);

			if (i < meshObject->GetMaterialBuffers().size() || meshObject->GetMaterialBuffers()[i] != nullptr)
				continue;

			meshObject->GetMaterialBuffers()[i]->BindResource(m_pCommandBuffer);

			m_pCommandBuffer->SetIndexBuffer(*_subMeshBuf.GetBuffer());

			m_pCommandBuffer->DrawIndexed(_subMeshBuf.GetIndexCount(), 0, 0);
		}
	}

	void Renderer::RenderParticle(ParticleObject* particleObject)
	{

	}

	void Renderer::RenderSkyBox(RenderPass* renderPass)
	{
		renderPass->Bind(m_pCommandBuffer);
	}
}