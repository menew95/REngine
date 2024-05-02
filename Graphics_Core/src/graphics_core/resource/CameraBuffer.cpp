#include <graphics_core\resource\CameraBuffer.h>

#include <graphics_module\CommandBuffer.h>

#include <graphics_module\Texture.h>
#include <graphics_module\RenderTarget.h>

#include <graphics_core\ResourceManager.h>

#include <graphics_core\LightManager.h>

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
		m_cascadedInfo._cascadeOffset[0].x = m_cameraInfo._near;
		m_cascadedInfo._cascadeOffset[1].x = 10.0f,
		m_cascadedInfo._cascadeOffset[2].x = 20.0f,
		m_cascadedInfo._cascadeOffset[3].x = 40.0f,
		m_cascadedInfo._cascadeOffset[4].x = m_cameraInfo._far;

		// Get the inverse of the view transform
		math::Matrix CamInv = m_cameraInfo._view;

		// Get the light space transform
		math::Matrix LightM = math::Matrix::CreateLookAt(math::Vector3(0.0f, 0.0f, 0.0f), directionalLightDir, math::Vector3(0.0f, 1.0f, 0.0f));

		float tanHalfHFOV = std::tanf((m_cameraInfo._fov / 2.0f));
		float tanHalfVFOV = std::tanf(((m_cameraInfo._fov * m_cameraInfo._aspectRatio) / 2.0f));

		for (uint i = 0; i < NUM_CASCADES; i++) {
			float xn = m_cascadedInfo._cascadeOffset[i].x * tanHalfHFOV;
			float xf = m_cascadedInfo._cascadeOffset[i + 1].x * tanHalfHFOV;
			float yn = m_cascadedInfo._cascadeOffset[i].x * tanHalfVFOV;
			float yf = m_cascadedInfo._cascadeOffset[i + 1].x * tanHalfVFOV;

			math::Vector4 frustumCorners[NUM_FRUSTUM_CORNERS] = {
				// near face
				math::Vector4(xn, yn, m_cascadedInfo._cascadeOffset[i].x, 0.0),
				math::Vector4(-xn, yn, m_cascadedInfo._cascadeOffset[i].x, 0.0),
				math::Vector4(xn, -yn, m_cascadedInfo._cascadeOffset[i].x, 0.0),
				math::Vector4(-xn, -yn, m_cascadedInfo._cascadeOffset[i].x, 0.0),

				// far face
				math::Vector4(xf, yf, m_cascadedInfo._cascadeOffset[i + 1].x, 1.0),
				math::Vector4(-xf, yf, m_cascadedInfo._cascadeOffset[i + 1].x, 1.0),
				math::Vector4(xf, -yf, m_cascadedInfo._cascadeOffset[i + 1].x, 1.0),
				math::Vector4(-xf, -yf, m_cascadedInfo._cascadeOffset[i + 1].x, 1.0)
			};

			math::Vector4 frustumCornersL[NUM_FRUSTUM_CORNERS];

			float minX = std::numeric_limits<float>::max();
			float maxX = std::numeric_limits<float>::min();
			float minY = std::numeric_limits<float>::max();
			float maxY = std::numeric_limits<float>::min();
			float minZ = std::numeric_limits<float>::max();
			float maxZ = std::numeric_limits<float>::min();

			for (uint j = 0; j < NUM_FRUSTUM_CORNERS; j++) {

				// Transform the frustum coordinate from view to world space
				math::Vector4 vW = math::Vector4::Transform(frustumCorners[j], CamInv);

				// Transform the frustum coordinate from world to light space
				frustumCornersL[j] = math::Vector4::Transform(vW, LightM);

				minX = std::min(minX, frustumCornersL[j].x);
				maxX = std::max(maxX, frustumCornersL[j].x);
				minY = std::min(minY, frustumCornersL[j].y);
				maxY = std::max(maxY, frustumCornersL[j].y);
				minZ = std::min(minZ, frustumCornersL[j].z);
				maxZ = std::max(maxZ, frustumCornersL[j].z);
			}

			m_cascadedInfo._lightTransform[i] = LightM * math::Matrix::CreateOrthographicOffCenter(
				minX,
				maxX,
				minY,
				maxY,
				minZ,
				maxZ
			);
		}
	}

	void* CameraBuffer::GetTextureID()
	{
		return m_pColorTexture->GetTextureID();
	}
}