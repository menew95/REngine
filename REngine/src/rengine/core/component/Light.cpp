﻿#include <rengine\core\component\Light.h>
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
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::Color)
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
	.property("Inner Spot Angle", &rengine::Light::GetInnerSpotAngle, &rengine::Light::SetInnerSpotAngle)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT)
	)
	.property("Resolution", &rengine::Light::GetResolution, &rengine::Light::SetResolution)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::ENUM),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::UINT32)
	)
	;

	rttr::registration::enumeration<rengine::LightType>("Light Type")
	(
		rttr::value("Spot", rengine::LightType::Spot),
		rttr::value("Directional", rengine::LightType::Directional),
		rttr::value("Point", rengine::LightType::Point)
	);

	rttr::registration::enumeration<rengine::ShadowResolution>("Resolution")
		(
			rttr::value("Low", rengine::ShadowResolution::Low),
			rttr::value("Medium", rengine::ShadowResolution::Medium),
			rttr::value("High", rengine::ShadowResolution::High),
			rttr::value("VeryHigh", rengine::ShadowResolution::VeryHigh)
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
		auto _trans = GetTransform().lock();

		if(_trans == nullptr)
			return;

		auto _world = _trans->GetWorld();

		//m_pLightBuffer->SetWorld(_world);

		m_pLightBuffer->SetDirection(_world.Forward());

		m_pLightBuffer->SetPosition(_world.Translation());

		m_pLightBuffer->SetUp(_world.Up());
	}
	
	void Light::OnEnable()
	{
		m_pLightBuffer->SetEnable(true);
	}
	
	void Light::OnDisable()
	{
		m_pLightBuffer->SetEnable(false);
	}
	
	void Light::OnDestroy()
	{
		assert(graphics::ResourceManager::GetInstance()->RelaseLightBuffer(GetUUID()));

		m_pLightBuffer = nullptr;
	}
	
	void Light::SetLightType(uint32 value)
	{
		m_lightType = (LightType)value;
		
		m_pLightBuffer->SetLightType(value);
	}
	
	void Light::SetLightColor(const math::Color& value)
	{
		m_lightColor = value;

		m_pLightBuffer->SetLightColor(value.ToVector3());
	}
	
	void Light::SetIntensity(float value)
	{
		m_intensity = value;

		m_pLightBuffer->SetIntensity(value);
	}
	
	void Light::SetRange(float value)
	{
		m_range = value;

		m_pLightBuffer->SetRange(value);
	}
	
	void Light::SetSpotAnlge(float value)
	{
		m_spotAngle = value;

		m_pLightBuffer->SetSpotAnlge(value);
	}
	
	void Light::SetInnerSpotAngle(float value)
	{
		m_innerSpotAngle = value;

		m_pLightBuffer->SetInnerSpotAngle(value);
	}

	void Light::SetResolution(uint32 value)
	{
		m_resolution = static_cast<ShadowResolution>(value);

		m_pLightBuffer->SetResolution(value);
	}
}