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

	void MeshRenderer::Awake()
	{
		m_pRenderObject = (graphics::MeshObject*)graphics::RenderQueue::GetInstance()->CreateRenderObject(graphics::RenderType::MESH, GetUUID());
	}

	void MeshRenderer::Render()
	{
		auto _trans = m_pGameObject.lock()->GetTransform();

		if (_trans == nullptr)
			return;

		m_pRenderObject->SetWorld(_trans->GetWorld());
	}
}