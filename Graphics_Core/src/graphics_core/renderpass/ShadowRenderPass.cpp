#include <graphics_core\renderpass\ShadowRenderPass.h>

#include <graphics_core\object\MeshObject.h>

#include <graphics_core\ResourceManager.h>
#include <graphics_core\resource\CameraBuffer.h>
#include <graphics_core\resource\LightBuffer.h>
#include <graphics_core\resource\MaterialBuffer.h>
#include <graphics_core\resource\MeshBuffer.h>

#include <graphics_core\renderer\Renderer.h>
#include <graphics_core\RenderQueue.h>
#include <graphics_core\utility\Culling.h>

#include <graphics_core\LightManager.h>

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
	ShadowRenderPass::ShadowRenderPass()
		: RenderPass(TEXT("Shadow Pass"))
	{
	
	}
	
	ShadowRenderPass::~ShadowRenderPass()
	{
	
	}
	
	void ShadowRenderPass::Init()
	{
		m_pTransBuffer = ResourceManager::GetInstance()->GetBuffer(TEXT("PerObject"));
		m_pMaterialBuffer = ResourceManager::GetInstance()->GetBuffer(TEXT("PerMaterial"));

		m_spotLightShadow = ResourceManager::GetInstance()->CreateMaterialBuffer(TEXT("Spot_Light_Shadow"));
		m_spotLightShadow->SetPipelineID(TEXT("Spot_Light_Shadow"));

		m_cascadedLightShadow = ResourceManager::GetInstance()->CreateMaterialBuffer(TEXT("Cascaded_Light_Shadow"));
		m_cascadedLightShadow->SetPipelineID(TEXT("Cascaded_Light_Shadow"));

		m_pointLightShadow = ResourceManager::GetInstance()->CreateMaterialBuffer(TEXT("Point_Light_Shadow"));
		m_pointLightShadow->SetPipelineID(TEXT("Point_Light_Shadow"));

		m_copy2DDepth = ResourceManager::GetInstance()->CreateMaterialBuffer(TEXT("Copy2DDepth"));
		m_copy2DDepth->SetPipelineID(TEXT("Copy2DDepth"));

		m_copyCubeDepth = ResourceManager::GetInstance()->CreateMaterialBuffer(TEXT("CopyCubeDepth"));
		m_copyCubeDepth->SetPipelineID(TEXT("CopyCubeDepth"));
	}
	
	void ShadowRenderPass::BeginExcute(CommandBuffer* command, CameraBuffer* camBuffer)
	{
		__super::BeginExcute(command, camBuffer);

		for (auto& [_uuid, _lightBuffer] : LightManager::GetInstance()->m_lightBufferMap)
		{
			// 라이트가 꺼져있다면 쉐도우 맵도 만들 필요가 없음
			if (!_lightBuffer->GetEnable()) continue;

			ProjectedShadowInfo _shadowInfo;

			_shadowInfo._lightBuffer = _lightBuffer;

			// 라이트별로 컬링 처리
			BeginCulling(&_shadowInfo);

			if(_shadowInfo._primitives.size() != 0)
				m_projectedShadowInfos.push_back(_shadowInfo);
		}

		BeginGatherCSMShadowPrimitives(camBuffer);

		command->SetResource(*m_pMaterialBuffer, 2, BindFlags::ConstantBuffer, StageFlags::GS);
	}
	
	void ShadowRenderPass::Excute(CommandBuffer* command)
	{
		for (auto& _projectedShadowInfo : m_projectedShadowInfos)
		{
			RenderDepth(command, _projectedShadowInfo);

			CopyToShadowAtlas(command, _projectedShadowInfo);
		}
	}
	
	void ShadowRenderPass::EndExcute(CommandBuffer* command)
	{
		m_projectedShadowInfos.clear();

		__super::EndExcute(command);
	}

	void ShadowRenderPass::BeginCulling(ProjectedShadowInfo* shadowInfo)
	{
		// 라이트 트랜스폼 업데이트
		shadowInfo->_lightBuffer->UpdateLightTransform();

		shadowInfo->_primitives.reserve(RenderQueue::GetInstance()->m_meshObjectList.size());
		shadowInfo->_instanceData.reserve(RenderQueue::GetInstance()->m_meshObjectList.size());

		Frustum _frustum[6];

		uint32 _viewCnt = 0;
		
		switch ((LightType)shadowInfo->_lightBuffer->m_lightInfo._type)
		{
			case LightType::Spot:
			{
				_viewCnt = 1;
				break;
			}
			case LightType::Point:
			{
				_viewCnt = 6;
				break;
			}
			case LightType::Directional:
			{
				// CSM은 따로 처리
				return;
			}
		}

		for (uint32 i = 0; i < _viewCnt; i++)
		{
			_frustum[i] = CullingHelper::CreateFrustum(
				{
					0.01f,
					shadowInfo->_lightBuffer->m_lightInfo._attenuationRadius,
					1.0f,
					shadowInfo->_lightBuffer->m_lightInfo._angle,
					shadowInfo->_lightBuffer->m_lightInfo._position,
					shadowInfo->_lightBuffer->m_shadowInfo._view[i]
				}
			);
		}

		// 프리미티브를 순회하며 선별 작업
		for (auto& [_uuid, _meshObj] : RenderQueue::GetInstance()->m_meshObjectList)
		{
			// 비활성화 상태이거나 매쉬가 설정이 안되어 있을 경우 넘어감
			if(!_meshObj->GetEnable() || _meshObj->GetMeshBuffer() == nullptr) continue;

			switch ((LightType)shadowInfo->_lightBuffer->m_lightInfo._type)
			{
				case LightType::Spot:
				{
					if (CullingHelper::ViewFrustumCullingBoundingBox(
						_frustum[0],
						_meshObj->GetWorld(),
						_meshObj->GetBoundinBoxMin(),
						_meshObj->GetBoundinBoxMax()
					))
					{
						shadowInfo->_primitives.emplace_back(_meshObj.get());
					}
					break;
				}
				case LightType::Point:
				{
					bool _isCulled = true;
					vector<int> _instance;

					for (uint32 i = 0; i < 6; i++)
					{
						// 컬링 패스 실패
						if (CullingHelper::ViewFrustumCullingBoundingBox(
							_frustum[i],
							_meshObj->GetWorld(),
							_meshObj->GetBoundinBoxMin(),
							_meshObj->GetBoundinBoxMax()
						))
						{
							_isCulled = false;
							_instance.push_back(i);
							break;
						}
					}

					if(!_isCulled)
						shadowInfo->_primitives.emplace_back(_meshObj.get());
					break;
				}
			}
		}
	}

	void ShadowRenderPass::BeginGatherCSMShadowPrimitives(CameraBuffer* camBuffer)
	{
		// cascaded가 설정이 안되어있다면 스킾
		if(!LightManager::GetInstance()->m_cascadedSet)
			return;
		
		ProjectedShadowInfo _projectedShadowInfo;

		_projectedShadowInfo._lightBuffer = LightManager::GetInstance()->m_cascadedLightBuffer;


		// 프리미티브를 순회하며 선별 작업
		for (auto& [_uuid, _meshObj] : RenderQueue::GetInstance()->m_meshObjectList)
		{
			// 비활성화 상태이거나 매쉬가 설정이 안되어 있을 경우 넘어감
			if (!_meshObj->GetEnable() || _meshObj->GetMeshBuffer() == nullptr) continue;

			_projectedShadowInfo._primitives.emplace_back(_meshObj.get());
		}

		m_projectedShadowInfos.push_back(_projectedShadowInfo);
	}

	void ShadowRenderPass::RenderDepth(CommandBuffer* command, ProjectedShadowInfo& projectedShadowInfo)
	{	
		command->UpdateBuffer(*m_pMaterialBuffer, 0, &projectedShadowInfo._lightBuffer->GetLightInfo(), sizeof(LightInfo));
		
		command->SetViewport(projectedShadowInfo._lightBuffer->m_pRenderTarget->GetResolution());

		// 라이트 타입에 따른 파이프라인 바인드
		switch ((LightType)projectedShadowInfo._lightBuffer->m_lightInfo._type)
		{
			case LightType::Spot:
			{
				m_spotLightShadow->BindPipelineState(command);
				break;
			}
			case LightType::Point:
			{
				m_pointLightShadow->BindPipelineState(command);
				break;
			}
			case LightType::Directional:
			{
				m_cascadedLightShadow->BindPipelineState(command);
				break;
			}
		}

		// 렌더 타겟을 바인드
		AttachmentClear _clear{ 1.0f };

		command->SetRenderTarget(*projectedShadowInfo._lightBuffer->m_pRenderTarget, 1, &_clear);

		for (auto* _renderObject : projectedShadowInfo._primitives)
		{
			// PerObject 버퍼 업데이트
			command->UpdateBuffer(*m_pTransBuffer, 0, &_renderObject->GetTrans(), sizeof(PerObject));

			auto* _meshObj = reinterpret_cast<MeshObject*>(_renderObject);

			MeshBuffer* _meshBuffer = _meshObj->GetMeshBuffer();

			// 서브 매쉬를 순회 하며 모두 Draw
			for (uint32 _submeshIdx = 0; _submeshIdx < _meshBuffer->GetSubMeshCount(); _submeshIdx++)
			{
				switch (_renderObject->GetRenderType())
				{
					case RenderType::MESH:
					{
						Renderer::GetInstance()->RenderMesh(_meshObj, _submeshIdx);
						break;
					}
					case RenderType::PARTICLE:
					{
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
	}

	void ShadowRenderPass::CopyToShadowAtlas(CommandBuffer* command, ProjectedShadowInfo& projectedShadowInfo)
	{
		command->SetRenderTarget(*LightManager::GetInstance()->m_shadowMapAtlasRT, 0, nullptr);

		switch ((LightType)projectedShadowInfo._lightBuffer->m_lightInfo._type)
		{
			case LightType::Spot:
			{
				Viewport _viewPort
				{
					projectedShadowInfo._lightBuffer->m_lightInfo._uv0[0].x,
					projectedShadowInfo._lightBuffer->m_lightInfo._uv0[0].y,
					projectedShadowInfo._lightBuffer->m_lightInfo._uv1[0].x - projectedShadowInfo._lightBuffer->m_lightInfo._uv0[0].x,
					projectedShadowInfo._lightBuffer->m_lightInfo._uv1[0].y - projectedShadowInfo._lightBuffer->m_lightInfo._uv0[0].y,
					0.f, 1.0f
				};

				command->SetViewport(_viewPort);

				m_copy2DDepth->BindPipelineState(command);

				m_copy2DDepth->SetTexture(TEXT("ShadowDepthTexture"), projectedShadowInfo._lightBuffer->m_pDepthTexture);
				
				m_copy2DDepth->BindResource(command);

				Renderer::GetInstance()->DrawRectangle();
				break;
			}
			case LightType::Point:
			{
				m_copyCubeDepth->SetTexture(TEXT("ShadowDepthArrayTexture"), projectedShadowInfo._lightBuffer->m_pDepthTexture);
				
				m_copyCubeDepth->BindPipelineState(command);

				for (uint32 i = 0; i < 6; i++)
				{
					Viewport _viewPort = {
						projectedShadowInfo._lightBuffer->m_lightInfo._uv0[i].x,
						projectedShadowInfo._lightBuffer->m_lightInfo._uv0[i].y,
						projectedShadowInfo._lightBuffer->m_lightInfo._uv1[i].x - projectedShadowInfo._lightBuffer->m_lightInfo._uv0[i].x,
						projectedShadowInfo._lightBuffer->m_lightInfo._uv1[i].y - projectedShadowInfo._lightBuffer->m_lightInfo._uv0[i].y,
						0.f, 1.0f
					};

					m_copyCubeDepth->SetInteger(TEXT("index"), i);

					m_copyCubeDepth->BindResource(command);

					command->SetViewport(_viewPort);

					Renderer::GetInstance()->DrawRectangle();
				}

				break;
			}
			case LightType::Directional:
			{
				return;
				m_copyCubeDepth->SetTexture(TEXT("ShadowDepthArrayTexture"), projectedShadowInfo._lightBuffer->m_pDepthTexture);

				m_copyCubeDepth->BindPipelineState(command);

				for (uint32 i = 0; i < NUM_CASCADES; i++)
				{
					Viewport _viewPort = {
						projectedShadowInfo._lightBuffer->m_lightInfo._uv0[i].x,
						projectedShadowInfo._lightBuffer->m_lightInfo._uv0[i].y,
						projectedShadowInfo._lightBuffer->m_lightInfo._uv1[i].x - projectedShadowInfo._lightBuffer->m_lightInfo._uv0[i].x,
						projectedShadowInfo._lightBuffer->m_lightInfo._uv1[i].y - projectedShadowInfo._lightBuffer->m_lightInfo._uv0[i].y,
						0.f, 1.0f
					};

					m_copyCubeDepth->SetInteger(TEXT("index"), i);

					m_copyCubeDepth->BindResource(command);

					command->SetViewport(_viewPort);

					Renderer::GetInstance()->DrawRectangle();
				}

				return;
			}
		}

		
	}
}