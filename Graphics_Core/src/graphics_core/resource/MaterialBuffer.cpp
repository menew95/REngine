#include <graphics_core\resource\MaterialBuffer.h>
#include <graphics_core\resource\MaterialPropertyBlock.h>
#include <graphics_core\resource\TextureBuffer.h>
#include <graphics_core\ResourceManager.h>
#include <graphics_core\RenderPass.h>

#include <graphics_module\CommandBuffer.h>
#include <graphics_module\PipelineState.h>

namespace graphics
{
	MaterialBuffer::MaterialBuffer()
	{
		m_materialPropertyBlock = make_unique<MaterialPropertyBlock>();
	}

	MaterialBuffer::~MaterialBuffer()
	{

	}

	void MaterialBuffer::SetName(const char* name)
	{

	}

	void MaterialBuffer::SetRenderPass(const tstring& id)
	{
		if(m_pRenderPass != nullptr)
			m_pRenderPass->RemoveMaterialBuffer(this);

		m_pRenderPass = graphics::ResourceManager::GetInstance()->GetRenderPass(id);

		assert(m_pRenderPass != nullptr);

		m_pRenderPass->AddMaterialBuffer(this);
	}

	MaterialBufferProperty::PropertyType GetPropertyType(const BufferField& field)
	{
		switch (field._type)
		{
		case graphics::FieldType::BOOL:
			break;
		case graphics::FieldType::BOOL2:
			break;
		case graphics::FieldType::BOOL3:
			break;
		case graphics::FieldType::BOOL4:
			break;
		case graphics::FieldType::INT:
			return MaterialBufferProperty::PropertyType::Int;
			break;
		case graphics::FieldType::INT2:
			break;
		case graphics::FieldType::INT3:
			break;
		case graphics::FieldType::INT4:
			break;
		case graphics::FieldType::UINT:
			break;
		case graphics::FieldType::UINT2:
			break;
		case graphics::FieldType::UINT3:
			break;
		case graphics::FieldType::UINT4:
			break;
		case graphics::FieldType::FLOAT:
			return MaterialBufferProperty::PropertyType::Float;
			break;
		case graphics::FieldType::FLOAT2:
			break;
		case graphics::FieldType::FLOAT3:
			break;
		case graphics::FieldType::FLOAT4:
		{
			// 만약 이름에 Color가 붙으면 Color 타입을 반환, 아닐 경우 일반 vector타입을 반환
			if(field._name.find(TEXT("Color")) != tstring::npos) return MaterialBufferProperty::PropertyType::Color;
			else return MaterialBufferProperty::PropertyType::Vector;
			break;
		}
		case graphics::FieldType::DOUBLE:
			break;
		case graphics::FieldType::DOUBLE2:
			break;
		case graphics::FieldType::DOUBLE3:
			break;
		case graphics::FieldType::DOUBLE4:
			break;
		default:
			break;
		}

		//int 타입과 vector4타입만 다루도록 만듬 Unity ShaderLab이 다루는 scalr 타입이 int, float, color, vector 타입 네가지임
		assert(false);
		return  MaterialBufferProperty::PropertyType::Color;
	}

	void MaterialBuffer::SetPipelineID(const tstring& pipelineID)
	{
		m_pPipelineState = graphics::ResourceManager::GetInstance()->GetPipelineState(pipelineID);

		assert(m_pPipelineState != nullptr);

		m_materialPropertyBlock->SetProperty(m_pPipelineState->GetPropertyDesc());

		// material class에게 알려주기 위해 버퍼에서도 프로퍼티 정보에 대해 저장을 해준다.
		m_properties.clear();

		for (auto& _constDesc : m_pPipelineState->GetPropertyDesc()._bindBuffers)
		{
			for (auto& _field : _constDesc._fields)
			{
				MaterialBufferProperty _prop;
				{
					_prop._name = _field._name;
					_prop._type = GetPropertyType(_field);
					_prop._defaultValue = _field._defaultValue;
				}

				m_properties.push_back(_prop);
			}
		}

		for (auto& _resourceDesc : m_pPipelineState->GetPropertyDesc()._bindResources)
		{
			MaterialBufferProperty _prop;
			{
				_prop._name = _resourceDesc._name;
				_prop._type = MaterialBufferProperty::PropertyType::Texture;
			}

			m_properties.push_back(_prop);
		}

		m_pPipelineLayout = graphics::ResourceManager::GetInstance()->GetPipelineLayout(pipelineID);
	}

