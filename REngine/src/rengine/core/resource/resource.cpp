#include <rengine\core\resource\Resource.h>

namespace rengine
{
	Resource::Resource(uuid uuid, tstring type)
	: Object(uuid, type, type)
	{

	}
}