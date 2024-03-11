#include <rengine\core\resource\Material.h>

#include <rengine\system\GraphicsSystem.h>

#include <graphics_core\ResourceManager.h>

#include <graphics_core\resource\MaterialBuffer.h>

#include <rttr\registration.h>

RTTR_REGISTRATION
{
	rttr::registration::class_<rengine::Material>("Material")
	.constructor<tstring>()
	.property("Properties", &rengine::Material::GetProperties, &rengine::Material::SetProperties)
	;
}

namespace rengine
{
	Material::Material(uuid uuid)
	: Resource(uuid, TEXT("Material"))
	{

	}

	Material::~Material()
	{
	}

	bool Material::LoadMemory()
	{
		m_pMaterialBuffer = graphics::ResourceManager::GetInstance()->CreateMaterialBuffer(GetUUID());

		m_pMaterialBuffer->SetName(GetNameStr().c_str());

		m_bIsLoad = true;

		return true;
	}

	bool Material::UnLoadMemory()
	{
		bool _ret = graphics::ResourceManager::GetInstance()->CreateMaterialBuffer(GetUUID());

		if (_ret)
		{
			m_pMaterialBuffer = nullptr;

			m_bIsLoad = false;
		}

		return _ret;
	}

	void Material::AddProperties(vector<MaterialProperty> properties)
	{
		for (auto& _prop : properties)
		{
			m_properties[_prop.GetPropType()].push_back(_prop);
		}
	}
}