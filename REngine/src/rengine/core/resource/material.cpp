﻿#include <rengine\core\resource\Material.h>

#include <rengine\system\GraphicsSystem.h>

#include <graphics_core\ResourceManager.h>

#include <graphics_core\resource\MaterialBuffer.h>

#include <rttr\registration.h>

std::weak_ptr<rengine::Material> converter_func_weak_container(const weak_ptr<rengine::Object>& value, bool& ok)
{
	std::weak_ptr<rengine::Material> _ret = std::static_pointer_cast<rengine::Material>(value.lock());

	ok = _ret.lock() != nullptr || value.lock() == nullptr;

	return _ret;
}

std::shared_ptr<rengine::Material> converter_func_shared_container(const shared_ptr<rengine::Object>& value, bool& ok)
{
	std::shared_ptr<rengine::Material> _ret = std::static_pointer_cast<rengine::Material>(value);

	ok = _ret != nullptr || value == nullptr;

	return _ret;
}

RTTR_REGISTRATION
{
	rttr::registration::class_<rengine::Material>("Material")
	.constructor<tstring>()
	.property("Properties", &rengine::Material::GetProperties, &rengine::Material::SetProperties)
	.property("m_renderPassID", &rengine::Material::GetRenderPassID, &rengine::Material::SetRenderPassID)
	.property("m_pipelineID", &rengine::Material::GetPipelineID, &rengine::Material::SetPipelineID)
	;
	rttr::type::register_converter_func(converter_func_weak_container);
	rttr::type::register_converter_func(converter_func_shared_container);
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

		if (m_bIsDirty)
		{
			//m_pMaterialBuffer->SetRenderPass(m_renderPassID);

			//m_pMaterialBuffer->SetPipelineID(m_pipelineID);

			m_bIsDirty = false;
		}

		return true;
	}

	bool Material::UnLoadMemory()
	{
		bool _ret = graphics::ResourceManager::GetInstance()->RelaseMaterialBuffer(GetUUID());

		if (_ret)
		{
			m_pMaterialBuffer = nullptr;

			m_bIsLoad = false;
		}

		return _ret;
	}

	void Material::SetRenderPassID(const tstring& pass)
	{
		m_renderPassID = pass;

		if (m_pMaterialBuffer == nullptr)
		{
			m_bIsDirty = true;
			return;
		}

		m_pMaterialBuffer->SetRenderPass(pass);
	}

	void Material::SetPipelineID(const tstring& id)
	{
		m_pipelineID = id;

		if(m_pMaterialBuffer == nullptr)
		{
			m_bIsDirty = true;
			return;
		}

		m_pMaterialBuffer->SetPipelineID(id);
	}

	void Material::AddProperties(vector<MaterialProperty> properties)
	{
		for (auto& _prop : properties)
		{
			m_properties[_prop.GetPropType()].push_back(_prop);
		}
	}
	
	void Material::SetColor(const tstring& name, const Color& value)
	{
		auto& _props = m_properties[MaterialProperty::PropType::Color];

		auto _iter = find_if(begin(_props), end(_props), [&name](const auto& prop)
			{
				return prop.m_name == name;
			});

		if (_iter == _props.end())
			return;

		_iter->SetColor(value);

	}
	
	void Material::SetVector4(const tstring& name, const Vector4& value)
	{
		auto& _props = m_properties[MaterialProperty::PropType::Vector];

		auto _iter = find_if(begin(_props), end(_props), [&name](const auto& prop)
			{
				return prop.m_name == name;
			});

		if (_iter == _props.end())
			return;

		_iter->SetVector4(value);
	}
	
	void Material::SetFloat(const tstring& name, float value)
	{
		auto& _props = m_properties[MaterialProperty::PropType::Float];

		auto _iter = find_if(begin(_props), end(_props), [&name](const auto& prop)
			{
				return prop.m_name == name;
			});

		if (_iter == _props.end())
			return;

		_iter->SetFloat(value);
	}
	
	void Material::SetTexture(const tstring& name, const shared_ptr<Texture>& texture)
	{
		auto& _props = m_properties[MaterialProperty::PropType::Texture];

		auto _iter = find_if(begin(_props), end(_props), [&name](const auto& prop)
			{
				return prop.m_name == name;
			});

		if (_iter == _props.end())
			return;

		_iter->SetTexture(texture);
	}
	
	void Material::SetInteger(const tstring& name, int value)
	{
		auto& _props = m_properties[MaterialProperty::PropType::Int];

		auto _iter = find_if(begin(_props), end(_props), [&name](const auto& prop)
			{
				return prop.m_name == name;
			});

		if (_iter == _props.end())
			return;

		_iter->SetInt(value);
	}
}