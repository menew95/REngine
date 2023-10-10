#include "dx11_module_pch.h"

#include "graphics_module\Resource.h"

#include "dx11_module\DX11PipelineLayout.h"

namespace graphics
{
	namespace DX11
	{
		DX11PipelineLayout::DX11PipelineLayout(const PipelineLayoutDesc& desc)
			: m_Bindings(desc._bindings)
			, m_Resources(desc._resources)
		{
			m_Bindings = desc._bindings;
		}

		DX11PipelineLayout::~DX11PipelineLayout()
		{

		}

		void DX11PipelineLayout::SetResources(std::vector<void*>& resources)
		{
			for (uint32 i = 0; i < GetNumBindings(); i++)
			{
				SetResource(i, resources[i]);
			}
		}

		void DX11PipelineLayout::SetResource(uint32 index, void* resource)
		{
			if (index >= m_Bindings.size())
				return;

			Resource* _resource = nullptr;

			if (m_Bindings[index]._arraySize != 1)
			{
				if ((m_Bindings[index]._bindFlags & (BindFlags::ShaderResource | BindFlags::UnorderedAccess)) != 0)
				{
					auto* _castTest = reinterpret_cast<Resource*>(resource);
					_resource = dynamic_cast<Resource*>(_castTest);

					if (_resource == nullptr)
					{
						assert(false);
						return;
					}
				}
				else
				{
					_resource = *(reinterpret_cast<Resource**>(resource));
				}
			}
			else
				_resource = reinterpret_cast<Resource*>(resource);

			if (_resource->GetResourceType() != m_Bindings[index]._type)
			{
				// m_Bindings에 기록된 바인딩 정보와 다른 리소스를 저장하려고 함
				assert(false);
				return;
			}

			// Todo : 슬록, 바인딩 플래그, 스테이지 플래그도 비교가 필요할수도?
			m_Resources[index] = resource;
		}

		uint32 DX11PipelineLayout::GetNumBindings() const
		{
			return static_cast<uint32>(m_Bindings.size());
		}

		void* DX11PipelineLayout::GetResource(uint32 index)
		{
			return m_Resources.at(index);
		}

		graphics::Buffer* DX11PipelineLayout::GetBuffer(uint32 index)
		{
			if (m_Bindings[index]._arraySize != 1)
				return nullptr;

			auto _resource = reinterpret_cast<Resource*>(m_Resources.at(index));

			assert(_resource->GetResourceType() == ResourceType::Buffer);

			return reinterpret_cast<Buffer*>(_resource);
		}

		graphics::Texture* DX11PipelineLayout::GetTexture(uint32 index)
		{
			if (m_Bindings[index]._arraySize != 1)
				return nullptr;

			auto _resource = reinterpret_cast<Resource*>(m_Resources.at(index));

			assert(_resource->GetResourceType() == ResourceType::Texture);

			return reinterpret_cast<Texture*>(_resource);
		}

		graphics::Sampler* DX11PipelineLayout::GetSampler(uint32 index)
		{
			if (m_Bindings[index]._arraySize != 1)
				return nullptr;

			auto _resource = reinterpret_cast<Resource*>(m_Resources.at(index));

			assert(_resource->GetResourceType() == ResourceType::Sampler);

			return reinterpret_cast<Sampler*>(_resource);
		}

		graphics::ResourceView* DX11PipelineLayout::GetResourceView(uint32 index)
		{
			if (m_Bindings[index]._arraySize != 1)
				return nullptr;

			auto _resource = reinterpret_cast<Resource*>(m_Resources.at(index));

			assert(_resource->GetResourceType() == ResourceType::ResourceView);

			return reinterpret_cast<ResourceView*>(_resource);
		}
	}
}