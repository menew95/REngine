#include <rengine\core\resource\Model.h>

namespace rengine
{
	Model::Model(const uuid& uuid)
		: Resource(uuid, TEXT("Model"))
	{
	}
}