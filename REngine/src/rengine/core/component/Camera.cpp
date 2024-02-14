#include <rengine\core\component\Camera.h>
#include <rengine\core\component\Transform.h>

#include <rengine\core\object\GameObject.h>
#include <rengine\core\scene\Scene.h>

#include <graphics_core\GraphicsEngine.h>
#include <graphics_core\ResourceManager.h>

#include <graphics_core\resource\CameraBuffer.h>

#include <rttr\registration.h>

RTTR_REGISTRATION
{
	rttr::registration::class_<rengine::Camera>("Camera")
	//.constructor<std::shared_ptr<rengine::GameObject>&>()
	.constructor</*std::shared_ptr<rengine::GameObject>&, */uuid>()
	.property("Near", &rengine::Camera::GetNear, &rengine::Camera::SetNear)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT)
	)
	.property("Far", &rengine::Camera::GetFar, &rengine::Camera::SetFar)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT)
	)
	.property("Field of View", &rengine::Camera::GetFOV, &rengine::Camera::SetFOV)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT)
	)
	.property("Aspect Ratio", &rengine::Camera::GetAspect, &rengine::Camera::SetAspect)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT)
	)
	;
}

namespace rengine
{
std::weak_ptr<Camera> Camera::m_MainCamera;

	/*Camera::Camera(std::shared_ptr<GameObject>& gameObj)
		: Component(gameObj)
	{
		SetName(TEXT("Camera"));
	}*/

	Camera::Camera(/*std::shared_ptr<GameObject>& gameObj, */uuid uuid)
		: Component(/*gameObj, */uuid, TEXT("Camera"))
	{

	}

	Camera::~Camera()
	{

	}

	void Camera::Awake()
	{
		if (m_MainCamera.lock() == nullptr)
		{
			m_MainCamera = shared_from_this();
		}

		m_pCameraBuffer = graphics::ResourceManager::GetInstance()->CreateCameraBuffer(GetUUID());

		m_pCameraBuffer->Init();

		m_pCameraBuffer->SetName(GetNameStr().c_str());
	}

	void Camera::Update()
	{
		graphics::CameraInfo _info;

		auto _trans = GetTransform().lock();

		if(_trans == nullptr)
			return;

		if(m_camWorld == _trans->GetWorld() && !m_bIsDirty)
			return;

		m_camWorld == _trans->GetWorld();

		_info._cameraWorldPos = m_camWorld.Translation();
		_info._near = m_fNear;
		_info._far = m_fFar;

		_info._view = math::Matrix::CreateLookAt(_info._cameraWorldPos, _info._cameraWorldPos + m_camWorld.Forward(), m_camWorld.Up());
		_info._proj = math::Matrix::CreatePerspectiveFieldOfView(m_fFieldOfView, m_fAspectRadio, m_fNear, m_fFar);
		_info._projInv = _info._proj.Invert();
		_info._viewToTexSpace = _info._view * _info._proj * math::Matrix(
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, -0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f
		);
		_info._worldViewProj = m_camWorld * _info._view * _info._proj;

		m_pCameraBuffer->Update(_info);

		graphics::GraphicsEngine::GetInstance()->PushCameraBuffer(m_pCameraBuffer);
	}

	void Camera::OnDestroy()
	{
		assert(graphics::ResourceManager::GetInstance()->RelaseTextureBuffer(GetUUID()));

		m_pCameraBuffer = nullptr;
	}

	void* Camera::GetMainTextureID()
	{
		auto _main = m_MainCamera.lock();

		return _main == nullptr ? nullptr : _main->m_pCameraBuffer->GetTextureID();
	}
}
