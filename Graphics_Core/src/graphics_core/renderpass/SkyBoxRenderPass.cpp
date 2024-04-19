#include <graphics_core\renderpass\SkyBoxRenderPass.h>

#include <graphics_core\object\MeshObject.h>

#include <graphics_core\ResourceManager.h>
#include <graphics_core\resource\CameraBuffer.h>
#include <graphics_core\resource\MaterialBuffer.h>

#include <graphics_core\renderer\Renderer.h>

#include <graphics_module\CommandBuffer.h>

#include <graphics_module\PipelineState.h>
#include <graphics_module\PipelineLayout.h>

#include <graphics_module\Shader.h>
#include <graphics_module\Sampler.h>
#include <graphics_module\Texture.h>
#include <graphics_module\RenderTarget.h>

#include <common\AssetPath.h>

namespace graphics
{
	SkyBoxRenderPass::SkyBoxRenderPass()
		: RenderPass(TEXT("SkyBox Pass"))
	{
		
	}

	SkyBoxRenderPass::~SkyBoxRenderPass()
	{
		delete m_pSkyBoxMesh;

		delete m_pSkySphereMesh;

		ResourceManager::GetInstance()->RelaseMaterialBuffer(TEXT("SkyBox-Cubed"));

		ResourceManager::GetInstance()->RelaseMaterialBuffer(TEXT("SkyBox-Panoramic"));

		ResourceManager::GetInstance()->RelaseMaterialBuffer(TEXT("SkyBox-Procedural"));
	}

	void SkyBoxRenderPass::Init()
	{
		m_pSkyBoxMesh = new MeshObject(TEXT("SkyBox Object"));

		m_pSkyBoxMesh->SetMeshBuffer(ResourceManager::GetInstance()->GetMeshBuffer(TEXT("00000000-0000-0000-0000-000000000001")));

		m_pTransBuffer = ResourceManager::GetInstance()->GetBuffer(TEXT("PerObject"));

#pragma region Cubed

		m_pSkyBox_CubedMatBuffer = ResourceManager::GetInstance()->CreateMaterialBuffer(TEXT("SkyBox-Cubed"));

		m_pSkyBox_CubedMatBuffer->SetPipelineID(TEXT("SkyBox-Cubed"));

		m_pSkyBoxTexture = ResourceManager::GetInstance()->GetTextureBuffer(TEXT("fe6f153f-d693-4675-9b0e-65b8be91f35b"));
		
		m_pSkyBox_CubedMatBuffer->SetTexture(TEXT("gTexture"), m_pSkyBoxTexture);

#pragma endregion


#pragma region Panoramic

		m_pSkySphereMesh = new MeshObject(TEXT("SkyBox Object"));

		m_pSkySphereMesh->SetMeshBuffer(ResourceManager::GetInstance()->GetMeshBuffer(TEXT("00000000-0000-0000-0000-000000000003")));

		m_pSkyBox_PanoramicMatBuffer = ResourceManager::GetInstance()->CreateMaterialBuffer(TEXT("Skybox-Panoramic"));

		m_pSkyBox_PanoramicMatBuffer->SetPipelineID(TEXT("Skybox-Panoramic"));
		
		m_pSkyBoxTexture = ResourceManager::GetInstance()->GetTextureBuffer(TEXT("491bdb39-3a7e-4554-b37a-c81add3326b4"));

		m_pSkyBox_PanoramicMatBuffer->SetTexture(TEXT("gTexture"), m_pSkyBoxTexture);

#pragma endregion


#pragma region Procedural

		m_pSkyBox_ProceduralMatBuffer = ResourceManager::GetInstance()->CreateMaterialBuffer(TEXT("SkyBox-Procedural"));

		m_pSkyBox_ProceduralMatBuffer->SetPipelineID(TEXT("Skybox-Procedural"));

		m_pRenderTarget = ResourceManager::GetInstance()->GetRenderTarget(TEXT("MainFrame"));

#pragma endregion
	}

	void SkyBoxRenderPass::BeginExcute(CommandBuffer* command, CameraBuffer* camBuffer)
	{
		__super::BeginExcute(command, camBuffer);

		auto& _camInfo = camBuffer->GetCameraInfo();

		math::Matrix _camWorld = math::Matrix::CreateTranslation(_camInfo._cameraWorldPos);

		m_pSkyBoxMesh->SetWorld(_camWorld);
		
		command->UpdateBuffer(*m_pTransBuffer, 0, &m_pSkyBoxMesh->GetTrans(), sizeof(PerObject));

		AttachmentClear _attachmentClear[2] =
		{
			{ math::Vector4::Zero, 0 },
			{ 1.0f, 0u }
		};

		command->SetRenderTarget(*m_pRenderTarget, 2, _attachmentClear);

		command->SetViewport(m_pRenderTarget->GetResolution());

		/*command->SetRenderTarget(*camBuffer->GetRenderTarget(), 2, _attachmentClear);

		command->SetViewport(camBuffer->GetRenderTarget()->GetResolution());*/
	}
	
	void SkyBoxRenderPass::Excute(CommandBuffer* command)
	{
		__super::Excute(command);

		m_pSkyBox_PanoramicMatBuffer->BindPipelineState(command);

		m_pSkyBox_PanoramicMatBuffer->BindResource(command);

		Renderer::GetInstance()->RenderMesh(m_pSkySphereMesh, 0);
	}

	void SkyBoxRenderPass::EndExcute(CommandBuffer* command)
	{
		__super::EndExcute(command);
	}

	void SkyBoxRenderPass::SetSkyBox(TextureBuffer* texture)
	{
		m_pSkyBoxTexture = texture;

		m_pSkyBox_CubedMatBuffer->SetTexture(TEXT("gTexture"), texture);
	}
}