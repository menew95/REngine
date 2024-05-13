#include <rengine\core\resource\Prefab.h>

namespace rengine
{
	Prefab::Prefab(const uuid& uuid)
	: Resource(uuid, TEXT("Prefab"))
	{
	}
}