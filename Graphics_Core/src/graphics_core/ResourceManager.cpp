#include <graphics_core\ResourceManager.h>

#include <graphics_module\RenderSystem.h>

#include <graphics_core\resource\MeshBuffer.h>
#include <graphics_core\resource\LightBuffer.h>
#include <graphics_core\resource\CameraBuffer.h>
#include <graphics_core\resource\TextureBuffer.h>
#include <graphics_core\resource\MaterialBuffer.h>

template <typename T>
bool RemoveFromUnorderedMap(std::unordered_map<uuid, T*>& cont, uuid uuid)
{
	static_assert(is_base_of<graphics::ResourceBuffer, T>::value, "class doesn't derive from the base");
	auto it = cont.find(uuid);
	if (it == std::end(cont))
		return false;

	cont.erase(it);
	return true;
}

template <typename T>
bool RemoveFromUnorderedMap(std::unordered_map<uuid, T*>& cont, const T* entry)
{
	static_assert(is_base_of<graphics::RenderSystemObject, T>::value, "class doesn't derive from the base");
	auto it = find_if(std::begin(cont), std::end(cont), [entry](pair<const uuid, T*>& p)
		{
			return p.second == entry;
		});

	if (it == std::end(cont))
		return false;

	cont.erase(it);
	return true;
}

namespace graphics
{
	DEFINE_SINGLETON_CLASS(ResourceManager,
		{
			// constructor
		},
		{
			// destructor
		},
		{
			// release
			ResourceManager::GetInstance()->ReleaseAll();
		});
	
	void ResourceManager::Initialze(RenderSystem* renderSystem)
	{
		m_pRenderSystem = renderSystem;
	}

	MeshBuffer* ResourceManager::CreateMeshBuffer(uuid uuid)
	{
		auto* _buffer = new MeshBuffer();

		m_meshBuffers.insert(make_pair(uuid, _buffer));

		return _buffer;
	}

	MaterialBuffer* ResourceManager::CreateMaterialBuffer(uuid uuid)
	{
		auto* _buffer = new MaterialBuffer();

		m_materialBuffers.insert(make_pair(uuid, _buffer));

		return _buffer;
	}

	TextureBuffer* ResourceManager::CreateTextureBuffer(uuid uuid)
	{
		auto* _buffer = new TextureBuffer();

		m_textureBuffers.insert(make_pair(uuid, _buffer));

		return _buffer;
	}

	CameraBuffer* ResourceManager::CreateCameraBuffer(uuid uuid)
	{
		auto* _buffer = new CameraBuffer();

		m_cameraBuffers.insert(make_pair(uuid, _buffer));

		return _buffer;
	}

	LightBuffer* ResourceManager::CreateLightBuffer(uuid uuid)
	{
		auto* _buffer = new LightBuffer();

		m_lightBuffers.insert(make_pair(uuid, _buffer));

		return _buffer;
	}

	bool ResourceManager::RelaseMeshBuffer(uuid uuid)
	{
		return RemoveFromUnorderedMap(m_meshBuffers, uuid);
	}

	bool ResourceManager::RelaseMaterialBuffer(uuid uuid)
	{
		return RemoveFromUnorderedMap(m_materialBuffers, uuid);
	}

	bool ResourceManager::RelaseTextureBuffer(uuid uuid)
	{
		return RemoveFromUnorderedMap(m_textureBuffers, uuid);
	}

	bool ResourceManager::RelaseCameraBuffer(uuid uuid)
	{
		return RemoveFromUnorderedMap(m_cameraBuffers, uuid);
	}

	bool ResourceManager::RelaseLightBuffer(uuid uuid)
	{
		return RemoveFromUnorderedMap(m_lightBuffers, uuid);
	}

	RenderPass* ResourceManager::CreateRenderPass(uuid uuid, RenderPassDesc& desc)
	{
		return nullptr;
	}

	Buffer* ResourceManager::CreateBuffer(uuid uuid, BufferDesc& desc, const void* init)
	{
		auto* _buffer = m_pRenderSystem->CreateBuffer(uuid, desc, init);

		return _buffer;
	}

	Shader* ResourceManager::CreateShader(uuid uuid, ShaderDesc& desc)
	{
		auto* _shader = m_pRenderSystem->CreateShader(uuid, desc);

		return _shader;
	}

	Sampler* ResourceManager::CreateSampler(uuid uuid, SamplerDesc& desc)
	{
		auto _sampler = m_pRenderSystem->CreateSampler(uuid, desc);

		return _sampler;
	}

