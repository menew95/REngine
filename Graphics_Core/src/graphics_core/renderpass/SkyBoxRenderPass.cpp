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

		ResourceManager::GetInstance()->RelaseMaterialBuffer(TEXT("SkyBox"));
	}

	void SkyBoxRenderPass::Init()
	{
		m_pSkyBoxMesh = new MeshObject(TEXT("SkyBox Object"));

		m_pSkyBoxMesh->SetMeshBuffer(ResourceManager::GetInstance()->GetMeshBuffer(TEXT("00000000-0000-0000-0000-100000000000")));

		m_pTransBuffer = ResourceManager::GetInstance()->GetBuffer(TEXT("PerObject"));

		m_pSkyBoxMatBuffer = ResourceManager::GetInstance()->CreateMaterialBuffer(TEXT("Default_SkyBox"));

		m_pSkyBoxMatBuffer->SetPipelineID(TEXT("SkyBox"));

		m_pSkyBoxTexture = ResourceManager::GetInstance()->GetTextureBuffer(TEXT("fe6f153f-d693-4675-9b0e-65b8be91f35b"));

		m_pSkyBoxMatBuffer->SetTexture(TEXT("gSkyBox"), m_pSkyBoxTexture);
	}
	
	void SkyBoxRenderPass::Bind(CommandBuffer* command)
	{
		__super::Bind(command);
	}
	
	void SkyBoxRenderPass::BeginExcute(CommandBuffer* command, CameraBuffer* camBuffer)
	{
		__super::BeginExcute(command, camBuffer);

		auto& _camInfo = camBuffer->GetCameraInfo();

		math::Matrix _camWorld = math::Matrix::CreateTranslation(_camInfo._cameraWorldPos);

		m_pSkyBoxMesh->SetWorld(_camWorld);
		
		command->UpdateBuffer(*m_pTransBuffer, 0, &m_pSkyBoxMesh->GetTrans(), sizeof(PerObject));

		command->SetRenderTarget(*camBuffer->GetRenderTarget(), 0, nullptr);
	}
	
	void SkyBoxRenderPass::Excute(CommandBuffer* command)
	{
		__super::Excute(command);

		m_pSkyBoxMatBuffer->BindPipelineState(command);

		m_pSkyBoxMatBuffer->BindResource(command);

		Renderer::GetInstance()->RenderMesh(m_pSkyBoxMesh, 0);
	}

	void SkyBoxRenderPass::EndExcute(CommandBuffer* command)
	{
		__super::EndExcute(command);
	}

	void SkyBoxRenderPass::SetSkyBox(TextureBuffer* texture)
	{
		m_pSkyBoxTexture = texture;

		m_pSkyBoxMatBuffer->SetTexture(TEXT("g_SkyBox"), texture);
	}
}