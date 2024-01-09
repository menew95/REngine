#pragma once

#include <rengine\core\object\object.h>

#include <common\UUIDGenerator.h>

namespace rengine
{
	class Object::Impl
	{
	public:
		Impl()
		: m_uuid(UUIDGenerator::Generate())
		{
			
		}

		Impl(uuid uuid) 
		: m_uuid(uuid)
		{
		}

		Impl(uuid uuid, tstring name)
		: m_uuid(uuid)
		, m_objectName(name)
		{
		}

		Impl(uuid uuid, tstring name, tstring type)
		: m_uuid(uuid)
		, m_objectName(name)
		, m_typeName(type)
		{
		}

		Impl(const Impl& object) = default;

		Impl(Impl&& object) = default;

	private:
		uuid m_uuid;

		tstring m_objectName;

		tstring m_typeName;

	public:
		inline uuid GetUUID() { return m_uuid; }
		inline void SetUUID(uuid uuid) { m_uuid = uuid; }

		inline tstring GetName()			{ return m_objectName; }
		inline void SetName(tstring val)	{ m_objectName = val; }
		
		inline tstring GetType()			{ return m_typeName; }
		inline void SetType(tstring val)	{ m_typeName = val; }
	};
}