	bool MaterialBuffer::UnLoadMaterial()
	{
		if (m_pRenderPass != nullptr)
			m_pRenderPass->RemoveMaterialBuffer(this);

		return true;
	}

	void MaterialBuffer::AddRenderObject(RenderObject* obj, uint32 subMeshIdx)
	{
		m_renderObjectList.emplace_back(obj, subMeshIdx);
	}

	void MaterialBuffer::RemoveRenderObject(RenderObject* obj)
	{
		auto _it = find_if(std::begin(m_renderObjectList), std::end(m_renderObjectList), 
			[&obj](auto& pair)
			{
				return obj == pair.first;
			});

		if (_it != std::end(m_renderObjectList))
			m_renderObjectList.erase(_it);
	}

	void MaterialBuffer::SetColor(const tstring& name, const Color& value)
	{
		m_materialPropertyBlock->SetColor(name, value);
	}

	void MaterialBuffer::SetVector4(const tstring& name, const Vector4& value)
	{
		m_materialPropertyBlock->SetVector4(name, value);
	}

	void MaterialBuffer::SetFloat(const tstring& name, float value)
	{
		m_materialPropertyBlock->SetFloat(name, value);
	}

	void MaterialBuffer::SetTexture(const tstring& name, TextureBuffer* texture)
	{
		if (texture == nullptr)
		{
			if(name == TEXT("AlbedoMap")) texture = TextureBuffer::White;
			else if(name == TEXT("NormalMap")) texture = TextureBuffer::Bump;
			else if(name == TEXT("MetallicRougnessMap")) texture = TextureBuffer::White;
			else if(name == TEXT("EmissiveMap")) texture = TextureBuffer::White;
			else if(name == TEXT("AmbientOcclusionMap")) texture = TextureBuffer::White;
			else texture = TextureBuffer::Gray;
		}

		m_materialPropertyBlock->SetTexture(name, texture);
	}

	void MaterialBuffer::SetTexture(const tstring& name, Texture* texture)
	{
		if (texture == nullptr)
		{
			if (name == TEXT("AlbedoMap")) texture = TextureBuffer::White->GetTexture();
			else if (name == TEXT("NormalMap")) texture = TextureBuffer::Bump->GetTexture();
			else if (name == TEXT("MetallicRougnessMap")) texture = TextureBuffer::White->GetTexture();
			else if (name == TEXT("EmissiveMap")) texture = TextureBuffer::White->GetTexture();
			else if (name == TEXT("AmbientOcclusionMap")) texture = TextureBuffer::White->GetTexture();
			else texture = TextureBuffer::Gray->GetTexture();
		}

		m_materialPropertyBlock->SetTexture(name, texture);
	}

	void MaterialBuffer::SetResource(const tstring& name, Resource* resource)
	{
		m_materialPropertyBlock->SetResource(name, resource);
	}

	void MaterialBuffer::SetInteger(const tstring& name, int value)
	{
		m_materialPropertyBlock->SetInteger(name, value);
	}

	void MaterialBuffer::BindPipelineState(CommandBuffer* command)
	{
		if(m_pPipelineState == nullptr)
			return;

		command->SetPipelineState(*m_pPipelineState);
	}

	void MaterialBuffer::BindResource(CommandBuffer* command)
	{
		m_materialPropertyBlock->BindProperty(command);
		
		if(m_pPipelineLayout == nullptr)
			return;

		command->SetResources(*m_pPipelineLayout);
	}
}