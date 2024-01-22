#include <graphics_core\resource\MaterialBuffer.h>

#include <graphics_core\RenderPass.h>

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

	void MaterialBuffer::AddRenderObject(RenderObject* obj)
	{
		m_pRenderPass->AddRenderObject(obj);
	}

	void MaterialBuffer::RemoveRenderObject(RenderObject* obj)
	{
		m_pRenderPass->RemoveRenderObject(obj);
	}
}