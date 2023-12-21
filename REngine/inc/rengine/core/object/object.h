#pragma once

#include <common\common.h>

#include <rengine/rengine_dllexport.h>

namespace rengine
{
	class RENGINE_API Object
	{
		class Impl;
	public:
		Object(); 

		Object(uuid uuid);

		Object(uuid uuid, tstring name);

		Object(uuid uuid, tstring name, tstring type);

		Object(const Object& object) = default;

		Object(Object&& object) = default;

		virtual ~Object();

		uuid GetUUID();

		void SetUUID(uuid uuid);

		tstring GetName();

		string GetNameStr();

		void SetName(tstring val);

		void SetNameStr(string val);

		tstring GetType();

	private:
		class Impl* m_pImpl;
	};
}