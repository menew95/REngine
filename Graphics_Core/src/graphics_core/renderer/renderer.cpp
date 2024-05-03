#include <graphics_core\renderer\Renderer.h>
#include <graphics_core\RenderPass.h>
#include <graphics_core\ResourceManager.h>
#include <graphics_core\LightManager.h>
#include <graphics_core\GraphicsEngine.h>

#include <graphics_module\Buffer.h>
#include <graphics_module\RenderTarget.h>
#include <graphics_module\CommandBuffer.h>

#include <graphics_core\object\MeshObject.h>
#include <graphics_core\object\PerFrame.h>

#include <graphics_core\resource\CameraBuffer.h>
#include <graphics_core\resource\MeshBuffer.h>
#include <graphics_core\resource\LightBuffer.h>
#include <graphics_core\resource\MaterialBuffer.h>

#include <graphics_core\RenderQueue.h>
#include <graphics_core\utility\Culling.h>
#include <graphics_core\utility\ShadowHelper.h>

namespace graphics
{
	Renderer* Renderer::s_pRenderer = nullptr;

	Renderer::Renderer(CommandBuffer* command)
	: m_commandBuffer(command)
	{
		assert(s_pRenderer == nullptr);

		s_pRenderer = this;

		m_perFrameBuffer = ResourceManager::GetInstance()->GetBuffer(TEXT("PerFrame"));
		m_perCameraBuffer = ResourceManager::GetInstance()->GetBuffer(TEXT("PerCamera"));
		m_perObjectBuffer = ResourceManager::GetInstance()->GetBuffer(TEXT("PerObject"));
		m_perSkinedBuffer = ResourceManager::GetInstance()->GetBuffer(TEXT("PerSkin"));

		m_quadMeshBuffer = ResourceManager::GetInstance()->GetMeshBuffer(TEXT("00000000-0000-0000-0000-000000000002"));
	}

	Renderer::~Renderer()
	{
		s_pRenderer = nullptr;
	}

	void Renderer::SetFrameResource()
	{
		m_commandBuffer->SetResource(*m_perFrameBuffer, 0, BindFlags::ConstantBuffer, StageFlags::AllStages);
		m_commandBuffer->SetResource(*m_perCameraBuffer, 1, BindFlags::ConstantBuffer, StageFlags::AllStages);
		m_commandBuffer->SetResource(*m_perObjectBuffer, 2, BindFlags::ConstantBuffer, StageFlags::AllStages);

		PerFrame _perFrame;

		LightManager::GetInstance()->GetPerFrame(_perFrame);

		m_commandBuffer->UpdateBuffer(*m_perFrameBuffer, 0, &_perFrame, sizeof(PerFrame));
	}

