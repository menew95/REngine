#include <graphics_core\RenderPass.h>

#include <graphics_module\CommandBuffer.h>

#include <graphics_core\object\RenderObject.h>
#include <graphics_core\object\MeshObject.h>


#include <graphics_core\resource\MeshBuffer.h>
#include <graphics_core\resource\MaterialBuffer.h>

namespace graphics
{
	RenderPass::RenderPass(tstring passName)
		: m_passName(passName)
	{

	}

	RenderPass::~RenderPass()
	{

	}

	void RenderPass::BeginExcute(CommandBuffer* command, CameraBuffer* camBuffer)
	{
		command->BeginEvent(m_passName.c_str());
	}

	void RenderPass::Excute(CommandBuffer* command)
	{

	}

	void RenderPass::EndExcute(CommandBuffer* command)
	{
		command->EndRenderPass();

		command->EndEvent();
	}

	void RenderPass::AddMaterialBuffer(MaterialBuffer* matBuffer)
	{
		m_materialBufferList.emplace_back(matBuffer);
	}

	void RenderPass::RemoveMaterialBuffer(MaterialBuffer* matBuffer)
	{
		auto _it = find(std::begin(m_materialBufferList), std::end(m_materialBufferList), matBuffer);

		if (_it != std::end(m_materialBufferList))
			m_materialBufferList.erase(_it);
	}
}