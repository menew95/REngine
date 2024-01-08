#include <graphics_core\RenderPass.h>

#include <graphics_module\CommandBuffer.h>

namespace graphics
{
	RenderPass::RenderPass(tstring passName)
		: m_passName(passName)
	{

	}

	RenderPass::~RenderPass()
	{

	}

	void RenderPass::Bind(CommandBuffer* command)
	{
		command->SetPipelineState(*m_pPipelineState);

		command->SetResources(*m_pPipelineLayout);
	}
}