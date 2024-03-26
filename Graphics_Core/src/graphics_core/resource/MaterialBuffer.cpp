#include <graphics_core\resource\MaterialBuffer.h>
#include <graphics_core\ResourceManager.h>
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

	void MaterialBuffer::SetRenderPass(const tstring& id)
	{
		if(m_pRenderPass != nullptr)
			m_pRenderPass->RemoveMaterialBuffer(this);

		m_pRenderPass = graphics::ResourceManager::GetInstance()->GetRenderPass(id);

		assert(m_pRenderPass != nullptr);

		m_pRenderPass->AddMaterialBuffer(this);
	}

	void MaterialBuffer::SetPipelineID(const tstring& pipelineID)
	{
		m_pPipelineState = graphics::ResourceManager::GetInstance()->GetPipelineState(pipelineID);

		m_pPipelineLayout = graphics::ResourceManager::GetInstance()->GetPipelineLayout(pipelineID);
	}

	void MaterialBuffer::AddRenderObject(RenderObject* obj)
	{
		//m_pRenderPass->AddRenderObject(obj);

		m_renderObjectList.emplace_back(obj);
	}

	void MaterialBuffer::RemoveRenderObject(RenderObject* obj)
	{
		//m_pRenderPass->RemoveRenderObject(obj);

		auto _it = find(std::begin(m_renderObjectList), std::end(m_renderObjectList), obj);

		if (_it != std::end(m_renderObjectList))
			m_renderObjectList.erase(_it);
	}

	void MaterialBuffer::BindResource(CommandBuffer* command)
	{
		if(m_pPipelineLayout == nullptr)
			return;

		command->SetResources(*m_pPipelineLayout);
	}
}