#include <graphics_core\resource\CameraBuffer.h>

#include <graphics_module\CommandBuffer.h>

#include <graphics_module\Texture.h>
#include <graphics_module\RenderTarget.h>

#include <graphics_core\ResourceManager.h>

namespace graphics
{
	CameraBuffer::CameraBuffer()
	{

	}

	CameraBuffer::~CameraBuffer()
	{
		ResourceManager::GetInstance()->ReleaseRenderTarget(m_pRenderTarget);

		ResourceManager::GetInstance()->ReleaseTexture(m_pColorTexture);

		ResourceManager::GetInstance()->ReleaseTexture(m_pDepthTexture);
	}

	void CameraBuffer::Init()
	{
		TextureDesc _texDesc;
		
		_texDesc._extend = { 1280, 720, 0 };

		m_pColorTexture = ResourceManager::GetInstance()->CreateTexture(m_uuid + TEXT("_Color"), _texDesc);

		_texDesc._bindFlags = BindFlags::ShaderResource | BindFlags::DepthStencil;

		_texDesc._format = Format::R24G8_TYPELESS;

		m_pDepthTexture = ResourceManager::GetInstance()->CreateTexture(m_uuid + TEXT("_Depth"), _texDesc);

		RenderTargetDesc _rtDesc;

		_rtDesc._extend = { 1280, 720 };

		AttachmentDesc _attachDesc;

		_attachDesc._renderTargetType = RenderTargetType::RenderTarget;

		_attachDesc._resource = m_pColorTexture;

		_rtDesc._attachments.push_back(_attachDesc);

		_attachDesc._renderTargetType = RenderTargetType::DepthStencil;

		_attachDesc._resource = m_pDepthTexture;

		_rtDesc._attachments.push_back(_attachDesc);

		m_pRenderTarget = ResourceManager::GetInstance()->CreateRenderTarget(m_uuid, _rtDesc);
	}

	void CameraBuffer::OnResize(const Extent2D& resolution)
	{

	}

	void CameraBuffer::SetName(const char* name)
	{
		m_pRenderTarget->SetName(name);
	}

	void CameraBuffer::UpdateBuffer(CommandBuffer* command, Buffer* buffer)
	{
		command->UpdateBuffer(*buffer, 0, &m_cameraInfo, sizeof(CameraInfo));
	}

	void CameraBuffer::UpdateCascadeShadow(math::Vector3 directionalLightDir)
	{
		// todo : cascaded shadow info 버퍼에 사용될 소스 데이터 갱신
		m_cascadedInfo._cascadeEndClipSpace[0] = m_cameraInfo._near;
		m_cascadedInfo._cascadeEndClipSpace[1] = m_cameraInfo._near;
		m_cascadedInfo._cascadeEndClipSpace[2] = m_cameraInfo._near;
		m_cascadedInfo._cascadeEndClipSpace[3] = m_cameraInfo._near;
	}

	void* CameraBuffer::GetTextureID()
	{
		return m_pColorTexture->GetTextureID();
	}
}