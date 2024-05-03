﻿#include <rengine\core\component\Renderer.h>
#include <rengine\core\component\Transform.h>

#include <rengine\core\EventManager.h>

#include <rengine\core\object\GameObject.h>
#include <rengine\core\resource\Mesh.h>
#include <rengine\core\resource\Material.h>

#include <graphics_core\resource\MeshBuffer.h>
#include <graphics_core\resource\MaterialBuffer.h>

#include <graphics_core\RenderQueue.h>

#include <rttr\registration.h>

std::vector<weak_ptr<rengine::Material>> converter_func_weak_container(const vector<weak_ptr<rengine::Object>>& value, bool& ok)
{
	ok = true;

	std::vector<weak_ptr<rengine::Material>> _ret;

	for (auto& _ptr : value)
	{
		if (_ptr.lock() == nullptr)
			_ret.push_back(std::static_pointer_cast<rengine::Material>(_ptr.lock()));
		else
		{
			_ret.push_back(std::static_pointer_cast<rengine::Material>(_ptr.lock()));
			ok = _ret.back().lock() != nullptr;
		}
	}

	return _ret;
}

std::vector<shared_ptr<rengine::Material>> converter_func_shared_container(const vector<shared_ptr<rengine::Object>>& value, bool& ok)
{
	ok = true;

	std::vector<shared_ptr<rengine::Material>> _ret;

	for (auto& _ptr : value)
	{
		if(_ptr == nullptr)
			_ret.push_back(nullptr);
		else
		{
			_ret.push_back(std::static_pointer_cast<rengine::Material>(_ptr));
			ok = _ret.back() != nullptr;
		}
	}

	return _ret;
}

RTTR_REGISTRATION
{
	rttr::registration::class_<rengine::Renderer>("Renderer")
	.constructor<const uuid&, const tstring&>()
	.property("Mesh", &rengine::Renderer::GetMesh, &rengine::Renderer::SetMesh)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::UUID),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::UUID)
	)
	.property("Materials", &rengine::Renderer::GetMaterials, &rengine::Renderer::SetMaterials)
	(
		rttr::metadata(rengine::MetaData::Editor, rengine::MetaDataType::UUID),
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::UUID)
	)
	;

	rttr::type::register_converter_func(converter_func_weak_container);
	rttr::type::register_converter_func(converter_func_shared_container);
}

namespace rengine
{
	Renderer::Renderer(const uuid& uuid, const tstring& type)
		: Component(uuid, type)
	{

	}

	Renderer::~Renderer()
	{

	}

	vector<shared_ptr<Material>> Renderer::GetMaterials()
	{
		vector<shared_ptr<Material>> _ret;
		
		for (size_t i = 0; i < m_materials.size(); i++)
		{
			auto _mat = m_materials[i].lock();

			_ret.push_back(_mat);
		}

		return _ret;
	}

	void Renderer::SetMaterials(vector<shared_ptr<Material>> val)
	{
		// 각 머티리얼에 할당된 렌더패스에서 등록된 오브젝트 삭제
		for (uint32 i = 0; i < m_materials.size(); i++)
		{
			auto _material = m_materials[i].lock();

			if (_material == nullptr)
				continue;

			_material->GetMaterialBuffer()->RemoveRenderObject(m_pRenderObject, i);
		}

		m_materials.resize(val.size());

		std::copy(val.begin(), val.end(), m_materials.begin());


		// 각 머티리얼에 할당된 렌더패스에서 오브젝트 추가 등록
		for (uint32 i = 0; i < m_materials.size(); i++)
		{
			auto _material = m_materials[i].lock();

			if (_material == nullptr)
				continue;

			_material->GetMaterialBuffer()->AddRenderObject(m_pRenderObject, i);
		}
	}

	void Renderer::SetMaterial(size_t i, shared_ptr<Material>& mat)
	{
		if(m_materials.size() <= i)
			return;

		if(m_materials[i].lock() != nullptr)
			m_materials[i].lock()->GetMaterialBuffer()->RemoveRenderObject(m_pRenderObject, i);

		m_materials[i] = mat;

		m_pRenderObject->SetMaterialBuffer(i, mat->GetMaterialBuffer());

		mat->GetMaterialBuffer()->AddRenderObject(m_pRenderObject, static_cast<uint32>(i));
	}

	void Renderer::SetMesh(shared_ptr<Mesh> mesh)
	{
		m_mesh = mesh;

		if(mesh == nullptr) m_pRenderObject->SetMeshBuffer(nullptr);
		else
		{
			m_pRenderObject->SetMeshBuffer(m_mesh.lock()->GetMeshBuffer());

			m_pRenderObject->SetBoundinBoxMin(m_mesh.lock()->GetBoundingBoxMin());
			m_pRenderObject->SetBoundinBoxMax(m_mesh.lock()->GetBoundingBoxMax());
		}
	}

	void Renderer::Render()
	{
		auto _trans = m_pGameObject.lock()->GetTransform();

		if (_trans == nullptr)
			return;

		m_pRenderObject->SetWorld(_trans->GetWorld());
	}

	void Renderer::OnEnable()
	{
		m_pRenderObject->SetEnable(true);

		if(m_pGameObject.lock() != nullptr)
			m_pRenderObject->SetHash(m_pGameObject.lock()->GetHash());

		if (m_eventListenerID == UINT64_MAX)
		{
			std::function<void()> _functor = std::bind(&Renderer::Render, this);

			m_eventListenerID = EventManager::GetInstance()->AddEventFunction(TEXT("SceneRendering"), _functor);
		}
	}

	void Renderer::OnDisable()
	{
		m_pRenderObject->SetEnable(false);

		if (m_eventListenerID != UINT64_MAX)
		{
			EventManager::GetInstance()->RemoveEventFunction<void>(TEXT("SceneRendering"), m_eventListenerID);

			m_eventListenerID = UINT64_MAX;
		}
	}

	void Renderer::OnDestroy()
	{
		// 각 머티리얼에 할당된 렌더패스에서 등록된 오브젝트 삭제
		for (uint32 i = 0; i < m_materials.size(); i++)
		{
			auto _material = m_materials[i].lock();

			if (_material == nullptr)
				continue;

			_material->GetMaterialBuffer()->RemoveRenderObject(m_pRenderObject, i);
		}

		graphics::RenderQueue::GetInstance()->DeleteObject(m_pRenderObject);
	}
}