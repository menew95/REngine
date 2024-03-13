#include <rengine\core\component\Renderer.h>
#include <rengine\core\component\Transform.h>

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
		_ret.push_back(std::static_pointer_cast<rengine::Material>(_ptr.lock()));
	}

	return _ret;
}

RTTR_REGISTRATION
{
	rttr::registration::class_<rengine::Renderer>("Renderer")
	.constructor<tstring, tstring>()
	.property("Mesh", &rengine::Renderer::GetMesh, &rengine::Renderer::SetMesh)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::UUID)
	)
	.property("Materials", &rengine::Renderer::GetMaterials, &rengine::Renderer::SetMaterials)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::UUID)
	)
	;

	rttr::type::register_converter_func(converter_func_weak_container);
}

namespace rengine
{
	Renderer::Renderer(uuid uuid, tstring type)
		: Component(uuid, type)
	{

	}

	Renderer::~Renderer()
	{

	}

	void Renderer::SetMaterials(vector<weak_ptr<Material>> val)
	{
		for (auto& _mat : m_materials)
		{
			auto _material = _mat.lock();

			if (_material == nullptr)
				continue;

			_material->GetMaterialBuffer()->RemoveRenderObject(m_pRenderObject);
		}

		m_materials = val;

		for (auto& _mat : m_materials)
		{
			auto _material = _mat.lock();

			if(_material == nullptr)
				continue;

			_material->GetMaterialBuffer()->AddRenderObject(m_pRenderObject);
		}
	}

	void Renderer::SetMaterial(size_t i, shared_ptr<Material>& mat)
	{
		if(m_materials.size() <= i)
			return;

		if(m_materials[i].lock() != nullptr)
			m_materials[i].lock()->GetMaterialBuffer()->RemoveRenderObject(m_pRenderObject);

		m_materials[i] = mat;

		mat->GetMaterialBuffer()->AddRenderObject(m_pRenderObject);

		m_pRenderObject->SetMaterialBuffer(i, mat->GetMaterialBuffer());
	}

	void Renderer::SetMesh(shared_ptr<Mesh> mesh)
	{
		m_mesh = mesh;

		if(mesh == nullptr) m_pRenderObject->SetMeshBuffer(nullptr);
		else m_pRenderObject->SetMeshBuffer(m_mesh.lock()->GetMeshBuffer());
	}

	void Renderer::Render()
	{
		auto _trans = GetTransform().lock();

		if(_trans == nullptr)
			return;

		m_pRenderObject->SetWorld(_trans->GetWorld());
	}

	void Renderer::OnDisable()
	{
		graphics::RenderQueue::GetInstance()->DeleteObject(m_pRenderObject);
	}
}