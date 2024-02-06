#include <graphics_core\renderpass\SkyBoxRenderPass.h>

#include <graphics_core\object\MeshObject.h>

namespace graphics
{
	SkyBoxRenderPass::SkyBoxRenderPass()
		: RenderPass(TEXT("SkyBox Pass"))
	{
		
	}

	SkyBoxRenderPass::~SkyBoxRenderPass()
	{

	}

	void SkyBoxRenderPass::Init()
	{
		m_pSkyBoxMesh = new MeshObject(TEXT("SkyBox Object"));

		m_renderObjects.push_back(m_pSkyBoxMesh);
	}
	
	void SkyBoxRenderPass::Bind(CommandBuffer* command)
	{
		__super::Bind(command);
	}
	
	void SkyBoxRenderPass::BeginExcute(CommandBuffer* command)
	{
		__super::BeginExcute(command);
	}
	
	void SkyBoxRenderPass::Excute(CommandBuffer* command)
	{
		__super::Excute(command);
	}

	void SkyBoxRenderPass::EndExcute(CommandBuffer* command)
	{
		__super::EndExcute(command);
	}
}