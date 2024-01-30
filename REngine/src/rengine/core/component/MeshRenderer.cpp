#include <rengine\core\component\MeshRenderer.h>

#include <rttr\registration.h>

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
}