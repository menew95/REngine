#include <graphics_core\renderpass\PostProcessRenderPass.h>
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
	PostProcessRenderPass::PostProcessRenderPass()
		: RenderPass(TEXT("PostProcess Pass"))
	{
	}

	PostProcessRenderPass::~PostProcessRenderPass()
	{
	
	}
	
	void PostProcessRenderPass::Init()
	{
	
	}

	void PostProcessRenderPass::BeginExcute(CommandBuffer* command, CameraBuffer* camBuffer)
	{
	
	}
	
	void PostProcessRenderPass::Excute(CommandBuffer* command)
	{
	
	}
	
	void PostProcessRenderPass::EndExcute(CommandBuffer* command)
	{
	
	}
}