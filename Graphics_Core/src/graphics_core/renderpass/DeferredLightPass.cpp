#include <graphics_core\renderpass\DeferredLightPass.h>
#include <graphics_core\LightManager.h>

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
	DeferredLightPass::DeferredLightPass()
		: RenderPass(TEXT("Deferred Light Pass"))
	{
	
	}
	
	DeferredLightPass::~DeferredLightPass()
	{
		delete m_pScreenMesh;
	}
	
	void DeferredLightPass::Init()
	{
		m_pMatBuffer = ResourceManager::GetInstance()->CreateMaterialBuffer(TEXT("Deferred Light"));

		m_pMatBuffer->SetPipelineID(TEXT("Deferred Light"));

		m_pMatBuffer->SetTexture(TEXT("gCameraGBufferTexture0"), ResourceManager::GetInstance()->GetTexture(TEXT("Albedo")));
		m_pMatBuffer->SetTexture(TEXT("gCameraGBufferTexture1"), ResourceManager::GetInstance()->GetTexture(TEXT("Emissive")));
		m_pMatBuffer->SetTexture(TEXT("gCameraGBufferTexture2"), ResourceManager::GetInstance()->GetTexture(TEXT("Normal")));
		m_pMatBuffer->SetTexture(TEXT("gCameraGBufferTexture3"), ResourceManager::GetInstance()->GetTexture(TEXT("World")));
		m_pMatBuffer->SetTexture(TEXT("gCameraGBufferTexture4"), ResourceManager::GetInstance()->GetTexture(TEXT("Flag")));
		
		m_pMatBuffer->SetTexture(TEXT("gPreFilteredMap"), ResourceManager::GetInstance()->GetTexture(TEXT("PreFilteredMap")));
		m_pMatBuffer->SetTexture(TEXT("gIrradianceMap"), ResourceManager::GetInstance()->GetTexture(TEXT("IrradianceMap")));
		m_pMatBuffer->SetTexture(TEXT("gIntegrateBRDFMap"), ResourceManager::GetInstance()->GetTexture(TEXT("BRDFLookUpTable")));
		
		m_pMatBuffer->SetResource(TEXT("gLightTexture"), LightManager::GetInstance()->m_lightBuffer);

		m_pScreenMesh = new MeshObject(TEXT("Screen Object"));

		m_pScreenMesh->SetMeshBuffer(ResourceManager::GetInstance()->GetMeshBuffer(TEXT("00000000-0000-0000-0000-000000000002")));
	
		m_pRenderTarget = ResourceManager::GetInstance()->GetRenderTarget(TEXT("MainFrame"));
	}

	void DeferredLightPass::BeginExcute(CommandBuffer* command, CameraBuffer* camBuffer)
	{
		__super::BeginExcute(command, camBuffer);

		//command->SetRenderTarget(*camBuffer->GetRenderTarget(), 0, nullptr);

		//command->SetViewport(camBuffer->GetRenderTarget()->GetResolution());

		/*AttachmentClear _attachmentClear[1] =
		{
			{ math::Vector4::Zero, 0 }
		};*/

		//command->SetRenderTarget(*m_pRenderTarget, 1, _attachmentClear);

		command->SetRenderTarget(*m_pRenderTarget, 0, nullptr);

		command->SetViewport(m_pRenderTarget->GetResolution());
	}
	
	void DeferredLightPass::Excute(CommandBuffer* command)
	{
		__super::Excute(command);

		m_pMatBuffer->BindPipelineState(command);
		
		m_pMatBuffer->BindResource(command);

		Renderer::GetInstance()->RenderMesh(m_pScreenMesh, 0);
	}
	
	void DeferredLightPass::EndExcute(CommandBuffer* command)
	{
		__super::EndExcute(command);
	}
}