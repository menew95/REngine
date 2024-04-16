#include <graphics_core\renderpass\PostProcessRenderPass.h>
#include <graphics_core\object\MeshObject.h>

#include <graphics_core\ResourceManager.h>
#include <graphics_core\resource\CameraBuffer.h>
#include <graphics_core\resource\MaterialBuffer.h>
#include <graphics_core\resource\MeshBuffer.h>

#include <graphics_core\renderer\Renderer.h>

#include <graphics_module\CommandBuffer.h>

#include <graphics_module\PipelineState.h>
#include <graphics_module\PipelineLayout.h>

#include <graphics_module\Shader.h>
#include <graphics_module\Buffer.h>
#include <graphics_module\Sampler.h>
#include <graphics_module\Texture.h>
#include <graphics_module\RenderTarget.h>

#include <common\AssetPath.h>

namespace graphics
{
	PostProcessRenderPass::PostProcessRenderPass()
		: RenderPass(TEXT("PostProcess Pass"))
	{
	}

	PostProcessRenderPass::~PostProcessRenderPass()
	{
		delete m_pScreenMesh;
	}
	
	void PostProcessRenderPass::Init()
	{
		m_pScreenMesh = new MeshObject(TEXT("Screen Object"));

		m_pScreenMesh->SetMeshBuffer(ResourceManager::GetInstance()->GetMeshBuffer(TEXT("00000000-0000-0000-0000-000000000002")));

		m_pRenderTarget = ResourceManager::GetInstance()->GetRenderTarget(TEXT("MainFrame"));

		m_pMainFrameColorTexture = ResourceManager::GetInstance()->GetTexture(TEXT("MainFrame"));

		{
			MaterialBuffer* _matBuffer = ResourceManager::GetInstance()->CreateMaterialBuffer(TEXT("Tone Mapping"));

			_matBuffer->SetPipelineID(TEXT("ToneMapping_Uncharted"));

			_matBuffer->SetTexture(TEXT("gTexture"), ResourceManager::GetInstance()->GetTexture(TEXT("MainFrame")));
		}
	}

	void PostProcessRenderPass::BeginExcute(CommandBuffer* command, CameraBuffer* camBuffer)
	{
		m_pCameraColorTexture = camBuffer->GetColorTexture();
	}
	
	void PostProcessRenderPass::Excute(CommandBuffer* command)
	{
		__super::Excute(command);

		for (auto* _matBuf : m_materialBufferList)
		{
			assert(_matBuf != nullptr);

			_matBuf->BindPipelineState(command);

			_matBuf->BindResource(command);

			Renderer::GetInstance()->RenderMesh(m_pScreenMesh, 0);

			TextureLocation _empty;

			command->CopyTexture(*m_pCameraColorTexture, _empty, *m_pMainFrameColorTexture, _empty, m_pMainFrameColorTexture->GetResolution());
		}
	}
	
	void PostProcessRenderPass::EndExcute(CommandBuffer* command)
	{
	
	}
}