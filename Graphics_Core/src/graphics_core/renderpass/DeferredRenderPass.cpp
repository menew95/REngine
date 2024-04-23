#include <graphics_core\renderpass\DeferredRenderPass.h>

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
#include <graphics_module\Sampler.h>
#include <graphics_module\Texture.h>
#include <graphics_module\RenderTarget.h>

#include <common\AssetPath.h>


namespace graphics
{
	DeferredRenderPass::DeferredRenderPass()
		: RenderPass(TEXT("Deferred Pass"))
	{
	
	}
	
	DeferredRenderPass::~DeferredRenderPass()
	{
	
	}
	
	void DeferredRenderPass::Init()
	{
		m_pTransBuffer = ResourceManager::GetInstance()->GetBuffer(TEXT("PerObject"));
	
		m_pRenderTarget = ResourceManager::GetInstance()->GetRenderTarget(TEXT("Deferred"));
	}
	
	void DeferredRenderPass::BeginExcute(CommandBuffer* command, CameraBuffer* camBuffer)
	{
		__super::BeginExcute(command, camBuffer);

		AttachmentClear _attachmentClear[6] =
		{
			{ math::Vector4::Zero, 0 },
			{ math::Vector4::Zero, 1 },
			{ math::Vector4::Zero, 2 },
			{ math::Vector4::Zero, 3 },
			{ math::Vector4::Zero, 4 },
			{ 1.0f, 0u }
		};

		command->SetRenderTarget(*m_pRenderTarget, 6, _attachmentClear);

		command->SetViewport(m_pRenderTarget->GetResolution());
	}
	
	void DeferredRenderPass::Excute(CommandBuffer* command)
	{
		__super::Excute(command);

		for (auto* _matBuf : m_materialBufferList)
		{
			assert(_matBuf != nullptr);

			if(_matBuf->GetRenderObjectList().size() <= 0) continue;

			_matBuf->BindPipelineState(command);
			
			_matBuf->BindResource(command);

			for (auto& [_renderObj, _submeshIdx] : _matBuf->GetRenderObjectList())
			{
				// 렌더 컴포넌트가 비활성화 이거나 컬링 상태일 경우엔 그리지 않음
				if(!_renderObj->GetEnable() || _renderObj->GetCulling())
					continue;

				auto* _meshObj = reinterpret_cast<MeshObject*>(_renderObj);

				// 머티리얼이 그리기로 한 서브 매쉬의 인덱스가 메쉬 버퍼의 서브매쉬보다 크면 그릴 서브 매쉬가 없음
				if(_meshObj->GetMeshBuffer() == nullptr || _submeshIdx > _meshObj->GetMeshBuffer()->GetSubMeshCount())
					continue;

				command->UpdateBuffer(*m_pTransBuffer, 0, &_renderObj->GetTrans(), sizeof(PerObject));

				switch (_renderObj->GetRenderType())
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
	
	void DeferredRenderPass::EndExcute(CommandBuffer* command)
	{
		__super::EndExcute(command);
	}
}