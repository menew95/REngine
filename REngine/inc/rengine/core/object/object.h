#pragma once

#include <common\common.h>

namespace rengine
{
	class Object
	{
	public:
		Object(); 

		Object(uuid uuid);

		Object(uuid uuid, tstring name);

		Object(uuid uuid, tstring name, tstring type);

		Object(const Object& object) = default;

		Object(Object&& object) = default;

	private:
		uuid m_uuid;

		tstring m_objectName;

		tstring m_typeName;

	public:
		inline uuid GetUUID() { return m_uuid; }
		inline void SetUUID(uuid uuid) { m_uuid = uuid; }
		
		inline tstring GetName() { return m_objectName; }
		inline tstring SetName(tstring val) { m_objectName = val; }
	
		inline tstring GetType() { return m_typeName; }
	};
}