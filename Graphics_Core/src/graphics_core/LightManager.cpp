#include <graphics_core\LightManager.h>

#include <graphics_core\ResourceManager.h>

#include <graphics_core\object\PerFrame.h>

#include <graphics_core\resource\TextureAtlas.h>
#include <graphics_core\resource\LightBuffer.h>

#include <graphics_module\Buffer.h>
#include <graphics_module\Texture.h>
#include <graphics_module\RenderTarget.h>
#include <graphics_module\CommandBuffer.h>


namespace graphics
{
	DEFINE_SINGLETON_CLASS(LightManager,
		{

		},
		{

		},
		{
			Instance.ReleaseAllResource();
 		});

	void graphics::LightManager::Initialize()
	{
		CreateResource();
	}

	LightBuffer* LightManager::CreateLightBuffer(uuid uuid)
	{
		auto _iter = m_lightBufferMap.find(uuid);

		// 이미 존재한 uuid가 들어왔음
		assert(_iter == m_lightBufferMap.end());

		auto* _lightBuffer = new LightBuffer();

		_lightBuffer->m_uuid = uuid;

		return m_lightBufferMap.insert(make_pair(uuid, _lightBuffer)).first->second;
	}

	bool LightManager::ReleaseLightBuffer(uuid uuid)
	{
		auto _iter = m_lightBufferMap.find(uuid);

		if (_iter != m_lightBufferMap.end())
		{
			delete _iter->second;

			m_lightBufferMap.erase(_iter);

			return true;
		}

		return false;
	}

	void graphics::LightManager::Excute(CommandBuffer* command)
	{
		UpdateLightBuffer(command);
	}

	void graphics::LightManager::GetPerFrame(PerFrame& perFrame)
	{
		perFrame._lightCnt = m_curLightCnt;

		perFrame._WorldSpaceLightPos0 = m_directionalLightPos;
		perFrame._LightColor0 = m_directionalLightColor;

		perFrame._shadowAtlasResolution = Vector2(SHADOWMAP_ATLAS_SIZE, SHADOWMAP_ATLAS_SIZE);
	}

	uint32 graphics::LightManager::GetShadowMapTextureIndex(const LightInfo& light)
	{

		return uint32();
	}

	bool graphics::LightManager::ReturnShadowMapSpace(const LightInfo& light)
	{
		return false;
	}

	bool graphics::LightManager::SetShadowMapSpace(LightBuffer* lightbuffer)
	{
		bool _result = true;

		size_t _count = 0;

		switch (lightbuffer->m_lightInfo._type)
		{
			case (uint32)LightType::Spot:
			{
				_count = 1;
				break;
			}
			case (uint32)LightType::Directional:
			{
				_count = 4;
				break;
			}
			case (uint32)LightType::Point:
			{
				_count = 6;
				break;
			}
			default:
			{
				assert(false);
				break;
			}
		}

		for (size_t i = 0; i < _count; i++)
		{
			// 만약 할당 도중에 실패 할경우 여태까지 할당한 텍스처를 다시 해제하고 FALSE 반환
			if (!m_shadowMapAtlas->GetTextureSpace(lightbuffer->m_uuid, lightbuffer->GetResolution(), lightbuffer->m_lightInfo._uv0[i], lightbuffer->m_lightInfo._uv1[i]))
			{
				for (size_t j = 0; j < i; j++)
				{
					assert(m_shadowMapAtlas->ReturnTextureSpace(lightbuffer->m_uuid));
				}

				_result = false;

				break;
			}
		}

		return _result;
	}

	bool graphics::LightManager::ReleaseShadowMapSpace(LightBuffer* lightbuffer)
	{
		bool _result = true;

		size_t _count = 0;

		switch (lightbuffer->m_lightInfo._type)
		{
			case (uint32)LightType::Spot:
			{
				_count = 1;
				break;
			}
			case (uint32)LightType::Directional:
			{
				_count = 4;
				break;
			}
			case (uint32)LightType::Point:
			{
				_count = 6;
				break;
			}
			default:
			{
				assert(false);
				break;
			}
		}

		for (size_t i = 0; i < _count; i++)
		{
			assert(m_shadowMapAtlas->ReturnTextureSpace(lightbuffer->m_uuid));
		}

		return _result;
	}

