#include <graphics_core\renderer\Renderer.h>
#include <graphics_core\RenderPass.h>
#include <graphics_core\ResourceManager.h>
#include <graphics_core\LightManager.h>

#include <graphics_module\Buffer.h>
#include <graphics_module\CommandBuffer.h>

#include <graphics_core\object\MeshObject.h>
#include <graphics_core\object\PerFrame.h>

#include <graphics_core\resource\CameraBuffer.h>
#include <graphics_core\resource\MeshBuffer.h>
#include <graphics_core\resource\MaterialBuffer.h>

#include <graphics_core\RenderQueue.h>
#include <graphics_core\utility\Culling.h>

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

		m_pQuadMesh = ResourceManager::GetInstance()->GetMeshBuffer(TEXT("00000000-0000-0000-0000-000000000002"));

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
		PerFrame _perFrame;

		_perFrame._camera = cameraBuffer->GetCameraInfo();

		LightManager::GetInstance()->GetPerFrame(_perFrame);

		m_pCommandBuffer->UpdateBuffer(*m_pFrameBuffer, 0, &_perFrame, sizeof(PerFrame));

		Frustum _frustum;

		CullingHelper::CreateFrustumFromCamera(cameraBuffer->GetCameraInfo(), _frustum);

		RenderQueue::GetInstance()->FrustumCulling(_frustum);
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

	void Renderer::DrawRectangle()
	{
		m_pCommandBuffer->SetVertexBuffer(*m_pQuadMesh->GetBuffer());

		auto& _subMeshBuf = m_pQuadMesh->GetSubMesh(0);

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
	
	}
}