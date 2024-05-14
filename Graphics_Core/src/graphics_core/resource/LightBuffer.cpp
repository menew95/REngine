#include <graphics_core\resource\LightBuffer.h>

#include <graphics_core\ResourceManager.h>

#include <graphics_core\LightManager.h>

#include <graphics_module\RenderTarget.h>
#include <graphics_module\Texture.h>

namespace graphics
{
	math::Vector2 CalcResolution(LightType type, uint32 resolution);

	static uint32 s_lightIdx = 0;

	LightBuffer::LightBuffer()
	{
		m_lightIdx = s_lightIdx++;
	}

	LightBuffer::~LightBuffer()
	{
		if (m_isSetShadowMapSpace)
		{
			assert(LightManager::GetInstance()->ReleaseShadowMapSpace(this));
			m_isSetShadowMapSpace = false;
		}

		ReleaseShadowMap();
	}

	void LightBuffer::SetName(const char* name)
	{

	}

	void LightBuffer::Initialize()
	{
		m_isSetShadowMapSpace = LightManager::GetInstance()->SetShadowMapSpace(this);

		CreateShadowMap();
	}

	void LightBuffer::SetLightType(uint32 value)
	{
		if(m_lightInfo._type == value)
			return; 

		if (m_isSetShadowMapSpace)
		{
			assert(LightManager::GetInstance()->ReleaseShadowMapSpace(this));
			m_isSetShadowMapSpace = false;
		}

		m_lightInfo._type = value;

		m_isSetShadowMapSpace = LightManager::GetInstance()->SetShadowMapSpace(this);

		m_isDirty = true;

		ReleaseShadowMap();

		CreateShadowMap();
	}

	void LightBuffer::SetDirection(const Vector3& value)
	{
		if(m_lightInfo._direction == value)
			return;

		m_lightInfo._direction = value;
		
		m_isDirty = true;
	}

	void LightBuffer::SetPosition(const Vector3& value)
	{
		if (m_lightInfo._position == value)
			return;

		m_lightInfo._position = value;
		
		m_isDirty = true;
	}

	void LightBuffer::SetRange(float value)
	{
		if (m_lightInfo._attenuationRadius == value)
			return;

		m_lightInfo._attenuationRadius = value;
		
		m_isDirty = true;
	}

	void LightBuffer::SetLightColor(const math::Vector3& value)
	{
		if (m_lightInfo._color == value)
			return;

		m_lightInfo._color = value;
		
		m_isDirty = true;
	}

	void LightBuffer::SetIntensity(float value)
	{
		if (m_lightInfo._intensity == value)
			return;

		m_lightInfo._intensity = value;
		
		m_isDirty = true;
	}

	void LightBuffer::SetSpotAnlge(float value)
	{
		if (m_lightInfo._angle == value)
			return;

		m_lightInfo._angle = value;
		
		m_isDirty = true;
	}

	void LightBuffer::SetInnerSpotAngle(float value)
	{
		if (m_lightInfo._innerAngle == value)
			return;

		m_lightInfo._innerAngle = value; 
		
		m_isDirty = true;
	}

	void LightBuffer::SetFallOffExponential(float value)
	{
		if(m_lightInfo._fallOffExponential ==  value)
			return;

		m_lightInfo._fallOffExponential = value; 
		
		m_isDirty = true;
	}

	void LightBuffer::SetWidth(float value)
	{
		if (m_lightInfo._width == value)
			return;

		m_lightInfo._width = value; 
		
		m_isDirty = true;
	}

	void LightBuffer::SetUp(const Vector3& value)
	{
		if (m_lightInfo._up == value)
			return;

		m_lightInfo._up = value; 
		
		m_isDirty = true;
	}

	void LightBuffer::SetHeight(float value)
	{
		if (m_lightInfo._height == value)
			return;

		m_lightInfo._height = value;
		
		m_isDirty = true;
	}

