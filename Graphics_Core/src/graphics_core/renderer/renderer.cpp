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
	Renderer::Renderer(CommandBuffer* command)
	: m_pCommandBuffer(command)
	{
		m_pFrameBuffer = ResourceManager::GetInstance()->GetBuffer(TEXT("Frame Buffer"));
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::SetCamera(CameraBuffer* cameraBuffer)
	{
		cameraBuffer->UpdateBuffer(m_pCommandBuffer, m_pFrameBuffer);
	}

	void Renderer::RenderMesh(vector<RenderObject*>& renderObjects, vector<RenderPass*>& renderPassList)
	{
		

	}

	void Renderer::RenderMesh(MeshObject* meshObject)
	{
		MeshBuffer* _meshBuf = meshObject->GetMeshBuffer();

		m_pCommandBuffer->SetVertexBuffer(*_meshBuf->GetBuffer());

		for (size_t i = 0; i < _meshBuf->GetSubMeshCount(); i++)
		{
			auto& _subMeshBuf = _meshBuf->GetSubMesh(i);

			if (i < meshObject->GetMaterialBuffers().size() || meshObject->GetMaterialBuffers()[i] != nullptr)
				continue;
			
			m_pCommandBuffer->SetIndexBuffer(*_subMeshBuf.GetBuffer());

			m_pCommandBuffer->DrawIndexed(_subMeshBuf.GetIndexCount(), 0, 0);
		}

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