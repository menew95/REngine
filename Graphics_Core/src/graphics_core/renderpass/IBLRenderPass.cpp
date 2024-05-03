#include <graphics_core\renderpass\IBLRenderPass.h>

#include <graphics_core\object\MeshObject.h>

#include <graphics_core\ResourceManager.h>
#include <graphics_core\resource\CameraBuffer.h>
#include <graphics_core\resource\MaterialBuffer.h>
#include <graphics_core\resource\MeshBuffer.h>
#include <graphics_core\resource\TextureBuffer.h>

#include <graphics_core\renderer\Renderer.h>

#include <graphics_module\CommandBuffer.h>

#include <graphics_module\PipelineState.h>
#include <graphics_module\PipelineLayout.h>

#include <graphics_module\Shader.h>
#include <graphics_module\Sampler.h>
#include <graphics_module\Texture.h>
#include <graphics_module\Buffer.h>
#include <graphics_module\RenderTarget.h>

#include <common\AssetPath.h>

namespace graphics
{
	IBLRenderPass::IBLRenderPass()
		: RenderPass(TEXT("IBL Pass"))
	{
	
	}
	
	IBLRenderPass::~IBLRenderPass()
	{

	}
	
	void IBLRenderPass::Init()
	{
		m_integrateBRDF_Material = ResourceManager::GetInstance()->CreateMaterialBuffer(TEXT("IntergrateBRDF"));

		m_integrateBRDF_Material->SetPipelineID(TEXT("IBL_BRDF"));

		m_preFiltered_Material = ResourceManager::GetInstance()->CreateMaterialBuffer(TEXT("PreFiltered"));

		m_preFiltered_Material->SetPipelineID(TEXT("IBL_Radiance"));

		m_irradiance_Material = ResourceManager::GetInstance()->CreateMaterialBuffer(TEXT("Irradiance"));

		m_irradiance_Material->SetPipelineID(TEXT("IBL_Irradiance"));

		m_pCubeMeshBuffer = ResourceManager::GetInstance()->GetMeshBuffer(TEXT("00000000-0000-0000-0000-000000000001"));
	
		m_integrateBRDF_RT = ResourceManager::GetInstance()->GetRenderTarget(TEXT("BRDFLookUpTable"));

		m_preFiltered_RT = ResourceManager::GetInstance()->GetRenderTarget(TEXT("PreFilteredMap"));

		m_irradiance_RT = ResourceManager::GetInstance()->GetRenderTarget(TEXT("IrradianceMap"));

		m_pBuffer = ResourceManager::GetInstance()->GetBuffer(TEXT("PerObject"));

		CreateIntegrateBRDFMap(Renderer::GetInstance()->GetCommandBuffer());

		auto* _texture = ResourceManager::GetInstance()->GetTextureBuffer(TEXT("fe6f153f-d693-4675-9b0e-65b8be91f35b"));
		
		CreatePreFilteredMap(Renderer::GetInstance()->GetCommandBuffer(), _texture);
		CreateIrradianceMap(Renderer::GetInstance()->GetCommandBuffer(), _texture);
	}

	//void IBLRenderPass::BeginExcute(CommandBuffer* command, CameraBuffer* camBuffer)
	//{
	//	__super::BeginExcute(command, camBuffer);
	//}

	//void IBLRenderPass::Excute(CommandBuffer* command)
	//{

	//}

	//void IBLRenderPass::EndExcute(CommandBuffer* command)
	//{
	//	__super::EndExcute(command);
	//}
	
	void IBLRenderPass::CreateIntegrateBRDFMap(CommandBuffer* command)
	{
		command->BeginEvent(TEXT("Intergrate BRDF Map"));

		command->SetRenderTarget(*m_integrateBRDF_RT, 0, nullptr);

		command->SetViewport({0, 0, m_integrateBRDF_RT->GetResolution().x, m_integrateBRDF_RT->GetResolution().y});

		m_integrateBRDF_Material->BindPipelineState(command);

		Renderer::GetInstance()->DrawRectangle();

		command->ClearState();

		command->EndEvent();
	}
	
	void IBLRenderPass::CreatePreFilteredMap(CommandBuffer* command, TextureBuffer* textureBuffer)
	{
		command->BeginEvent(TEXT("PreFiltered Map"));

		m_preFiltered_Material->SetTexture(TEXT("gCubeMap"), textureBuffer);

		m_preFiltered_Material->BindPipelineState(command);

		m_preFiltered_Material->BindResource(command);

		command->SetResource(*m_pBuffer, 2, BindFlags::ConstantBuffer, StageFlags::PS);

		int mipmapSize = 5;

		auto _preFilteredTexture = ResourceManager::GetInstance()->GetTexture(TEXT("PreFilteredMap"));

		vector<RenderTarget*> _mipRTVs;

		CreatePreFilteredRenderTargetFromTexture(_mipRTVs, _preFilteredTexture);

		struct PreFilteredData
		{
			int _face;
			float _roughness;
		};

		PreFilteredData _data;


		for (size_t mipIdx = 0; mipIdx < mipmapSize; mipIdx++)
		{
			float _texWidth = static_cast<float>(m_preFiltered_RT->GetResolution().x * pow(0.5f, mipIdx));
			float _texHeight = static_cast<float>(m_preFiltered_RT->GetResolution().y * pow(0.5f, mipIdx));

			command->SetViewport({ 0, 0, _texWidth, _texHeight});

			for (uint faceIdx = 0; faceIdx < 6; faceIdx++)
			{
				size_t _arrayIdx = 6 * mipIdx + faceIdx;
				command->SetRenderTarget(*_mipRTVs[_arrayIdx], 0, nullptr);

				_data._face = faceIdx;
				_data._roughness = static_cast<float>(mipIdx) / static_cast<float>(mipmapSize);

				command->UpdateBuffer(*m_pBuffer, 0, &_data, sizeof(PreFilteredData));

				command->SetVertexBuffer(*m_pCubeMeshBuffer->GetBuffer());
				command->SetIndexBuffer(*m_pCubeMeshBuffer->GetSubMesh(0).GetBuffer());

				command->DrawIndexed(m_pCubeMeshBuffer->GetSubMesh(0).GetIndexCount(), 0);
			}
		}

		for (int mipIdx = 0; mipIdx < mipmapSize; ++mipIdx)
		{
			for (int faceIdx = 0; faceIdx < 6; ++faceIdx)
			{
				int _arrayIdx = 6 * mipIdx + faceIdx;

				ResourceManager::GetInstance()->ReleaseRenderTarget(_mipRTVs[_arrayIdx]);
			}
		}

		command->ClearState();

		command->EndEvent();
	}
	