	void Renderer::SetCamera(const SceneInfo& sceneInfo, CameraBuffer* cameraBuffer)
	{
		PerCamera _perCamera;

		_perCamera._time.x = sceneInfo._timeStep / 20.f;
		_perCamera._time.y = sceneInfo._timeStep;
		_perCamera._time.z = sceneInfo._timeStep * 2.f;
		_perCamera._time.w = sceneInfo._timeStep * 3.f;

		_perCamera._sinTime.x = sinf(sceneInfo._timeStep * 0.125f);
		_perCamera._sinTime.y = sinf(sceneInfo._timeStep * 0.25f);
		_perCamera._sinTime.z = sinf(sceneInfo._timeStep * 0.5f);
		_perCamera._sinTime.w = sinf(sceneInfo._timeStep);

		_perCamera._cosTime.x = cosf(sceneInfo._timeStep * 0.125f);
		_perCamera._cosTime.y = cosf(sceneInfo._timeStep * 0.25f);
		_perCamera._cosTime.z = cosf(sceneInfo._timeStep * 0.5f);
		_perCamera._cosTime.w = cosf(sceneInfo._timeStep);

		_perCamera._deltaTime.x = sceneInfo._deltaTime;
		_perCamera._deltaTime.y = 1.f / sceneInfo._deltaTime;
		_perCamera._deltaTime.z = sceneInfo._deltaTime * 2.f;
		_perCamera._deltaTime.w = sceneInfo._deltaTime * 3.f;

		_perCamera._screen = {
			cameraBuffer->GetRenderTarget()->GetResolution().x,
			cameraBuffer->GetRenderTarget()->GetResolution().y,
			1.f / cameraBuffer->GetRenderTarget()->GetResolution().x,
			1.f / cameraBuffer->GetRenderTarget()->GetResolution().y
		};

		_perCamera._ZBufferParams.x = (1 - cameraBuffer->m_cameraInfo._far) / cameraBuffer->m_cameraInfo._near;
		_perCamera._ZBufferParams.y = cameraBuffer->m_cameraInfo._far / cameraBuffer->m_cameraInfo._near;
		_perCamera._ZBufferParams.z = _perCamera._ZBufferParams.x / cameraBuffer->m_cameraInfo._far;
		_perCamera._ZBufferParams.w = _perCamera._ZBufferParams.y / cameraBuffer->m_cameraInfo._far;
		
		_perCamera._camera = cameraBuffer->GetCameraInfo();

		if (LightManager::GetInstance()->m_cascadedSet)
		{
			/*float cascadeOffset[4]{ 0.05f, 0.18f, 0.6f, 1.0f };

			CascadeShadowSlice slice[4];
			ShadowHelper::CalculateCascadeShadowSlices2(
				_perFrame._camera,
				cascadeOffset,
				slice,
				1024,
				LightManager::GetInstance()->m_cascadedLightBuffer->GetDirection()
			);

			for (int sliceIdx = 0; sliceIdx < 4; ++sliceIdx)
			{
				ShadowHelper::CalcuateViewProjectionMatrixFromCascadeSlice2(
					slice[sliceIdx],
					LightManager::GetInstance()->m_cascadedLightBuffer->GetDirection(),
					_perFrame._shadow._lightTransform[sliceIdx]);
			}

			_perFrame._shadow._cascadeOffset[0].x = cascadeOffset[0];
			_perFrame._shadow._cascadeOffset[1].x = cascadeOffset[1];
			_perFrame._shadow._cascadeOffset[2].x = cascadeOffset[2];
			_perFrame._shadow._cascadeOffset[3].x = cascadeOffset[3];

			_perFrame._shadow._cascadeScale.x = slice[0]._frustumRadius * 2;
			_perFrame._shadow._cascadeScale.y = slice[1]._frustumRadius * 2;
			_perFrame._shadow._cascadeScale.z = slice[2]._frustumRadius * 2;
			_perFrame._shadow._cascadeScale.w = slice[3]._frustumRadius * 2;*/

			cameraBuffer->UpdateCascadeShadow(LightManager::GetInstance()->m_cascadedDir);
			_perCamera._shadow = cameraBuffer->GetCascadedInfo();
		}

		m_commandBuffer->UpdateBuffer(*m_perCameraBuffer, 0, &_perCamera, sizeof(PerCamera));

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

		m_commandBuffer->SetVertexBuffer(*_meshBuf->GetBuffer());

		auto& _subMeshBuf = _meshBuf->GetSubMesh(subMeshIdx);

		m_commandBuffer->SetIndexBuffer(*_subMeshBuf.GetBuffer());

		m_commandBuffer->DrawIndexed(_subMeshBuf.GetIndexCount(), 0, 0);
	}

	void Renderer::DrawRectangle()
	{
		m_commandBuffer->SetVertexBuffer(*m_quadMeshBuffer->GetBuffer());

		auto& _subMeshBuf = m_quadMeshBuffer->GetSubMesh(0);

		m_commandBuffer->SetIndexBuffer(*_subMeshBuf.GetBuffer());

		m_commandBuffer->DrawIndexed(_subMeshBuf.GetIndexCount(), 0, 0);
	}

	void Renderer::RenderShadow(MeshObject* meshObject)
	{
		MeshBuffer* _meshBuf = meshObject->GetMeshBuffer();

		m_commandBuffer->SetVertexBuffer(*_meshBuf->GetBuffer());

		for (size_t i = 0; i < _meshBuf->GetSubMeshCount(); i++)
		{
			auto& _subMeshBuf = _meshBuf->GetSubMesh(i);

			if (i < meshObject->GetMaterialBuffers().size() || meshObject->GetMaterialBuffers()[i] != nullptr)
				continue;

			meshObject->GetMaterialBuffers()[i]->BindResource(m_commandBuffer);

			m_commandBuffer->SetIndexBuffer(*_subMeshBuf.GetBuffer());

			m_commandBuffer->DrawIndexed(_subMeshBuf.GetIndexCount(), 0, 0);
		}
	}

	void Renderer::RenderParticle(ParticleObject* particleObject)
	{

	}

	void Renderer::RenderSkyBox(RenderPass* renderPass)
	{
	
	}
}