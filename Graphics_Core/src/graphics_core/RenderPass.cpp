#include <graphics_core\RenderPass.h>

#include <graphics_module\CommandBuffer.h>

#include <graphics_core\object\RenderObject.h>

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

	void RenderPass::AddRenderObject(RenderObject* obj)
	{
		m_renderObjects.emplace_back(obj);
	}

	void RenderPass::RemoveRenderObject(RenderObject* obj)
	{
		auto _it = find(std::begin(m_renderObjects), std::end(m_renderObjects), obj);

		if(_it != std::end(m_renderObjects))
			m_renderObjects.erase(_it);
	}
}