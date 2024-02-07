#include <graphics_core\renderpass\SkyBoxRenderPass.h>

#include <graphics_core\object\MeshObject.h>

#include <graphics_core\ResourceManager.h>

#include <graphics_core\renderer\Renderer.h>

#include <graphics_module\PipelineState.h>
#include <graphics_module\PipelineLayout.h>

#include <graphics_module\Shader.h>
#include <graphics_module\Sampler.h>
#include <graphics_module\Texture.h>

#include <common\AssetPath.h>

namespace graphics
{
	SkyBoxRenderPass::SkyBoxRenderPass()
		: RenderPass(TEXT("SkyBox Pass"))
	{
		
	}

	SkyBoxRenderPass::~SkyBoxRenderPass()
	{

	}

	void SkyBoxRenderPass::Init()
	{
		return;
		m_pSkyBoxMesh = new MeshObject(TEXT("SkyBox Object"));

		m_pSkyBoxMesh->SetMeshBuffer(ResourceManager::GetInstance()->GetMeshBuffer(TEXT("00000000-0000-0000-0000-100000000000")));

		m_renderObjects.push_back(m_pSkyBoxMesh);

		ShaderDesc _vsDesc, _psDesc;

		_vsDesc._shaderType = ShaderType::Vertex;
		_vsDesc._sourceType = ShaderSourceType::HLSL;
		_vsDesc._filePath = g_assetPath + TEXT("bin\\shader\\VS_Mesh.hlsl");
		_vsDesc._entryPoint = "main";
		_vsDesc._profile = "vs_5_0";

		_psDesc._shaderType = ShaderType::Pixel;
		_psDesc._sourceType = ShaderSourceType::HLSL;
		_psDesc._filePath = g_assetPath + TEXT("bin\\shader\\PS_SkyBox.hlsl");
		_psDesc._entryPoint = "main";
		_psDesc._profile = "ps_5_0";

		Shader* _vs = ResourceManager::GetInstance()->CreateShader(TEXT("00000000-0000-0000-1000-000000000000"), _vsDesc);
		Shader* _ps = ResourceManager::GetInstance()->CreateShader(TEXT("00000000-0000-0000-2000-000000000000"), _psDesc);

		GraphicsPipelineDesc _lineDesc;

		_lineDesc._shaderProgram._vertexShader = ResourceManager::GetInstance()->GetShader(TEXT("00000000-0000-0000-1000-000000000000"));
		_lineDesc._shaderProgram._pixelShader = ResourceManager::GetInstance()->GetShader(TEXT("00000000-0000-0000-2000-000000000000"));

		_lineDesc._hasDSS = false;
		_lineDesc._hasBS = false;

		m_pPipelineState = ResourceManager::GetInstance()->CreatePipelineState(TEXT("SkyBox"), _lineDesc);

		SamplerDesc _samDesc;
		_samDesc._filter = Filter::MIN_MAG_MIP_LINEAR;

		Sampler* _sampler = ResourceManager::GetInstance()->CreateSampler(TEXT("WrapLinear"), _samDesc);

		Texture* _texture = ResourceManager::GetInstance()->GetTexture(TEXT(""));

		PipelineLayoutDesc _layoutDesc;

		_layoutDesc._bindings.push_back({ResourceType::Sampler, 0, StageFlags::PS, 0});
		_layoutDesc._bindings.push_back({ ResourceType::Texture, 0, StageFlags::PS, 0 });
		_layoutDesc._resources.push_back(_sampler);
		_layoutDesc._resources.push_back(_texture);

		m_pPipelineLayout = ResourceManager::GetInstance()->CreatePipelineLayout(TEXT("SkyBox"), _layoutDesc);
	}
	
	void SkyBoxRenderPass::Bind(CommandBuffer* command)
	{
		__super::Bind(command);
	}
	
	void SkyBoxRenderPass::BeginExcute(CommandBuffer* command)
	{
		__super::BeginExcute(command);
	}
	
	void SkyBoxRenderPass::Excute(CommandBuffer* command)
	{
		__super::Excute(command);
	}

	void SkyBoxRenderPass::EndExcute(CommandBuffer* command)
	{
		__super::EndExcute(command);
	}
}