#include <graphics_core\resource\CameraBuffer.h>

#include <graphics_module\CommandBuffer.h>

namespace graphics
{
	CameraBuffer::CameraBuffer()
	{

	}

	CameraBuffer::~CameraBuffer()
	{

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
}