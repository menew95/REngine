#include <graphics_core\resource\MaterialPropertyBlock.h>
#include <graphics_core\resource\ByteBuffer.h>
#include <graphics_core\resource\TextureBuffer.h>
#include <graphics_core\ResourceManager.h>
#include <graphics_module\Texture.h>
#include <graphics_module\Sampler.h>
#include <graphics_module\Buffer.h>
#include <common\math.h>

#include <graphics_module\CommandBuffer.h>

namespace graphics
{
	/*void MaterialPropertyBlock::BindProperty(PipelineLayout* layout)
	{
		
	}*/
	
	void MaterialPropertyBlock::SetProperty(const PropertyDesc& desc)
	{
		m_propertyDesc = desc;

		m_constantBlock.clear();
		m_constBuffers.clear();
		m_resourceBlock.clear();
		m_samplerBlock.clear();

		if (m_propertyDesc._bindBuffers.size() != 0)
		{
			assert(m_propertyDesc._bindBuffers[0]._name == TEXT("PerMaterial"));

			BindingDescriptor _desc
			{
				m_propertyDesc._bindBuffers[0]._name,
				(ResourceType)m_propertyDesc._bindBuffers[0]._resourceType,
				BindFlags::ConstantBuffer,
				m_propertyDesc._bindBuffers[0]._stageFlags,
				m_propertyDesc._bindBuffers[0]._boundSlot,
				m_propertyDesc._bindBuffers[0]._boundCount
			};

			auto _iter = m_constantBlock.insert(
					make_pair(
						m_propertyDesc._bindBuffers[0]._name, 
						make_pair(_desc, make_shared<ByteBuffer>())
					)
				);

			_iter.first->second.second->SetBufferSize(m_propertyDesc._bindBuffers[0]._size);

			m_constBuffers[m_propertyDesc._bindBuffers[0]._name] =
				ResourceManager::GetInstance()->GetBuffer(m_propertyDesc._bindBuffers[0]._name);

			for (auto& _field : m_propertyDesc._bindBuffers[0]._fields)
			{
				if (_field._defaultValue == nullptr || _field._size > 16)
					continue;

				m_constantBlock[TEXT("PerMaterial")].second->UpdateBuffer(
					&_field._default,
					_field._size, _field._offset
				);
			}
		}

		for (auto& resource : desc._bindResources)
		{
			BindingDescriptor _desc
			{
				resource._name,
				(ResourceType)resource._resourceType,
				BindFlags::ShaderResource,
				resource._stageFlags,
				resource._boundSlot,
				resource._boundCount
			};

			m_resourceBlock.insert(
				make_pair(
					resource._name, 
					make_pair(_desc, nullptr)
				)
			);
		}

		for (auto& resource : desc._bindSamplers)
		{
			BindingDescriptor _desc
			{
				resource._name,
				(ResourceType)resource._resourceType,
				BindFlags::ShaderResource,
				resource._stageFlags,
				resource._boundSlot,
				resource._boundCount
			};

			m_samplerBlock.insert(
				make_pair(
					resource._name,
					make_pair(_desc, nullptr)
				)
			);
		}
	}

	void MaterialPropertyBlock::BindProperty(CommandBuffer* command)
	{
		for (const auto& [name, pair] : m_constantBlock)
		{
			command->UpdateBuffer(*m_constBuffers[name], 
				0,
				pair.second->GetBuffer(),
				static_cast<uint32>(pair.second->GetSize())
			);

			command->SetResource(
				*m_constBuffers[name],
				pair.first._slot,
				pair.first._bindFlags,
				pair.first._stageFlags
			);
		}

		for(const auto& [name, pair] : m_resourceBlock)
		{
			command->SetResource(
				*pair.second,
				pair.first._slot,
				pair.first._bindFlags,
				pair.first._stageFlags
			);
		}

		for (const auto& [name, pair] : m_samplerBlock)
		{
			command->SetResource(
				*pair.second,
				pair.first._slot,
				pair.first._bindFlags,
				pair.first._stageFlags
			);
		}
	}

