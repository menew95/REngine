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
		float cascadeOffset[4]{ 0.01f, 0.18f, 0.6f, 1.0f };
		
		m_cascadedInfo._cascadeOffset[0].x = m_cameraInfo._near;
		m_cascadedInfo._cascadeOffset[1].x = lerp(m_cameraInfo._near, m_cameraInfo._far, cascadeOffset[0]);
		m_cascadedInfo._cascadeOffset[2].x = lerp(m_cameraInfo._near, m_cameraInfo._far, cascadeOffset[1]);
		m_cascadedInfo._cascadeOffset[3].x = lerp(m_cameraInfo._near, m_cameraInfo._far, cascadeOffset[2]);
		m_cascadedInfo._cascadeOffset[4].x = m_cameraInfo._far;

		// 카메라의 view inverse
		math::Matrix CamInv = m_cameraInfo._view.Invert();

		float tanHalfHFOV = std::tanf((m_cameraInfo._fov / 2.0f));
		float tanHalfVFOV = std::tanf(((m_cameraInfo._fov * m_cameraInfo._aspectRatio) / 2.0f));

		for (uint i = 0; i < NUM_CASCADES; i++) {
			// offset를 기준으로 near, far평면의 half width와 height를 구함
			float xn = m_cascadedInfo._cascadeOffset[i].x * tanHalfHFOV;
			float xf = m_cascadedInfo._cascadeOffset[i + 1].x * tanHalfHFOV;
			float yn = m_cascadedInfo._cascadeOffset[i].x * tanHalfVFOV;
			float yf = m_cascadedInfo._cascadeOffset[i + 1].x * tanHalfVFOV;

			// 카메라 공간의 offset를 기준으로 분할된 프러스텀의 각 코너를 만듬
			math::Vector4 frustumCorners[NUM_FRUSTUM_CORNERS] = {
				// near face
				math::Vector4(xn, yn, m_cascadedInfo._cascadeOffset[i].x, 1.0),
				math::Vector4(-xn, yn, m_cascadedInfo._cascadeOffset[i].x, 1.0),
				math::Vector4(xn, -yn, m_cascadedInfo._cascadeOffset[i].x, 1.0),
				math::Vector4(-xn, -yn, m_cascadedInfo._cascadeOffset[i].x, 1.0),

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


			Vector4 _cornerCenter = Vector4::Zero;

			for (uint j = 0; j < NUM_FRUSTUM_CORNERS; j++)
			{
				// 각 코너를 월드 좌표로 옮김
				frustumCornersL[j] = math::Vector4::Transform(frustumCorners[j], CamInv);
				_cornerCenter += frustumCornersL[j];
			}

			// 모든 코너들의 중점을 구함
			_cornerCenter *= (1.f / 8.f);

			m_slices[i]._frustumCenter.x = _cornerCenter.x;
			m_slices[i]._frustumCenter.y = _cornerCenter.y;
			m_slices[i]._frustumCenter.z = _cornerCenter.z;

			//라이트의 위치 조정(무한대로 측정을 하고싶지만 일단 그럴 수는 없기에 200를 곱했음)
			Vector3 _lightPos = _cornerCenter - directionalLightDir * 50.f;

			// 조정한 라이트의 위치를 가지고 ligth view matrix를 만든다.
			math::Matrix LightM = math::Matrix::CreateLookAt(_lightPos, m_slices[i]._frustumCenter, math::Vector3(0.0f, 1.0f, 0.0f));

			for (uint j = 0; j < NUM_FRUSTUM_CORNERS; j++)
			{
				// 각 코너를 라이트 공간으로 옮김
				frustumCornersL[j] = math::Vector4::Transform(frustumCornersL[j], LightM);


				minX = min(minX, frustumCornersL[j].x);
				maxX = max(maxX, frustumCornersL[j].x);
				minY = min(minY, frustumCornersL[j].y);
				maxY = max(maxY, frustumCornersL[j].y);
				minZ = min(minZ, frustumCornersL[j].z);
				maxZ = max(maxZ, frustumCornersL[j].z);

			}

			m_slices[i]._width = maxX - minX;
			m_slices[i]._height = maxY - minY;
			m_slices[i]._depth = maxZ - minZ;

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