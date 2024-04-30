#include <graphics_core\renderpass\ShadowRenderPass.h>

#include <graphics_core\object\MeshObject.h>

#include <graphics_core\ResourceManager.h>
#include <graphics_core\resource\CameraBuffer.h>
#include <graphics_core\resource\LightBuffer.h>
#include <graphics_core\resource\MaterialBuffer.h>

#include <graphics_core\renderer\Renderer.h>

#include <graphics_core\LightManager.h>

#include <graphics_module\CommandBuffer.h>

#include <graphics_module\PipelineState.h>
#include <graphics_module\PipelineLayout.h>

#include <graphics_module\Shader.h>
#include <graphics_module\Sampler.h>
#include <graphics_module\Texture.h>

#include <common\AssetPath.h>

namespace graphics
{
	ShadowRenderPass::ShadowRenderPass()
		: RenderPass(TEXT("Shadow Pass"))
	{
	
	}
	
	ShadowRenderPass::~ShadowRenderPass()
	{
	
	}
	
	void ShadowRenderPass::Init()
	{
		
	}
	
	void ShadowRenderPass::BeginExcute(CommandBuffer* command, CameraBuffer* camBuffer)
	{
		m_spotLightShadow = ResourceManager::GetInstance()->CreateMaterialBuffer(TEXT("Spot Light Shadow"));

		m_spotLightShadow->SetPipelineID(TEXT("Spot Light Shadow"));

		m_cascadedLightShadow = ResourceManager::GetInstance()->CreateMaterialBuffer(TEXT("Cascaded Light Shadow"));

		m_cascadedLightShadow->SetPipelineID(TEXT("Cascaded Light Shadow"));

		m_pointLightShadow = ResourceManager::GetInstance()->CreateMaterialBuffer(TEXT("Point Light Shadow"));

		m_pointLightShadow->SetPipelineID(TEXT("Point Light Shadow"));
	}
	
	void ShadowRenderPass::Excute(CommandBuffer* command)
	{
		for (auto& [_uuid, _lightBuffer] : LightManager::GetInstance()->m_lightBufferMap)
		{
			// 라이트가 꺼져있다면 쉐도우 맵도 만들 필요가 없음
			if(!_lightBuffer->GetEnable()) continue;

			switch (_lightBuffer->m_lightInfo._type)
			{
				case (uint32)LightType::Spot:
				{
					m_spotLightShadow->BindPipelineState(command);
					break;
				}
				case (uint32)LightType::Directional:
				{
					m_cascadedLightShadow->BindPipelineState(command);
					break;
				}
				case (uint32)LightType::Point:
				{
					m_pointLightShadow->BindPipelineState(command);
					break;
				}
				default:
				{
					assert(false);
					break;
				}
			}

		}
	}
	
	void ShadowRenderPass::EndExcute(CommandBuffer* command)
	{
	
	}
}