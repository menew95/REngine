#include <rengine\core\resource\resource.h>

namespace rengine
{
	Resource::Resource(uuid uuid, tstring name)
	: Object(uuid, name, TEXT("Resource"))
	{

	}

	Resource::Resource(uuid uuid, tstring name, tstring type)
		: Object(uuid, name, type)
	{

	}
}