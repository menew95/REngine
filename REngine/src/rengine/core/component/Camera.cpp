#include <rengine\core\component\Camera.h>

#include <rengine\core\object\GameObject.h>
#include <rengine\core\scene\scene.h>

#include <rttr\registration.h>

RTTR_REGISTRATION
{
	rttr::registration::class_<rengine::Camera>("Camera")
	.constructor<std::shared_ptr<rengine::GameObject>&>()
	.constructor<std::shared_ptr<rengine::GameObject>&, uuid>()
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
	Camera::Camera(std::shared_ptr<GameObject>& gameObj)
		: Component(gameObj)
	{
		SetName(TEXT("Camera"));
	}

	Camera::Camera(std::shared_ptr<GameObject>& gameObj, uuid uuid)
		: Component(gameObj, uuid)
	{
		SetName(TEXT("Camera"));
	}

	Camera::~Camera()
	{

	}
}
