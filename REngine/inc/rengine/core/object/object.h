#pragma once

#include <common\common.h>

#include <rengine/rengine_dllexport.h>

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
		inline RENGINE_API uuid GetUUID() { return m_uuid; }
		inline RENGINE_API void SetUUID(uuid uuid) { m_uuid = uuid; }
		
		inline RENGINE_API tstring GetName() { return m_objectName; }
		inline RENGINE_API string GetNameStr() { return StringHelper::WStringToString(m_objectName.c_str()); }
		inline RENGINE_API void SetName(tstring val) { m_objectName = val; }
	
		inline RENGINE_API tstring GetType() { return m_typeName; }
	};
}