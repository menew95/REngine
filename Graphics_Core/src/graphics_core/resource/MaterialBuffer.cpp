#include <graphics_core\resource\MaterialBuffer.h>
#include <graphics_core\resource\MaterialPropertyBlock.h>
#include <graphics_core\resource\TextureBuffer.h>
#include <graphics_core\ResourceManager.h>
#include <graphics_core\RenderPass.h>

#include <graphics_module\CommandBuffer.h>
#include <graphics_module\PipelineState.h>

namespace graphics
{
	MaterialBuffer::MaterialBuffer()
	{
		m_materialPropertyBlock = make_unique<MaterialPropertyBlock>();
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

		assert(m_pPipelineState != nullptr);

		m_materialPropertyBlock->SetProperty(m_pPipelineState->GetPropertyDesc());

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

	void MaterialBuffer::SetColor(const tstring& name, const Color& value)
	{
		m_materialPropertyBlock->SetColor(name, value);
	}

	void MaterialBuffer::SetVector4(const tstring& name, const Vector4& value)
	{
		m_materialPropertyBlock->SetVector4(name, value);
	}

	void MaterialBuffer::SetFloat(const tstring& name, float value)
	{
		m_materialPropertyBlock->SetFloat(name, value);
	}

	void MaterialBuffer::SetTexture(const tstring& name, TextureBuffer* texture)
	{
		m_materialPropertyBlock->SetTexture(name, texture);
	}

	void MaterialBuffer::SetInteger(const tstring& name, int value)
	{
		m_materialPropertyBlock->SetInteger(name, value);
	}

	void MaterialBuffer::BindPipelineState(CommandBuffer* command)
	{
		if(m_pPipelineState == nullptr)
			return;

		command->SetPipelineState(*m_pPipelineState);
	}

	void MaterialBuffer::BindResource(CommandBuffer* command)
	{
		m_materialPropertyBlock->BindProperty(command);
		
		if(m_pPipelineLayout == nullptr)
			return;

		command->SetResources(*m_pPipelineLayout);
	}
}