#include <graphics_core\renderer\Renderer.h>
#include <graphics_core\RenderPass.h>

#include <graphics_module\CommandBuffer.h>

namespace graphics
{
	Renderer::Renderer(CommandBuffer* command)
	: m_pCommandBuffer(command)
	{
	}

	void Renderer::RenderMesh(vector<RenderObject*>& renderObjects, vector<RenderPass*>& renderPassList)
	{
	}

	void Renderer::RenderSkyBox(RenderPass* renderPass)
	{
		renderPass->Bind(m_pCommandBuffer);
	}
}