	void LightBuffer::SetResolution(uint32 value)
	{
		if (m_shadowInfo._resolution == value)
			return;

		if (m_isSetShadowMapSpace)
		{
			assert(LightManager::GetInstance()->ReleaseShadowMapSpace(this));
			m_isSetShadowMapSpace = false;
		}

		m_shadowInfo._resolution = value;

		m_isSetShadowMapSpace = LightManager::GetInstance()->SetShadowMapSpace(this);

		m_isDirty = true;

		ReleaseShadowMap();

		CreateShadowMap();
	}

	void LightBuffer::SetWorld(Matrix& world)
	{
		Vector3 _s, _t;
		Quaternion _r;

		if (!world.Decompose(_s, _r, _t))
			assert(false);

		m_lightInfo._position = _t;
		
		m_lightInfo._direction = world.Forward();

		m_lightInfo._up = world.Up();

		m_isDirty = true;
	}

	math::Vector2 CalcResolution(LightType type, uint32 resolution)
	{
		float _multiply[4] = { 0.125f, 0.25f, 0.5f, 1.0f };
		float _resolution = 0.f;

		switch (type)
		{
			case graphics::LightType::Spot:
				_resolution = 1024.f;
				break;
			case graphics::LightType::Directional:
				_resolution = 2048.f;
				break;
			case graphics::LightType::Point:
				_resolution = 512.f;
				break;
			default:
				break;
		}

		return math::Vector2(_resolution, _resolution) * _multiply[resolution];
	}

	math::Vector2 LightBuffer::GetResolution()
	{
		return CalcResolution((LightType)m_lightInfo._type, m_shadowInfo._resolution);
	}

	void LightBuffer::CreateShadowMap()
	{
		uuid _uuid = TEXT("Light Buffer");

		math::Vector2 _resolution = CalcResolution((LightType)m_lightInfo._type, m_shadowInfo._resolution);

		uint32 _arrayCnt = 0;
		TextureType _textureType = TextureType::Texture2D;
		switch (m_lightInfo._type)
		{
			case (uint32)LightType::Spot:
			{
				_arrayCnt = 1;
				break;
			}
			case (uint32)LightType::Directional:
			{
				_arrayCnt = NUM_CASCADES;
				_textureType = TextureType::Texture2DArray;
				break;
			}
			case (uint32)LightType::Point:
			{
				_arrayCnt = 6;
				_textureType = TextureType::Texture2DArray;
				break;
			}
		}

		TextureDesc _texDesc;

		_texDesc._textureType = _textureType;
		_texDesc._miscFlags = (MiscFlags::FixedSamples | MiscFlags::GenerateMips);
		_texDesc._format = Format::R8G8B8A8_UNORM;
		_texDesc._extend = { static_cast<uint32>(_resolution.x), static_cast<uint32>(_resolution.y), 1 };
		_texDesc._arrayLayers = _arrayCnt;
		_texDesc._mipLevels = 0;
		_texDesc._samples = 1;

		//m_pTexture = ResourceManager::GetInstance()->CreateTexture(_uuid + TEXT("_C_") + to_wstring(m_lightIdx), _texDesc);

		_texDesc._bindFlags = (BindFlags::ShaderResource | BindFlags::DepthStencil);
		_texDesc._format = Format::R32_TYPELESS;

		m_pDepthTexture = ResourceManager::GetInstance()->CreateTexture(_uuid + TEXT("_D_") + to_wstring(m_lightIdx), _texDesc);
		
		RenderTargetDesc _renderTargetDesc;

		_renderTargetDesc._extend = _resolution;

		/*AttachmentDesc _renderAttachDesc;

		_renderAttachDesc._renderTargetType = RenderTargetType::RenderTarget;

		_renderAttachDesc._mipLevel = 0;
		_renderAttachDesc._resource = m_pTexture;
		_renderAttachDesc._arrayLayer = 0;
		_renderAttachDesc._arraySize = _arrayCnt;

		_renderTargetDesc._attachments.push_back(_renderAttachDesc);*/

		AttachmentDesc _depthAttachDesc;

		_depthAttachDesc._renderTargetType = RenderTargetType::DepthStencil;

		_depthAttachDesc._mipLevel = 0;
		_depthAttachDesc._resource = m_pDepthTexture;
		_depthAttachDesc._arrayLayer = 0;
		_depthAttachDesc._arraySize = _arrayCnt;

		_renderTargetDesc._attachments.push_back(_depthAttachDesc);

		m_pRenderTarget = ResourceManager::GetInstance()->CreateRenderTarget(_uuid + to_wstring(m_lightIdx), _renderTargetDesc);
	}

