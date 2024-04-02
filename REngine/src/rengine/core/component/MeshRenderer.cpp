#include <rengine\core\component\MeshRenderer.h>
#include <rengine\core\component\MeshFilter.h>
#include <rengine\core\object\GameObject.h>
#include <rengine\core\resource\Mesh.h>
#include <rengine\core\resource\Material.h>

#include <rengine\core\component\Transform.h>

#include <rttr\registration.h>

#include <graphics_core\RenderQueue.h>

RTTR_REGISTRATION
{
	rttr::registration::class_<rengine::MeshRenderer>("MeshRenderer")
	.constructor<tstring>()
	;
}

namespace rengine
{
	MeshRenderer::MeshRenderer(uuid uuid)
		: Renderer(uuid, TEXT("MeshRenderer"))
	{

	}

	MeshRenderer::~MeshRenderer()
	{

	}

	void MeshRenderer::Render()
	{
		__super::Render();

		graphics::MeshObject* _meshObj = reinterpret_cast<graphics::MeshObject*>(m_pRenderObject);

		GetGameObject().lock()->GetComponent<Transform>();

		/*auto _meshFilter = GetGameObject().lock()->GetComponent<MeshFilter>();

		if(_meshFilter == nullptr)
			return;

		auto _mesh = _meshFilter->GetMesh().lock();

		if(_mesh == nullptr)
			return;

		_meshObj->SetMeshBuffer(_mesh->GetMeshBuffer());

		vector<graphics::MaterialBuffer*> _mats;

		for (auto& _mat : m_materials)
		{
			auto _material = _mat.lock();

			if(_material == nullptr)
				return;

			_mats.push_back(_material->GetMaterialBuffer());
		}

		_meshObj->SetMaterialBuffers(_mats);*/
	}

	void MeshRenderer::Awake()
	{
		m_pRenderObject = (graphics::MeshObject*)graphics::RenderQueue::GetInstance()->CreateRenderObject(graphics::RenderType::MESH, GetUUID());
		
		// 임시
		m_pRenderObject->SetEnable(true);
	}
}