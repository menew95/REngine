#include <graphics_core\renderer\Renderer.h>
#include <graphics_core\RenderPass.h>
#include <graphics_core\ResourceManager.h>
#include <graphics_core\LightManager.h>

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

	struct alignas(16) PostProcess
	{
		// sslr
		Vector2 _depthBufferSize; // dimensions of the z-buffer
		float _zThickness = 0.2f; // thickness to ascribe to each pixel in the depth buffer
		float _nearPlaneZ; // the camera's near z plane

		float _stride = 1.0f; // Step in horizontal or vertical pixels between samples. This is a float
		// because integer math is slow on GPUs, but should be set to an integer >= 1.
		float _maxSteps = 1000.f; // Maximum number of iterations. Higher gives better images but may be slow.
		float _maxDistance = 300.f; // Maximum camera-space distance to trace before returning a miss.
		float _strideZCutoff; // More distant pixels are smaller in screen space. This value tells at what point to
		// start relaxing the stride to give higher quality reflections for objects far from
		// the camera.

		float _numMips = 7.f; // the number of mip levels in the convolved color buffer
		float _fadeStart = 0.09f; // determines where to start screen edge fading of effect
		float _fadeEnd = 1.f; // determines where to end screen edge fading of effect

		// tone map
		float g_fHardExposure = 1.0f;

		Vector4 _pad;  // 64bit
	};

	struct alignas(16) PerFrame
	{
		CameraInfo _camera;
		PostProcess _postProcess;

		uint _lightCnt = 0;

		CascadedInfo _shadow;
	};

	void Renderer::SetCamera(CameraBuffer* cameraBuffer)
	{
		PerFrame _perFrame;

		_perFrame._camera = cameraBuffer->GetCameraInfo();

		_perFrame._lightCnt = 1;

		m_pCommandBuffer->UpdateBuffer(*m_pFrameBuffer, 0, &_perFrame, sizeof(PerFrame));

		//cameraBuffer->UpdateBuffer(m_pCommandBuffer, m_pFrameBuffer);
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