	void MaterialPropertyBlock::SetColor(const tstring& name, const math::Color& value)
	{
		assert(m_propertyDesc._bindBuffers.size() != 0);
		assert(m_propertyDesc._bindBuffers[0]._name == TEXT("PerMaterial"));

		auto _iter = find_if(begin(m_propertyDesc._bindBuffers[0]._fields), end(m_propertyDesc._bindBuffers[0]._fields), [&name](auto& desc)
			{
				return desc._name == name;
			});

		if (_iter == m_propertyDesc._bindBuffers[0]._fields.end())
			return;

		float _val[4] = { value.x, value.y, value.z, value.w };

		m_constantBlock[TEXT("PerMaterial")].second->UpdateBuffer(_val, _iter->_size, _iter->_offset);
	}
	
	void MaterialPropertyBlock::SetVector4(const tstring& name, const math::Vector4& value)
	{
		assert(m_propertyDesc._bindBuffers.size() != 0);
		assert(m_propertyDesc._bindBuffers[0]._name == TEXT("PerMaterial"));

		auto _iter = find_if(begin(m_propertyDesc._bindBuffers[0]._fields), end(m_propertyDesc._bindBuffers[0]._fields), [&name](auto& desc)
			{
				return desc._name == name;
			});

		if (_iter == m_propertyDesc._bindBuffers[0]._fields.end())
			return;

		float _val[4] = { value.x, value.y, value.z, value.w };

		m_constantBlock[TEXT("PerMaterial")].second->UpdateBuffer(_val, _iter->_size, _iter->_offset);
	}
	
	void MaterialPropertyBlock::SetFloat(const tstring& name, float value)
	{
		assert(m_propertyDesc._bindBuffers.size() != 0);
		assert(m_propertyDesc._bindBuffers[0]._name == TEXT("PerMaterial"));

		auto _iter = find_if(begin(m_propertyDesc._bindBuffers[0]._fields), end(m_propertyDesc._bindBuffers[0]._fields), [&name](auto& desc)
			{
				return desc._name == name;
			});

		if (_iter == m_propertyDesc._bindBuffers[0]._fields.end())
			return;

		m_constantBlock[TEXT("PerMaterial")].second->UpdateBuffer(&value, _iter->_size, _iter->_offset);
	}
	
	void MaterialPropertyBlock::SetInteger(const tstring& name, int value)
	{
		assert(m_propertyDesc._bindBuffers.size() != 0);
		assert(m_propertyDesc._bindBuffers[0]._name == TEXT("PerMaterial"));

		auto _iter = find_if(begin(m_propertyDesc._bindBuffers[0]._fields), end(m_propertyDesc._bindBuffers[0]._fields), [&name](auto& desc)
			{
				return desc._name == name;
			});

		if (_iter == m_propertyDesc._bindBuffers[0]._fields.end())
			return;

		m_constantBlock[TEXT("PerMaterial")].second->UpdateBuffer(&value, _iter->_size, _iter->_offset);
	}
	
	void MaterialPropertyBlock::SetTexture(const tstring& name, TextureBuffer* textureBuffer)
	{
		auto _iter = find_if(begin(m_propertyDesc._bindResources), end(m_propertyDesc._bindResources), [&name](auto& desc)
			{
				return desc._name == name;
			});

		if (_iter == m_propertyDesc._bindResources.end())
			return;

		m_resourceBlock[_iter->_name].second = textureBuffer->GetTexture();
	}

	void MaterialPropertyBlock::SetTexture(const tstring& name, Texture* texture)
	{
		auto _iter = find_if(begin(m_propertyDesc._bindResources), end(m_propertyDesc._bindResources), [&name](auto& desc)
			{
				return desc._name == name;
			});

		if (_iter == m_propertyDesc._bindResources.end())
			return;

		m_resourceBlock[_iter->_name].second = texture;
	}

	void MaterialPropertyBlock::SetResource(const tstring& name, Resource* resource)
	{
		auto _iter = find_if(begin(m_propertyDesc._bindResources), end(m_propertyDesc._bindResources), [&name](auto& desc)
			{
				return desc._name == name;
			});

		if (_iter == m_propertyDesc._bindResources.end())
			return;

		m_resourceBlock[_iter->_name].second = resource;
	}
}