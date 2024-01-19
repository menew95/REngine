#include <rengine\core\resource\Resource.h>

#include <rttr\registration.h>

//RTTR_REGISTRATION
//{
//	rttr::registration::class_<rengine::Resource>("Resource")
//	.constructor<tstring, tstring>()
//	;
//}

namespace rengine
{
	Resource::Resource(uuid uuid, tstring type)
	: Object(uuid, type, type)
	{

	}
}