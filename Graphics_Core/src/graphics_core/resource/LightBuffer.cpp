#include <graphics_core\resource\LightBuffer.h>

#include <graphics_core\ResourceManager.h>

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

	}

	void LightBuffer::SetName(const char* name)
	{

	}

	void LightBuffer::SetLightType(uint32 value)
	{
		m_lightInfo._lightype = value;
	}

	void LightBuffer::SetLightColor(const math::Color& value)
	{
		m_lightInfo._color = value.ToVector3();
	}

	void LightBuffer::SetIntensity(float value)
	{
		m_lightInfo._intensity = value;
	}

	void LightBuffer::SetRange(float value)
	{
		m_lightInfo._range = value;
	}

	void LightBuffer::SetSpotAnlge(float value)
	{
		m_lightInfo._spotAngle = value;
	}

	void LightBuffer::SetInnerSpotAngle(float value)
	{
		m_lightInfo._fallOffAngle = value;
	}

	void LightBuffer::SetResolution(uint32 value)
	{
		m_shadowInfo._resolution = value;

		ResourceManager::GetInstance()->ReleaseTexture(m_pTexture);
		ResourceManager::GetInstance()->ReleaseRenderTarget(m_pRenderTarget);

		CreateRenderTarget();
	}

	math::Vector2 CalcResolution(LightType type, uint32 resolution)
	{
		float _multiply[4] = { 0.125f, 0.25f, 0.5f, 1.0f };
		uint32 _resolution;

		switch (type)
		{
			case graphics::LightType::Spot:
				_resolution = 1024;
				break;
			case graphics::LightType::Directional:
				_resolution = 2048;
				break;
			case graphics::LightType::Point:
				_resolution = 512;
				break;
			default:
				break;
		}

		return math::Vector2(_resolution, _resolution) * _multiply[resolution];
	}

	void LightBuffer::CreateRenderTarget()
	{
		uuid _uuid = TEXT("Light Buffer");

		math::Vector2 _resolution = CalcResolution((LightType)m_lightInfo._lightype, m_shadowInfo._resolution);

		TextureDesc _texDesc;

		_texDesc._textureType = TextureType::Texture2D;
		_texDesc._miscFlags = (MiscFlags::FixedSamples | MiscFlags::GenerateMips);
		_texDesc._format = Format::R8G8B8A8_UNORM;
		_texDesc._extend = { static_cast<uint32>(_resolution.x), static_cast<uint32>(_resolution.y), 1 };
		_texDesc._arrayLayers = m_lightInfo._lightype == static_cast<uint32>(LightType::Point) ? 6 : 1;
		_texDesc._mipLevels = 0;
		_texDesc._samples = 1;
		
		m_pTexture = ResourceManager::GetInstance()->CreateTexture(_uuid + TEXT("_C_") + to_wstring(m_lightIdx), _texDesc);

		_texDesc._bindFlags = (BindFlags::ShaderResource | BindFlags::DepthStencil);
		_texDesc._format = Format::R32_TYPELESS;

		m_pDepthTexture = ResourceManager::GetInstance()->CreateTexture(_uuid + TEXT("_D_") + to_wstring(m_lightIdx), _texDesc);

		RenderTargetDesc _renderTargetDesc;

		_renderTargetDesc._extend = _resolution;

		AttachmentDesc _renderAttachDesc;

		_renderAttachDesc._renderTargetType = RenderTargetType::RenderTarget;

		_renderAttachDesc._mipLevel = 0;
		_renderAttachDesc._resource = m_pTexture;
		_renderAttachDesc._arrayLayer = m_lightInfo._lightype == static_cast<uint32>(LightType::Point) ? 6 : 1;
		_renderAttachDesc._arraySize = m_lightInfo._lightype == static_cast<uint32>(LightType::Point) ? 6 : 1;

		_renderTargetDesc._attachments.push_back(_renderAttachDesc);

		AttachmentDesc _depthAttachDesc;

		_depthAttachDesc._renderTargetType = RenderTargetType::DepthStencil;

		_depthAttachDesc._mipLevel = 0;
		_depthAttachDesc._resource = m_pDepthTexture;
		_depthAttachDesc._arrayLayer = m_lightInfo._lightype == static_cast<uint32>(LightType::Point) ? 6 : 1;
		_depthAttachDesc._arraySize = m_lightInfo._lightype == static_cast<uint32>(LightType::Point) ? 6 : 1;

		_renderTargetDesc._attachments.push_back(_depthAttachDesc);

		m_pRenderTarget = ResourceManager::GetInstance()->CreateRenderTarget(_uuid + to_wstring(m_lightIdx), _renderTargetDesc);
	}
}