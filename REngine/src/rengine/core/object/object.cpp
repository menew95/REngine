#include <rengine/core/object/object.h>

namespace rengine
{
	Object::Object()
	{
	}
	Object::Object(uuid uuid)
	: m_uuid(uuid)
	{

	}
	Object::Object(uuid uuid, tstring name)
	: m_uuid(uuid)
	, m_objectName(name)
	{
	}
	Object::Object(uuid uuid, tstring name, tstring type)
	: m_uuid(uuid)
	, m_objectName(name)
	, m_typeName(type)
	{
	}
}
