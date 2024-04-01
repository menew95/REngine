#include <graphics_core\ResourceManager.h>

#include <graphics_module\RenderSystem.h>

#include <graphics_core\RenderPass.h>
#include <graphics_core\resource\MeshBuffer.h>
#include <graphics_core\resource\LightBuffer.h>
#include <graphics_core\resource\CameraBuffer.h>
#include <graphics_core\resource\TextureBuffer.h>
#include <graphics_core\resource\MaterialBuffer.h>

#include <graphics_core\table\JsonTable.h>

#include <graphics_core\renderpass\SkyBoxRenderPass.h>
#include <graphics_core\renderpass\DeferredRenderPass.h>
#include <graphics_core\renderpass\GridRenderPass.h>

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
	
	void LoadGraphicsTable()
	{
		TableLoader::LoadTextureTable(ResourceManager::GetInstance());

		TableLoader::LoadSamplerTable(ResourceManager::GetInstance());

		TableLoader::LoadBufferTable(ResourceManager::GetInstance());

		TableLoader::LoadRenderTargetTable(ResourceManager::GetInstance(), {1280, 720});

		TableLoader::LoadPipelineLayoutTable(ResourceManager::GetInstance());

		TableLoader::LoadShaderTable(ResourceManager::GetInstance());

		TableLoader::LoadPipelineStateTable(ResourceManager::GetInstance());

		TableLoader::LoadRenderPassTable(ResourceManager::GetInstance());
	}

	void ResourceManager::Initialze(RenderSystem* renderSystem)
	{
		m_pRenderSystem = renderSystem;
	}

	void ResourceManager::LoadGraphicsResource()
	{
		LoadGraphicsTable();

		{
			auto _renderPass = new SkyBoxRenderPass();

			m_renderPassMap.insert(make_pair(TEXT("SkyBox Pass"), _renderPass));
		}

		{
			auto _renderPass = new DeferredRenderPass();

			m_renderPassMap.insert(make_pair(TEXT("Deferred Pass"), _renderPass));
		}

		{
			auto _renderPass = new GridRenderPass();

			m_renderPassMap.insert(make_pair(TEXT("Grid Pass"), _renderPass));
		}
	}

	void ResourceManager::InitRenderPass()
	{
		for (auto& [_uuid, _renderPass] : m_renderPassMap)
		{
			_renderPass->Init();
		}
	}

	MeshBuffer* ResourceManager::CreateMeshBuffer(uuid uuid)
	{
		auto* _buffer = new MeshBuffer();

		_buffer->m_uuid = uuid;

		m_meshBuffers.insert(make_pair(uuid, _buffer));

		return _buffer;
	}

	MaterialBuffer* ResourceManager::CreateMaterialBuffer(uuid uuid)
	{
		auto* _buffer = new MaterialBuffer();

		_buffer->m_uuid = uuid;

		m_materialBuffers.insert(make_pair(uuid, _buffer));

		return _buffer;
	}

	TextureBuffer* ResourceManager::CreateTextureBuffer(uuid uuid)
	{
		auto* _buffer = new TextureBuffer();

		_buffer->m_uuid = uuid;

		m_textureBuffers.insert(make_pair(uuid, _buffer));

		return _buffer;
	}

	CameraBuffer* ResourceManager::CreateCameraBuffer(uuid uuid)
	{
		auto* _buffer = new CameraBuffer();

		_buffer->m_uuid = uuid;

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

	MeshBuffer* graphics::ResourceManager::GetMeshBuffer(uuid uuid)
	{
		auto _iter = m_meshBuffers.find(uuid);

		if (_iter != m_meshBuffers.end())
			return _iter->second;

		return nullptr;
	}

	MaterialBuffer* graphics::ResourceManager::GetMaterialBuffer(uuid uuid)
	{
		auto _iter = m_materialBuffers.find(uuid);

		if (_iter != m_materialBuffers.end())
			return _iter->second;

		return nullptr;
	}

	TextureBuffer* graphics::ResourceManager::GetTextureBuffer(uuid uuid)
	{
		auto _iter = m_textureBuffers.find(uuid);

		if (_iter != m_textureBuffers.end())
			return _iter->second;

		return nullptr;
	}

	CameraBuffer* graphics::ResourceManager::GetCameraBuffer(uuid uuid)
	{
		auto _iter = m_cameraBuffers.find(uuid);

		if (_iter != m_cameraBuffers.end())
			return _iter->second;

		return nullptr;
	}

	LightBuffer* graphics::ResourceManager::GetLightBuffer(uuid uuid)
	{
		auto _iter = m_lightBuffers.find(uuid);

		if(_iter != m_lightBuffers.end())
			return _iter->second;

		return nullptr;
	}

	RenderPass* graphics::ResourceManager::GetRenderPass(uuid uuid)
	{
		auto _iter = m_renderPassMap.find(uuid);

		if (_iter != m_renderPassMap.end())
			return _iter->second;

		return nullptr;
	}

	RenderPass* ResourceManager::CreateRenderPass(uuid uuid, RenderPassDesc& desc)
	{
		return nullptr;
	}

	bool graphics::ResourceManager::ReleaseRenderPass(RenderPass* renderPass)
	{
		auto it = find_if(std::begin(m_renderPassMap), std::end(m_renderPassMap), [renderPass](pair<const uuid, RenderPass*>& p)
			{
				return p.second == renderPass;
			});

		if (it == std::end(m_renderPassMap))
			return false;

		m_renderPassMap.erase(it);
		return true;
	}

	Buffer* ResourceManager::CreateBuffer(uuid uuid, BufferDesc& desc, const void* init)
	{
		auto* _buffer = m_pRenderSystem->CreateBuffer(uuid, desc, init);

		m_bufferMap.insert(make_pair(uuid, _buffer));

		return _buffer;
	}

	Shader* ResourceManager::CreateShader(uuid uuid, ShaderDesc& desc)
	{
		auto* _shader = m_pRenderSystem->CreateShader(uuid, desc);

		m_shaderMap.insert(make_pair(uuid, _shader));

		return _shader;
	}

	Sampler* ResourceManager::CreateSampler(uuid uuid, SamplerDesc& desc)
	{
		auto _sampler = m_pRenderSystem->CreateSampler(uuid, desc);

		m_samplerMap.insert(make_pair(uuid, _sampler));

		return _sampler;
	}

	Texture* ResourceManager::CreateTexture(uuid uuid, TextureDesc& desc, const struct ImageDesc* image)
	{
		auto* _tex = m_pRenderSystem->CreateTexture(uuid, desc, image);

		m_textureMap.insert(make_pair(uuid, _tex));

		return _tex;
	}

	RenderTarget* ResourceManager::CreateRenderTarget(uuid uuid, RenderTargetDesc& desc)
	{
		auto * _rt = m_pRenderSystem->CreateRenderTarget(uuid, desc);

		m_renderTargetMap.insert(make_pair(uuid, _rt));

		return _rt;
	}

	ResourceView* ResourceManager::CreateResourceView(uuid uuid, ResourceViewDesc& desc)
	{
		auto * _rv = m_pRenderSystem->CreateResoureView(uuid, desc);

		m_resourceViewMap.insert(make_pair(uuid, _rv));

		return _rv;
	}

	PipelineState* ResourceManager::CreatePipelineState(uuid uuid, GraphicsPipelineDesc& desc)
	{
		auto* _state = m_pRenderSystem->CreatePipelineState(uuid, desc);

		m_pipelineStateMap.insert(make_pair(uuid, _state));

		return _state;
	}

	PipelineState* ResourceManager::CreatePipelineState(uuid uuid, ComputePipelineDesc& desc)
	{
		auto* _state = m_pRenderSystem->CreatePipelineState(uuid, desc);

		m_pipelineStateMap.insert(make_pair(uuid, _state));

		return _state;
	}

	PipelineLayout* ResourceManager::CreatePipelineLayout(uuid uuid, PipelineLayoutDesc& desc)
	{
		auto* _layout = m_pRenderSystem->CreatePipelineLayout(uuid, desc);

		m_pipelineLayoutMap.insert(make_pair(uuid, _layout));

		return _layout;
	}

	Buffer* graphics::ResourceManager::GetBuffer(uuid uuid)
	{
		auto _it = m_bufferMap.find(uuid);

		if (_it != m_bufferMap.end())
			return _it->second;

		return nullptr;
	}

	Shader* graphics::ResourceManager::GetShader(uuid uuid)
	{
		auto _it = m_shaderMap.find(uuid);

		if (_it != m_shaderMap.end())
			return _it->second;

		return nullptr;
	}

	Sampler* graphics::ResourceManager::GetSampler(uuid uuid)
	{
		auto _it = m_samplerMap.find(uuid);

		if (_it != m_samplerMap.end())
			return _it->second;

		return nullptr;
	}

	Texture* graphics::ResourceManager::GetTexture(uuid uuid)
	{
		auto _it = m_textureMap.find(uuid);

		if (_it != m_textureMap.end())
			return _it->second;

		return nullptr;
	}

	ResourceView* graphics::ResourceManager::GetResourceView(uuid uuid)
	{
		auto _it = m_resourceViewMap.find(uuid);

		if (_it != m_resourceViewMap.end())
			return _it->second;

		return nullptr;
	}

	RenderTarget* graphics::ResourceManager::GetRenderTarget(uuid uuid)
	{
		auto _it = m_renderTargetMap.find(uuid);

		if (_it != m_renderTargetMap.end())
			return _it->second;

		return nullptr;
	}

	PipelineState* graphics::ResourceManager::GetPipelineState(uuid uuid)
	{
		auto _it = m_pipelineStateMap.find(uuid);

		if(_it != m_pipelineStateMap.end())
			return _it->second;

		return nullptr;
	}

	PipelineLayout* graphics::ResourceManager::GetPipelineLayout(uuid uuid)
	{
		auto _it = m_pipelineLayoutMap.find(uuid);

		if (_it != m_pipelineLayoutMap.end())
			return _it->second;

		return nullptr;
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