	void IBLRenderPass::CreateIrradianceMap(CommandBuffer* command, TextureBuffer* textureBuffer)
	{
		command->BeginEvent(TEXT("Irradiance Map"));

		command->SetViewport({ 0, 0, 512.f, 512.f });

		m_irradiance_Material->BindPipelineState(command);

		m_irradiance_Material->SetTexture(TEXT("gCubeMap"), textureBuffer);

		m_irradiance_Material->BindResource(command);

		command->SetResource(*m_pBuffer, 2, BindFlags::ConstantBuffer, StageFlags::PS);

		auto _irradianceTexture = ResourceManager::GetInstance()->GetTexture(TEXT("IrradianceMap"));

		vector<RenderTarget*> _rtvs;

		CreateIrradianceRenderTargetFromTexture(_rtvs, _irradianceTexture);

		command->SetViewport({ 0, 0, m_irradiance_RT->GetResolution().x, m_irradiance_RT ->GetResolution().y});

		for (uint faceIdx = 0; faceIdx < 6; faceIdx++)
		{
			command->SetRenderTarget(*_rtvs[faceIdx], 0, nullptr);

			command->UpdateBuffer(*m_pBuffer, 0, &faceIdx, sizeof(uint));

			command->SetVertexBuffer(*m_pCubeMeshBuffer->GetBuffer());
			command->SetIndexBuffer(*m_pCubeMeshBuffer->GetSubMesh(0).GetBuffer());

			command->DrawIndexed(m_pCubeMeshBuffer->GetSubMesh(0).GetIndexCount(), 0);
		}

		for (int faceIdx = 0; faceIdx < 6; ++faceIdx)
		{
			ResourceManager::GetInstance()->ReleaseRenderTarget(_rtvs[faceIdx]);
		}

		command->ClearState();

		command->EndEvent();
	}

	void IBLRenderPass::CreatePreFilteredRenderTargetFromTexture(vector<RenderTarget*>& rtvs, Texture* texture)
	{
		int mipmapSize = 5;

		rtvs.resize(6 * mipmapSize);

		RenderTargetDesc _renderTargetDesc;
		_renderTargetDesc._sample = 0;

		_renderTargetDesc._attachments.resize(1);
		_renderTargetDesc._attachments[0]._arraySize = 1;
		_renderTargetDesc._attachments[0]._renderTargetType = RenderTargetType::RenderTarget;
		_renderTargetDesc._attachments[0]._resource = texture;

		for (int mipIdx = 0; mipIdx < mipmapSize; ++mipIdx)
		{
			for (int faceIdx = 0; faceIdx < 6; ++faceIdx)
			{
				_renderTargetDesc._extend
					=
				{
					texture->GetResolution()._width * powf(0.5f, (float)mipIdx),
					texture->GetResolution()._height * powf(0.5f, (float)mipIdx)
				};

				_renderTargetDesc._attachments[0]._arrayLayer = faceIdx;
				_renderTargetDesc._attachments[0]._mipLevel = mipIdx;


				int _arrayIdx = 6 * mipIdx + faceIdx;
				tstring renderTargetID = TEXT("PreFilteredMap") + to_wstring(_arrayIdx);

				rtvs[_arrayIdx] = ResourceManager::GetInstance()->CreateRenderTarget(renderTargetID, _renderTargetDesc);
			}
		}
	}

	void IBLRenderPass::CreateIrradianceRenderTargetFromTexture(vector<RenderTarget*>& rtvs, Texture* texture)
	{
		rtvs.resize(6);

		RenderTargetDesc _renderTargetDesc;
		_renderTargetDesc._extend
			=
		{
			static_cast<float>(texture->GetResolution()._width),
			static_cast<float>(texture->GetResolution()._height)
		};
		_renderTargetDesc._sample = 0;

		_renderTargetDesc._attachments.resize(1);
		_renderTargetDesc._attachments[0]._arraySize = 1;
		_renderTargetDesc._attachments[0]._mipLevel = 0;

		_renderTargetDesc._attachments[0]._resource = texture;
		_renderTargetDesc._attachments[0]._renderTargetType = RenderTargetType::RenderTarget;

		for (int faceIdx = 0; faceIdx < 6; ++faceIdx)
		{
			_renderTargetDesc._attachments[0]._arrayLayer = faceIdx;

			tstring renderTargetID = TEXT("IrradianceMap") + to_wstring(faceIdx);

			rtvs[faceIdx] = ResourceManager::GetInstance()->CreateRenderTarget(renderTargetID, _renderTargetDesc);
		}
	}
}