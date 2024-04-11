#include <graphics_core\renderpass\FinalRenderPass.h>
#include <graphics_core\object\MeshObject.h>

#include <graphics_core\ResourceManager.h>
#include <graphics_core\resource\CameraBuffer.h>
#include <graphics_core\resource\MaterialBuffer.h>
#include <graphics_core\resource\MeshBuffer.h>

#include <graphics_core\renderer\Renderer.h>

#include <graphics_module\CommandBuffer.h>

#include <graphics_module\PipelineState.h>
#include <graphics_module\PipelineLayout.h>

#include <graphics_module\Shader.h>
#include <graphics_module\Buffer.h>
#include <graphics_module\Sampler.h>
#include <graphics_module\Texture.h>
#include <graphics_module\RenderTarget.h>

#include <common\AssetPath.h>

namespace graphics
{
	FinalRenderPass::FinalRenderPass()
		: RenderPass(TEXT("Final Pass"))
	{
	
	}
	
	FinalRenderPass::~FinalRenderPass()
	{
	
	}
	
	void FinalRenderPass::Init()
	{
		m_pMatBuffer = ResourceManager::GetInstance()->CreateMaterialBuffer(TEXT("Final"));

		m_pMatBuffer->SetPipelineID(TEXT("Final"));

		m_pMatBuffer->SetTexture(TEXT("gDebugTexture"), ResourceManager::GetInstance()->GetTexture(TEXT("MainFrame")));

		m_pScreenMesh = new MeshObject(TEXT("Screen Object"));

		m_pScreenMesh->SetMeshBuffer(ResourceManager::GetInstance()->GetMeshBuffer(TEXT("00000000-0000-0000-0000-000000000002")));

		m_pRenderTarget = ResourceManager::GetInstance()->GetRenderTarget(TEXT("MainFrame"));
	}
	
	void FinalRenderPass::Bind(CommandBuffer* command)
	{
	
	}
	
	void FinalRenderPass::BeginExcute(CommandBuffer* command, CameraBuffer* camBuffer)
	{
		__super::BeginExcute(command, camBuffer);

		command->SetRenderTarget(*camBuffer->GetRenderTarget(), 0, nullptr);

		command->SetViewport(camBuffer->GetRenderTarget()->GetResolution());
	}
	
	void FinalRenderPass::Excute(CommandBuffer* command)
	{
		__super::Excute(command);

		m_pMatBuffer->BindPipelineState(command);

		m_pMatBuffer->BindResource(command);

		Renderer::GetInstance()->RenderMesh(m_pScreenMesh, 0);
	}
	
	void FinalRenderPass::EndExcute(CommandBuffer* command)
	{
		__super::EndExcute(command);
	}
}