#include <rengine\core\resource\Resource.h>

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