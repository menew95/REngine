#include <rengine\core\component\MeshFilter.h>

#include <rengine\core\resource\Mesh.h>

#include <rttr\registration.h>


RTTR_REGISTRATION
{
	rttr::registration::class_<rengine::MeshFilter>("MeshFilter")
	.constructor<const uuid&>()
	.property("Mesh", &rengine::MeshFilter::GetMesh, &rengine::MeshFilter::SetMesh)
	(
		rttr::metadata(rengine::MetaData::Serializable, rengine::MetaDataType::UUID)
	)
	;
}

namespace rengine
{
	MeshFilter::MeshFilter(const uuid& uuid)
		: Component(uuid, TEXT("MeshFilter"))
	{

	}

	MeshFilter::~MeshFilter()
	{

	}
}