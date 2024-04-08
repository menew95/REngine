#include <graphics_core\renderpass\ShadowRenderPass.h>

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
	ShadowRenderPass::ShadowRenderPass()
		: RenderPass(TEXT("Shadow Pass"))
	{
	
	}
	
	ShadowRenderPass::~ShadowRenderPass()
	{
	
	}
	
	void ShadowRenderPass::Init()
	{
	
	}
	
	void ShadowRenderPass::Bind(CommandBuffer* command)
	{
	
	}
	
	void ShadowRenderPass::BeginExcute(CommandBuffer* command, CameraBuffer* camBuffer)
	{
	
	}
	
	void ShadowRenderPass::Excute(CommandBuffer* command)
	{
	
	}
	
	void ShadowRenderPass::EndExcute(CommandBuffer* command)
	{
	
	}
}