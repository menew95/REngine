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

	}

	void CameraBuffer::Init()
	{
		TextureDesc _texDesc;
		
		_texDesc._extend = { 1280, 720, 0 };

		m_pTexture = ResourceManager::GetInstance()->CreateTexture(m_uuid, _texDesc);

		AttachmentDesc _attachDesc;

		_attachDesc._renderTargetType = RenderTargetType::RenderTarget;

		_attachDesc._resource = m_pTexture;

		RenderTargetDesc _rtDesc;

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
		return m_pTexture->GetTextureID();
	}
}