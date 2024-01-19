#include <rengine\core\resource\Material.h>

#include <rengine\system\GraphicsSystem.h>

#include <graphics_core\ResourceManager.h>

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

}