	void graphics::LightManager::CreateResource()
	{
		m_shadowMapAtlas = make_shared<TextureAtlas>(TEXT("Shadow Map Atlas"), Vector2{ SHADOWMAP_ATLAS_SIZE, SHADOWMAP_ATLAS_SIZE });

		TextureDesc _shadowMapDesc;

		_shadowMapDesc._extend = { SHADOWMAP_ATLAS_SIZE, SHADOWMAP_ATLAS_SIZE, 0 };
		_shadowMapDesc._bindFlags = BindFlags::ShaderResource | BindFlags::DepthStencil;
		_shadowMapDesc._format = Format::R32_TYPELESS;

		_shadowMapDesc._textureType = TextureType::Texture2D;

		m_shadowMapAtlasTexture = ResourceManager::GetInstance()->CreateTexture(TEXT("Shadow Map Texture"), _shadowMapDesc);
		
		RenderTargetDesc _shawdowMapRTDesc;

		_shawdowMapRTDesc._extend = { SHADOWMAP_ATLAS_SIZE, SHADOWMAP_ATLAS_SIZE, 0 };

		_shawdowMapRTDesc._attachments.push_back({ RenderTargetType::DepthStencil, m_shadowMapAtlasTexture, 0, 0, 0});

		m_shadowMapAtlasRT = ResourceManager::GetInstance()->CreateRenderTarget(TEXT("Shadow Map Depth"), _shawdowMapRTDesc);

		BufferDesc _desc;

		_desc._bindFlags = BindFlags::ShaderResource | BindFlags::UnorderedAccess;
		_desc._format = Format::UNKNOWN;
		_desc._stride = sizeof(LightInfo);
		_desc._size = sizeof(LightInfo) * 100;
		_desc._cpuAccessFlags = CPUAccessFlags::Write;
		_desc._miscFlags = MiscFlags::DynamicUsage;

		m_lightBuffer = ResourceManager::GetInstance()->CreateBuffer(TEXT("Light Buffer View"), _desc);

		/*TextureDesc _spotShadowMapDesc;

		_spotShadowMapDesc._extend = { 512, 512, 0 };
		_spotShadowMapDesc._bindFlags = BindFlags::ShaderResource | BindFlags::DepthStencil;
		_spotShadowMapDesc._format = Format::R32_TYPELESS;

		_spotShadowMapDesc._textureType = TextureType::Texture2DArray;
		_spotShadowMapDesc._arrayLayers = NUM_SPOTLIGHT;

		m_spotLightShadowMap = ResourceManager::GetInstance()->CreateTexture(TEXT("Shadow Map Texture"), _spotShadowMapDesc);

		TextureDesc _pointShadowMapDesc;

		_pointShadowMapDesc._extend = { 512, 512, 0 };
		_pointShadowMapDesc._bindFlags = BindFlags::ShaderResource | BindFlags::DepthStencil;
		_pointShadowMapDesc._format = Format::R32_TYPELESS;

		_pointShadowMapDesc._textureType = TextureType::Texture2DArray;
		_pointShadowMapDesc._arrayLayers = NUM_POINTLIGHT * 6;

		m_pointLightShadowMap = ResourceManager::GetInstance()->CreateTexture(TEXT("Shadow Map Texture"), _pointShadowMapDesc);

		TextureDesc _cascadedShadowMapDesc;

		_cascadedShadowMapDesc._extend = { 1024, 1024, 0 };
		_cascadedShadowMapDesc._bindFlags = BindFlags::ShaderResource | BindFlags::DepthStencil;
		_cascadedShadowMapDesc._format = Format::R32_TYPELESS;

		_cascadedShadowMapDesc._textureType = TextureType::Texture2DArray;
		_cascadedShadowMapDesc._arrayLayers = NUM_CASCADES;

		m_cascadedShadowMap = ResourceManager::GetInstance()->CreateTexture(TEXT("Shadow Map Texture"), _cascadedShadowMapDesc);*/
	}

	void LightManager::ReleaseAllResource()
	{
		for (auto& [_uuid, _buffer] : m_lightBufferMap)
		{
			assert(_buffer != nullptr);

			delete _buffer;
		}

		m_lightBufferMap.clear();

		ResourceManager::GetInstance()->ReleaseTexture(m_shadowMapAtlasTexture);
		ResourceManager::GetInstance()->ReleaseRenderTarget(m_shadowMapAtlasRT);
	}

	void graphics::LightManager::UpdateLightBuffer(CommandBuffer* command)
	{
		m_cascadedSet = false;

		uint32 _stride = sizeof(LightInfo);
		uint32 _dataSize = (uint32)m_lightBufferMap.size() * _stride;
		m_curLightCnt = 0;

		if(_dataSize == 0)
			return;

		char* _data = new char[_dataSize];

		size_t _offset = 0;

		bool _isDirectionalLight = false;

		for (auto& [_uuid, _buffer] : m_lightBufferMap)
		{
			if(!_buffer->GetEnable())
				continue;

			m_curLightCnt++;

			memcpy(_data + _offset, &_buffer->GetLightInfo(), _stride);

			_offset += _stride;

			if (!_isDirectionalLight && _buffer->GetLightInfo()._type == (uint32)LightType::Directional)
			{
				_isDirectionalLight = true;

				m_directionalLightPos = 
				{
					_buffer->GetLightInfo()._direction.x,
					_buffer->GetLightInfo()._direction.y,
					_buffer->GetLightInfo()._direction.z,
					0.0f
				};

				m_directionalLightColor = 
				{
					_buffer->GetLightInfo()._color.x,
					_buffer->GetLightInfo()._color.y,
					_buffer->GetLightInfo()._color.z,
					1.0f
				};
			}

			if (!m_cascadedSet && _buffer->m_lightInfo._type == (uint32)LightType::Directional)
			{
				m_cascadedSet = true;
				m_cascadedDir = _buffer->m_lightInfo._direction;
				m_cascadedLightBuffer = _buffer;
			}
		}

		command->UpdateBuffer(*m_lightBuffer, 0, _data, _dataSize);

		//offsetof(PerFrame, _lightCnt);
		//
		//auto* buffer = ResourceManager::GetInstance()->GetBuffer(TEXT("PerFrame"));
		//
		//command->UpdateBuffer(*buffer, offsetof(PerFrame, _lightCnt), &m_curLightCnt, sizeof(m_curLightCnt));
	}
}