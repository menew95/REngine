#include <rengine\core\resource\Material.h>
#include <rengine\core\resource\Texture.h>

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

vector<shared_ptr<rengine::Material>>  converter_func_vec_weak_container(const vector<weak_ptr<rengine::Object>>& value, bool& ok)
{
	ok = true;

	std::vector<shared_ptr<rengine::Material>> _ret;

	for (auto& _ptr : value)
	{
		_ret.push_back(std::static_pointer_cast<rengine::Material>(_ptr.lock()));
	}

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
	rttr::type::register_converter_func(converter_func_vec_weak_container);
}

#include <Serialize\Serializer.h>

namespace rengine
{
	Material::Material(uuid uuid)
	: Resource(uuid, TEXT("Material"))
	{
		m_pMaterialBuffer = graphics::ResourceManager::GetInstance()->CreateMaterialBuffer(GetUUID());

		m_pMaterialBuffer->SetName(GetNameStr().c_str());
	}

	Material::~Material()
	{
		graphics::ResourceManager::GetInstance()->RelaseMaterialBuffer(GetUUID());
	}

	bool Material::LoadMemory()
	{
		if (!m_bIsLoad)
		{
			SetRenderPassID(m_renderPassID);

			SetPipelineID(m_pipelineID);

			m_bIsLoad = true;
		}

		return m_bIsLoad;
	}

	bool Material::UnLoadMemory()
	{
		auto _ret = m_pMaterialBuffer->UnLoadMaterial();
		
		m_bIsLoad = !_ret;

		return _ret;
	}

	void Material::SetRenderPassID(const tstring& pass)
	{
		m_renderPassID = pass;

		m_bIsDirty = true;
		m_bIsLoad = true;

		m_pMaterialBuffer->SetRenderPass(pass);
	}

	void Material::SetPipelineID(const tstring& id)
	{
		m_pipelineID = id;

		m_bIsDirty = true;
		m_bIsLoad = true;

		m_pMaterialBuffer->SetPipelineID(id);

		for (auto& _bufProp : m_pMaterialBuffer->GetProperties())
		{
			MaterialProperty _prop;
			_prop.m_type = (MaterialProperty::PropType)_bufProp._type;
			_prop.m_name = _bufProp._name;

			if (_bufProp._defaultValue != nullptr)
			{
				switch (_prop.m_type)
				{
					case MaterialProperty::PropType::Int:
					{
						_prop.m_intValue  = *((int*)_bufProp._defaultValue);
						break;
					}
					case MaterialProperty::PropType::Float:
					{
						float* _test = (float*)_bufProp._defaultValue;
						float _test2 = *_test;
						_prop.m_floatValue = *((float*)_bufProp._defaultValue);
						break;
					}
					case MaterialProperty::PropType::Color:
					{
						_prop.m_colorValue = *((Color*)_bufProp._defaultValue);
						break;
					}
					case MaterialProperty::PropType::Vector:
					{
						_prop.m_vectorValue = *((Vector4*)_bufProp._defaultValue);
						break;
					}
					case MaterialProperty::PropType::Range:
					case MaterialProperty::PropType::Texture:
					default:
					{
						break;
					}
				}
			}

			m_properties[_prop.m_type].push_back(_prop);
		}
	}

	void Material::SetProperties(map<MaterialProperty::PropType, vector<MaterialProperty>>& val)
	{
		m_properties = val;

		for (auto& [_type, _properties] : val)
		{
			for (auto& _property : _properties)
			{
				switch (_property.GetPropType())
				{
					case MaterialProperty::PropType::Color:
					{
						SetColor(_property.m_name, _property.m_colorValue);
						break;
					}
					case MaterialProperty::PropType::Vector:
					{
						SetVector4(_property.m_name, _property.m_vectorValue);
						break;
					}
					case MaterialProperty::PropType::Float:
					{
						SetFloat(_property.m_name, _property.m_floatValue);
						break;
					}
					case MaterialProperty::PropType::Range:
					{
						// 지금 사용안함
						assert(false);
						//SetRange(_property.m_name, _property.m_rangeLimits);
						break;
					}
					case MaterialProperty::PropType::Texture:
					{
						SetTexture(_property.m_name, _property.m_textureValue.lock());
						break;
					}
					case MaterialProperty::PropType::Int:
					{
						SetInteger(_property.m_name, _property.m_intValue);
						break;
					}
					default:
						assert(false);
						break;
				}
			}
		}
	}

	void Material::SetPropertyBlock(const vector<MaterialProperty>& properties)
	{
		for (auto& _property : properties)
		{
			switch (_property.GetPropType())
			{
			case MaterialProperty::PropType::Color:
			{
				SetColor(_property.m_name, _property.m_colorValue);
				break;
			}
			case MaterialProperty::PropType::Vector:
			{
				SetVector4(_property.m_name, _property.m_vectorValue);
				break;
			}
			case MaterialProperty::PropType::Float:
			{
				SetFloat(_property.m_name, _property.m_floatValue);
				break;
			}
			case MaterialProperty::PropType::Range:
			{
				// 지금 사용안함
				assert(false);
				//SetRange(_property.m_name, _property.m_rangeLimits);
				break;
			}
			case MaterialProperty::PropType::Texture:
			{
				SetTexture(_property.m_name, _property.m_textureValue.lock());
				break;
			}
			case MaterialProperty::PropType::Int:
			{
				SetInteger(_property.m_name, _property.m_intValue);
				break;
			}
			default:
				assert(false);
				break;
			}
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

		m_pMaterialBuffer->SetColor(name, value);

		m_bIsDirty = true;
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

		m_pMaterialBuffer->SetVector4(name, value);

		m_bIsDirty = true;
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

		m_pMaterialBuffer->SetFloat(name, value);

		m_bIsDirty = true;
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

		m_pMaterialBuffer->SetTexture(name, texture != nullptr ? texture->GetTextureBuffer() : nullptr);

		m_bIsDirty = true;
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

		m_pMaterialBuffer->SetInteger(name, value);

		m_bIsDirty = true;
	}
}