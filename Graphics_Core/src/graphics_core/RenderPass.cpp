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

	void RenderPass::Bind(CommandBuffer* command)
	{
		command->SetPipelineState(*m_pPipelineState);

		command->SetResources(*m_pPipelineLayout);
	}

	void RenderPass::BeginExcute(CommandBuffer* command, CameraBuffer* camBuffer)
	{
		Bind(command);

		command->BeginEvent(m_passName.c_str());
	}

	void RenderPass::Excute(CommandBuffer* command)
	{
		for (auto* renderObj : m_renderObjects)
		{
			if(!renderObj->GetEnable() || !renderObj->GetCulling())
				continue;

		}
	}

	void RenderPass::EndExcute(CommandBuffer* command)
	{
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

	/*void RenderPass::AddRenderObject(RenderObject* obj)
	{
		m_renderObjects.emplace_back(obj);
	}

	void RenderPass::RemoveRenderObject(RenderObject* obj)
	{
		auto _it = find(std::begin(m_renderObjects), std::end(m_renderObjects), obj);

		if(_it != std::end(m_renderObjects))
			m_renderObjects.erase(_it);
	}*/
}