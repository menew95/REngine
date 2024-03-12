#include <graphics_core\renderpass\SkyBoxRenderPass.h>

#include <graphics_core\object\MeshObject.h>

#include <graphics_core\ResourceManager.h>
#include <graphics_core\resource\CameraBuffer.h>

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
	}

	void SkyBoxRenderPass::Init()
	{
		m_pPipelineState = ResourceManager::GetInstance()->GetPipelineState(TEXT("SkyBox"));

		m_pPipelineLayout = ResourceManager::GetInstance()->GetPipelineLayout(TEXT("SkyBox"));

		m_pSkyBoxMesh = new MeshObject(TEXT("SkyBox Object"));

		m_pSkyBoxMesh->SetMeshBuffer(ResourceManager::GetInstance()->GetMeshBuffer(TEXT("00000000-0000-0000-0000-100000000000")));

		m_renderObjects.push_back(m_pSkyBoxMesh);

		//m_pRenderTarget = ResourceManager::GetInstance()->GetRenderTarget(TEXT("MainFrame"));

		m_pTransBuffer = ResourceManager::GetInstance()->GetBuffer(TEXT("PerObject"));
	}
	
	void SkyBoxRenderPass::Bind(CommandBuffer* command)
	{
		//command->SetRenderTarget(*m_pRenderTarget, 0, nullptr);

		__super::Bind(command);
	}
	
	void SkyBoxRenderPass::BeginExcute(CommandBuffer* command, CameraBuffer* camBuffer)
	{
		__super::BeginExcute(command, camBuffer);

		auto& _camInfo = camBuffer->GetCameraInfo();

		math::Matrix _camWorld = math::Matrix::CreateTranslation(_camInfo._cameraWorldPos);

		m_pSkyBoxMesh->SetWorld(_camWorld);
		
		command->UpdateBuffer(*m_pTransBuffer, 0, &m_pSkyBoxMesh->GetTrans(), sizeof(math::Matrix) * 2);

		command->SetRenderTarget(*camBuffer->GetRenderTarget(), 0, nullptr);
	}
	
	void SkyBoxRenderPass::Excute(CommandBuffer* command)
	{
		__super::Excute(command);

		Renderer::GetInstance()->RenderMesh(m_pSkyBoxMesh);
	}

	void SkyBoxRenderPass::EndExcute(CommandBuffer* command)
	{
		__super::EndExcute(command);
	}
}