	Texture* ResourceManager::CreateTexture(uuid uuid, TextureDesc& desc, const struct ImageDesc* image)
	{
		auto* _tex = m_pRenderSystem->CreateTexture(uuid, desc, image);

		return _tex;
	}

	RenderTarget* ResourceManager::CreateRenderTarget(uuid uuid, RenderTargetDesc& desc)
	{
		auto * _rt = m_pRenderSystem->CreateRenderTarget(uuid, desc);

		return _rt;
	}

	ResourceView* ResourceManager::CreateResourceView(uuid uuid, ResourceViewDesc& desc)
	{
		auto * _rv = m_pRenderSystem->CreateResoureView(uuid, desc);

		return _rv;
	}

	PipelineState* ResourceManager::CreatePipelineState(uuid uuid, GraphicsPipelineDesc& desc)
	{
		auto* _state = m_pRenderSystem->CreatePipelineState(uuid, desc);

		return _state;
	}

	PipelineState* ResourceManager::CreatePipelineState(uuid uuid, ComputePipelineDesc& desc)
	{
		auto* _state = m_pRenderSystem->CreatePipelineState(uuid, desc);

		return _state;
	}

	PipelineLayout* ResourceManager::CreatePipelineLayout(uuid uuid, PipelineLayoutDesc& desc)
	{
		auto* _layout = m_pRenderSystem->CreatePipelineLayout(uuid, desc);

		return _layout;
	}

	bool ResourceManager::ReleaseBuffer(Buffer* buffer)
	{
		m_pRenderSystem->Release(*buffer);

		return RemoveFromUnorderedMap(m_bufferMap, buffer);
	}

	bool ResourceManager::ReleaseShader(Shader* shader)
	{
		m_pRenderSystem->Release(*shader);

		return RemoveFromUnorderedMap(m_shaderMap, shader);
	}

	bool ResourceManager::ReleaseSampler(Sampler* sampler)
	{
		m_pRenderSystem->Release(*sampler);

		return RemoveFromUnorderedMap(m_samplerMap, sampler);
	}

	bool ResourceManager::ReleaseTexture(Texture* texture)
	{
		m_pRenderSystem->Release(*texture);

		return RemoveFromUnorderedMap(m_textureMap, texture);
	}

	bool ResourceManager::ReleaseRenderTarget(RenderTarget* renderTarget)
	{
		m_pRenderSystem->Release(*renderTarget);

		return RemoveFromUnorderedMap(m_renderTargetMap, renderTarget);
	}

	bool ResourceManager::ReleaseResourceView(ResourceView* resourceView)
	{
		m_pRenderSystem->Release(*resourceView);

		return RemoveFromUnorderedMap(m_resourceViewMap, resourceView);
	}

	bool ResourceManager::ReleasePipelineState(PipelineState* state)
	{
		m_pRenderSystem->Release(*state);

		return RemoveFromUnorderedMap(m_pipelineStateMap, state);
	}

	bool ResourceManager::ReleasePipelineLayout(PipelineLayout* layout)
	{
		m_pRenderSystem->Release(*layout);

		return RemoveFromUnorderedMap(m_pipelineLayoutMap, layout);
	}

	template <typename T>
	void ResourceManager::ClearFromUnorderedMap(std::unordered_map<uuid, T*>& cont)
	{
		for (auto& p : cont)
		{
			if constexpr (is_base_of<graphics::RenderSystemObject, T>::value)  m_pRenderSystem->Release(*p.second);
			else SAFE_DELETE(p.second);
		}
	}

	void ResourceManager::ReleaseAll()
	{
		ClearFromUnorderedMap(m_meshBuffers);
		ClearFromUnorderedMap(m_lightBuffers);
		ClearFromUnorderedMap(m_cameraBuffers);
		ClearFromUnorderedMap(m_textureBuffers);
		ClearFromUnorderedMap(m_materialBuffers);

		//ClearFromUnorderedMap(m_renderPassMap);

		ClearFromUnorderedMap(m_shaderMap);
		ClearFromUnorderedMap(m_bufferMap);
		ClearFromUnorderedMap(m_samplerMap);
		ClearFromUnorderedMap(m_textureMap);
		ClearFromUnorderedMap(m_resourceViewMap);
		ClearFromUnorderedMap(m_renderTargetMap);
		ClearFromUnorderedMap(m_pipelineStateMap);
		ClearFromUnorderedMap(m_pipelineLayoutMap);
	}
}