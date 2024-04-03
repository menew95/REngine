#include <graphics_core\resource\LightBuffer.h>

#include <graphics_core\ResourceManager.h>

#include <graphics_module\RenderTarget.h>
#include <graphics_module\Texture.h>

namespace graphics
{
	math::Vector2 CalcResolution(LightType type, uint32 resolution);

	LightBuffer::LightBuffer()
	{

	}

	LightBuffer::~LightBuffer()
	{

	}

	void LightBuffer::SetName(const char* name)
	{

	}

	math::Vector2 CalcResolution(LightType type, uint32 resolution)
	{
		float _multiply = pow(0.5f, resolution);
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

		return math::Vector2(_resolution, _resolution) * _multiply;
	}

	void LightBuffer::CreateRenderTarget()
	{
		// 라이트의 종류에 따른 랜더 타겟 생성 필요
		// 스팟 라이트는 하나만
		// 포인트 라이트는 랜더 타겟을 6개의 밉슬라이스로 생성 필요

		RenderTargetDesc _renderTargetDesc;

		_renderTargetDesc._extend = CalcResolution((LightType)m_lightInfo._lightype, m_shadowInfo._resolution);

		AttachmentDesc _renderAttachDesc;

		_renderAttachDesc._renderTargetType = RenderTargetType::RenderTarget;

		_renderAttachDesc._mipLevel = 0;
		_renderAttachDesc._arrayLayer = m_lightInfo._lightype == static_cast<uint32>(LightType::Point) ? 6 : 1;

		_renderTargetDesc._attachments.push_back(_renderAttachDesc);

		AttachmentDesc _depthAttachDesc;

		_depthAttachDesc._renderTargetType = RenderTargetType::DepthStencil;

		_depthAttachDesc._mipLevel = 0;
		_depthAttachDesc._arrayLayer = m_lightInfo._lightype == static_cast<uint32>(LightType::Point) ? 6 : 1;

		_renderTargetDesc._attachments.push_back(_depthAttachDesc);
	}
}