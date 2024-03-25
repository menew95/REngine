#include <graphics_core\resource\MaterialBuffer.h>

#include <graphics_core\RenderPass.h>

#include <graphics_module\CommandBuffer.h>

namespace graphics
{
	MaterialBuffer::MaterialBuffer()
	{

	}

	MaterialBuffer::~MaterialBuffer()
	{

	}

	void MaterialBuffer::SetName(const char* name)
	{

	}

	const tstring& MaterialBuffer::GetPipelineID()
	{
		return m_pRenderPass->GetRenderPassName();
	}

	void MaterialBuffer::SetPipelineID(const tstring& pipelineID)
	{

	}

	void MaterialBuffer::AddRenderObject(RenderObject* obj)
	{
		m_pRenderPass->AddRenderObject(obj);
	}

	void MaterialBuffer::RemoveRenderObject(RenderObject* obj)
	{
		m_pRenderPass->RemoveRenderObject(obj);
	}

	void MaterialBuffer::BindResource(CommandBuffer* command)
	{
		if(m_pPipelineLayout == nullptr)
			return;

		command->SetResources(*m_pPipelineLayout);
	}
}