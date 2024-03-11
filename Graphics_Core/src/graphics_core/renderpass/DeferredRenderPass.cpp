#include <graphics_core\renderpass\DeferredRenderPass.h>

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
	DeferredRenderPass::DeferredRenderPass()
		: RenderPass(TEXT("Deferred Pass"))
	{
	
	}
	
	DeferredRenderPass::~DeferredRenderPass()
	{
	
	}
	
	void DeferredRenderPass::Init()
	{
		m_pPipelineState = ResourceManager::GetInstance()->GetPipelineState(TEXT("Deferred"));

		m_pPipelineLayout = ResourceManager::GetInstance()->GetPipelineLayout(TEXT("Deferred"));
	}
	
	void DeferredRenderPass::Bind(CommandBuffer* command)
	{
		__super::Bind(command);
	}
	
	void DeferredRenderPass::BeginExcute(CommandBuffer* command, CameraBuffer* camBuffer)
	{
		__super::BeginExcute(command, camBuffer);
	}
	
	void DeferredRenderPass::Excute(CommandBuffer* command)
	{
		__super::Excute(command);
	}
	
	void DeferredRenderPass::EndExcute(CommandBuffer* command)
	{
		__super::EndExcute(command);
	}
}