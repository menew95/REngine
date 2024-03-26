#include <graphics_core\renderpass\GridRenderPass.h>

#include <graphics_core\object\MeshObject.h>

#include <graphics_core\ResourceManager.h>
#include <graphics_core\resource\CameraBuffer.h>
#include <graphics_core\resource\MaterialBuffer.h>

#include <graphics_core\renderer\Renderer.h>

#include <graphics_module\CommandBuffer.h>

#include <graphics_module\PipelineState.h>
#include <graphics_module\PipelineLayout.h>

#include <graphics_module\Shader.h>
#include <graphics_module\Sampler.h>
#include <graphics_module\Texture.h>

#include <common\AssetPath.h>


namespace graphics
{
	GridRenderPass::GridRenderPass()
		: RenderPass(TEXT("Grid Debug Pass"))
	{

	}

	GridRenderPass::~GridRenderPass()
	{
		delete m_pDebugMesh;
	}

	void GridRenderPass::Init()
	{
		m_pPipelineState = ResourceManager::GetInstance()->GetPipelineState(TEXT("Grid Debug"));

		m_pPipelineLayout = ResourceManager::GetInstance()->GetPipelineLayout(TEXT("Grid Debug"));

		m_pDebugBuffer = ResourceManager::GetInstance()->GetBuffer(TEXT("PerDebug"));

		m_pDebugMesh = new MeshObject(TEXT("Grid Object"));

		m_pDebugMesh->SetMeshBuffer(ResourceManager::GetInstance()->GetMeshBuffer(TEXT("00000000-0000-0000-0000-200000000000")));

		m_renderObjects.push_back(m_pDebugMesh);

		/*auto* _debugMaterialBuffer = ResourceManager::GetInstance()->CreateMaterialBuffer(TEXT("Grid"));

		_debugMaterialBuffer->SetPipelineID(TEXT("Grid Debug"));

		AddMaterialBuffer(_debugMaterialBuffer);*/

		//_debugMaterialBuffer->AddRenderObject(m_pDebugMesh);

		//m_pTransBuffer = ResourceManager::GetInstance()->GetBuffer(TEXT("PerObject"));
	}

	void GridRenderPass::Bind(CommandBuffer* command)
	{
		__super::Bind(command);
	}

	void GridRenderPass::BeginExcute(CommandBuffer* command, CameraBuffer* camBuffer)
	{
		__super::BeginExcute(command, camBuffer);

		struct Debug
		{
			// 격자 두께 설정
			float gridThickness = 0.02f;
			math::Vector3 _color = { 1.0f, 1.0f, 1.0f};
		};

		Debug _debug;

		command->UpdateBuffer(*m_pDebugBuffer, 0, &_debug, sizeof(Debug));

		Renderer::GetInstance()->RenderMesh(m_pDebugMesh);

		//command->Draw(4, 0);
	}

	void GridRenderPass::Excute(CommandBuffer* command)
	{
		__super::Excute(command);
	}

	void GridRenderPass::EndExcute(CommandBuffer* command)
	{
		__super::EndExcute(command);
	}
}