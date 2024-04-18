#include <rengine\core\component\TestComponent.h>

#include <rttr\registration.h>

RTTR_REGISTRATION
{
	rttr::registration::class_<rengine::TestComponent>("TestComponent")
	.constructor<uuid>()
	.property("m_tstring", &rengine::TestComponent::m_tstring)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::WSTRING)
	)
	.property("m_vec2", &rengine::TestComponent::m_vec2)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::VECTOR3)
	)
	.property("m_vec3", &rengine::TestComponent::m_vec3)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::VECTOR3)
	)
	.property("m_vec4", &rengine::TestComponent::m_vec4)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::VECTOR4)
	)
	.property("m_uint32", &rengine::TestComponent::m_uint32)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::UINT32)
	)
	.property("m_int32", &rengine::TestComponent::m_int32)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::INT32)
	)
	.property("m_float", &rengine::TestComponent::m_float)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::FLOAT)
	)
	.property("m_double", &rengine::TestComponent::m_double)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::DOUBLE)
	)
	.property("m_struct", &rengine::TestComponent::GetStruct, &rengine::TestComponent::SetStruct)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::Structure)
	)
	;

	rttr::registration::class_<rengine::TestComponent::TestStruct>("TestStruct")
	.constructor<>()
	.property("_color", &rengine::TestComponent::TestStruct::GetColor, &rengine::TestComponent::TestStruct::SetColor)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::Color)
	)
	.property("TestEnum", &rengine::TestComponent::TestStruct::GetEnum, &rengine::TestComponent::TestStruct::SetEnum)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::ENUM)
	);

	rttr::registration::enumeration<rengine::TestComponent::TestEnum>("TestEnum")
		(
			rttr::value("ENUM1", rengine::TestComponent::TestEnum::ENUM1),
			rttr::value("ENUM2", rengine::TestComponent::TestEnum::ENUM2),
			rttr::value("ENUM3", rengine::TestComponent::TestEnum::ENUM3)
			);
}

namespace rengine
{
	TestComponent::TestComponent(uuid uuid)
		: Component(uuid, TEXT("TestComponent"))
	{
	}

	TestComponent::~TestComponent()
	{
	}
}