	void LightBuffer::ReleaseShadowMap()
	{
		if (m_pRenderTarget) ResourceManager::GetInstance()->ReleaseRenderTarget(m_pRenderTarget);
		if (m_pTexture) ResourceManager::GetInstance()->ReleaseTexture(m_pTexture);
		if (m_pDepthTexture) ResourceManager::GetInstance()->ReleaseTexture(m_pDepthTexture);

		m_pRenderTarget = nullptr;
		m_pTexture = nullptr;
		m_pDepthTexture = nullptr;
	}

	void LightBuffer::UpdateLightTransform()
	{
		if (m_lightInfo._type == (uint32)LightType::Spot)
		{
			m_shadowInfo._view[0] = Matrix::CreateLookAt(
				m_lightInfo._position,
				m_lightInfo._position + m_lightInfo._direction,
				m_lightInfo._up
			);

			m_shadowInfo._proj = Matrix::CreatePerspectiveFieldOfView(
				m_lightInfo._angle * 2.f,
				1.0f,
				0.1f,
				m_lightInfo._attenuationRadius
			);

			m_lightInfo._shadowMatrix[0] = m_shadowInfo._view[0] * m_shadowInfo._proj;
		}
		else if (m_lightInfo._type == (uint32)LightType::Point)
		{
			//+x
			m_shadowInfo._view[0] = Matrix::CreateLookAt(m_lightInfo._position, m_lightInfo._position + Vector3::Right, Vector3::Up);
			//-x
			m_shadowInfo._view[1] = Matrix::CreateLookAt(m_lightInfo._position, m_lightInfo._position + Vector3::Left, Vector3::Up);
			//+y
			m_shadowInfo._view[2] = Matrix::CreateLookAt(m_lightInfo._position, m_lightInfo._position + Vector3::Up, Vector3::Backward);
			//-y
			m_shadowInfo._view[3] = Matrix::CreateLookAt(m_lightInfo._position, m_lightInfo._position + Vector3::Down, Vector3::Forward);
			//+z
			m_shadowInfo._view[4] = Matrix::CreateLookAt(m_lightInfo._position, m_lightInfo._position + Vector3::Forward, Vector3::Up);
			//-z
			m_shadowInfo._view[5] = Matrix::CreateLookAt(m_lightInfo._position, m_lightInfo._position + Vector3::Backward, Vector3::Up);

			m_shadowInfo._proj = Matrix::CreatePerspectiveFieldOfView(
				90.f * math::Deg2Rad,
				1.0f,
				0.1f,
				m_lightInfo._attenuationRadius
			);

			m_lightInfo._shadowMatrix[0] = m_shadowInfo._view[0] * m_shadowInfo._proj;
			m_lightInfo._shadowMatrix[1] = m_shadowInfo._view[1] * m_shadowInfo._proj;
			m_lightInfo._shadowMatrix[2] = m_shadowInfo._view[2] * m_shadowInfo._proj;
			m_lightInfo._shadowMatrix[3] = m_shadowInfo._view[3] * m_shadowInfo._proj;
			m_lightInfo._shadowMatrix[4] = m_shadowInfo._view[4] * m_shadowInfo._proj;
			m_lightInfo._shadowMatrix[5] = m_shadowInfo._view[5] * m_shadowInfo._proj;
		}
	}
}