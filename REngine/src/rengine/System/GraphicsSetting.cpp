#include <rengine\System\GraphicsSetting.h>

#include <rttr\registration.h>

using namespace rttr;

const size_t columns_size[] = { 1, 2 };

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<rengine::ScreenSpaceReflection>("ScreenSpaceReflection")
	.constructor<>()
	.property("Stride", &rengine::ScreenSpaceReflection::GetStride, &rengine::ScreenSpaceReflection::SetStride)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT)
	)
	.property("MaxSteps", &rengine::ScreenSpaceReflection::GetMaxSteps, &rengine::ScreenSpaceReflection::SetMaxSteps)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT)
	)
	.property("MaxDistance", &rengine::ScreenSpaceReflection::GetMaxDistance, &rengine::ScreenSpaceReflection::SetMaxDistance)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT)
	)
	.property("StrideZCutoff", &rengine::ScreenSpaceReflection::GetStrideZCutoff, &rengine::ScreenSpaceReflection::SetStrideZCutoff)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT)
	)
	.property("NumMips", &rengine::ScreenSpaceReflection::GetNumMips, &rengine::ScreenSpaceReflection::SetNumMips)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT)
	)
	.property("FadeStart", &rengine::ScreenSpaceReflection::GetFadeStart, &rengine::ScreenSpaceReflection::SetFadeStart)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT)
	)
	.property("FadeEnd", &rengine::ScreenSpaceReflection::GetFadeEnd, &rengine::ScreenSpaceReflection::SetFadeEnd)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT)
	)
	.property("ZThickness", &rengine::ScreenSpaceReflection::GetZThickness, &rengine::ScreenSpaceReflection::SetZThickness)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT)
	)
	;

	registration::class_<rengine::ExponentialHeightFog>("ExponentialHeightFog")
	.constructor<>()
	.property("FogEnsity", &rengine::ExponentialHeightFog::GetFogEnsity, &rengine::ExponentialHeightFog::SetFogEnsity)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT)
	)
	.property("FogMaxOpacity", &rengine::ExponentialHeightFog::GetFogMaxOpacity, &rengine::ExponentialHeightFog::SetFogMaxOpacity)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT)
	)
	.property("FogStartDistance", &rengine::ExponentialHeightFog::GetFogStartDistance, &rengine::ExponentialHeightFog::SetFogStartDistance)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT)
	)
	.property("CutOffDistance", &rengine::ExponentialHeightFog::GetCutOffDistance, &rengine::ExponentialHeightFog::SetCutOffDistance)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT)
	)
	.property("FogHeightFalloff", &rengine::ExponentialHeightFog::GetFogHeightFalloff, &rengine::ExponentialHeightFog::SetFogHeightFalloff)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT)
	)
	.property("FogHeight", &rengine::ExponentialHeightFog::GetFogHeight, &rengine::ExponentialHeightFog::SetFogHeight)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::VECTOR3),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::VECTOR3)
	)
	.property("FogScatteringColor", &rengine::ExponentialHeightFog::GetFogScatteringColor, &rengine::ExponentialHeightFog::SetFogScatteringColor)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::VECTOR3),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::VECTOR3)
	)
	.property("LightScatterFlag", &rengine::ExponentialHeightFog::GetLightScatterFlag, &rengine::ExponentialHeightFog::SetLightScatterFlag)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT)
	)
	;

	registration::class_<rengine::RimLight>("RimLight")
	.constructor<>()
	.property("ClipPoint", &rengine::RimLight::GetClipPoint, &rengine::RimLight::SetClipPoint)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT)
	)
	.property("RimDecrease", &rengine::RimLight::GetRimDecrease, &rengine::RimLight::SetRimDecrease)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT)
	)
	;

	registration::class_<rengine::Bloom>("Bloom")
	.constructor<>()
	.property("Intensity", &rengine::Bloom::GetIntensity, &rengine::Bloom::SetIntensity)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT)
	)
	.property("Threshhold", &rengine::Bloom::GetThreshhold, &rengine::Bloom::SetThreshhold)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT)
	)
	;

	registration::class_<rengine::SSAO>("SSAO")
	.constructor<>()
	;

	registration::class_<rengine::ToneMapping>("ToneMapping")
	.constructor<>()
	.property("Exposure", &rengine::ToneMapping::GetExposure, &rengine::ToneMapping::SetExposure)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::FLOAT),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT)
	)
	;

	registration::class_<rengine::PostProcessingSetting>("PostProcessingSetting")
	.constructor<>()
	.property("ScreenSpaceReflection", &rengine::PostProcessingSetting::GetScreenSpaceReflection, &rengine::PostProcessingSetting::SetScreenSpaceReflection)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::Structure),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::Structure),
		metadata("Columns Size", columns_size[1])
	)
	.property("ExponentialHeightFog", &rengine::PostProcessingSetting::GetExponentialHeightFog, &rengine::PostProcessingSetting::SetExponentialHeightFog)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::Structure),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::Structure),
		metadata("Columns Size", columns_size[1])
	)
	.property("RimLight", &rengine::PostProcessingSetting::GetRimLight, &rengine::PostProcessingSetting::SetRimLight)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::Structure),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::Structure),
		metadata("Columns Size", columns_size[1])
	)
	.property("Bloom", &rengine::PostProcessingSetting::GetBloom, &rengine::PostProcessingSetting::SetBloom)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::Structure),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::Structure),
		metadata("Columns Size", columns_size[1])
	)
	//.property("SSAO", rengine::PostProcessingSetting::GetSSAO, &rengine::PostProcessingSetting::SetSSAO)
	//(
	//	metadata(rengine::MetaData::Serializable, rengine::MetaDataType::Structure),
	//	metadata(rengine::MetaData::Editor, rengine::MetaDataType::Structure)
	//)
	.property("ToneMapping", &rengine::PostProcessingSetting::GetToneMapping, &rengine::PostProcessingSetting::SetToneMapping)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::Structure),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::Structure),
		metadata("Columns Size", columns_size[1])
	)
	;

	registration::class_<rengine::GraphicsSetting>("GraphicsSetting")
	.constructor<>()
	.property("PostProcessingSetting", &rengine::GraphicsSetting::GetPostProcessingSetting, &rengine::GraphicsSetting::SetPostProcessingSetting)
	(
		metadata(rengine::MetaData::Serializable, rengine::MetaDataType::Structure),
		metadata(rengine::MetaData::Editor, rengine::MetaDataType::Structure),
		metadata("Columns Size", columns_size[0])
	)
	;
}

namespace rengine
{

}