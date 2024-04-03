#include <rengine\core\component\Light.h>
#include <rengine\core\component\Transform.h>

#include <graphics_core\ResourceManager.h>
#include <graphics_core\resource\LightBuffer.h>

#include <rttr\registration.h>

RTTR_REGISTRATION
{
	rttr::registration::class_<rengine::Light>("Light")
	.constructor<uuid>()
	.property("Light Type", &rengine::Light::GetLightType, &rengine::Light::SetLightType)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::ENUM),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::UINT32)
	)
	.property("Light Color", &rengine::Light::GetLightColor, &rengine::Light::SetLightColor)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::Color),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::VECTOR4)
	)
	.property("Intensity", &rengine::Light::GetIntensity, &rengine::Light::SetIntensity)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT)
	)
	.property("Range", &rengine::Light::GetRange, &rengine::Light::SetRange)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT)
	)
	.property("Spot Angle", &rengine::Light::GetSpotAnlge, &rengine::Light::SetSpotAnlge)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT)
	)
	.property("inner Spot Angle", &rengine::Light::GetInnerSpotAngle, &rengine::Light::SetInnerSpotAngle)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT)
	)
	;

	rttr::registration::enumeration<rengine::LightType>("Light Type")
	(
		rttr::value("Spot", rengine::LightType::Spot),
		rttr::value("Directional", rengine::LightType::Directional),
		rttr::value("Point", rengine::LightType::Point)
	);
}

namespace rengine
{
	Light::Light(uuid uuid)
		: Component(uuid, TEXT("Light"))
	{

	}
	Light::~Light()
	{

	}
	
	void Light::Awake()
	{
		m_pLightBuffer = graphics::ResourceManager::GetInstance()->CreateLightBuffer(GetUUID());
	}
	
	void Light::Update()
	{
	
	}
	
	void Light::OnEnable()
	{
	
	}
	
	void Light::OnDisable()
	{
	
	}
	
	void Light::OnDestroy()
	{
		assert(graphics::ResourceManager::GetInstance()->RelaseLightBuffer(GetUUID()));

		m_pLightBuffer = nullptr;
	}
	
	void Light::SetLightType(uint32 value)
	{
		m_lightType = (LightType)value;
		
		//m_pLightBuffer->SetLightType();
	}
	
	void Light::SetLightColor(const math::Color& value)
	{
		m_lightColor = value;

		//m_pLightBuffer->SetLightColor();
	}
	
	void Light::SetIntensity(float value)
	{
		m_intensity = value;

		//m_pLightBuffer->SetIntensity();
	}
	
	void Light::SetRange(float value)
	{
		m_range = value;

		//m_pLightBuffer->SetRange();
	}
	
	void Light::SetSpotAnlge(float value)
	{
		m_spotAngle = value;

		//m_pLightBuffer->SetSpotAnlge();
	}
	
	void Light::SetInnerSpotAngle(float value)
	{
		m_innerSpotAngle = value;

		//m_pLightBuffer->SetInnerSpotAngle();
	}
}