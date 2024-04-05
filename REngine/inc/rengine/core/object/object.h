#pragma once

#include <common\common.h>

#include <rengine\core\object\MetaInfo.h>

#include <rengine/rengine_dllexport.h>

#include <rttr\registration_friend.h>
#include <rttr\rttr_enable.h>

namespace rengine
{
	class RENGINE_API Object
	{
		class Impl;

	public:
		Object(uuid uuid, tstring name, tstring type);

		Object(const Object& object) = default;

		Object(Object&& object) = default;

		virtual ~Object();

		uuid GetUUID();
		string GetUUIDStr();
		void SetUUID(uuid uuid);
		void SetUUIDStr(string uuid);

		inline uint64 GetHash();

		tstring GetName();
		string GetNameStr();
		void SetName(tstring val);
		void SetNameStr(string val);

		tstring GetType();
		string GetTypeStr();
		void SetType(tstring val);
		void SetTypeStr(string val);
		
		static void Destroy(shared_ptr<Object> obj, float t = 0.f);

	private:
		class Impl* m_pImpl;

		RTTR_ENABLE()

		RTTR_REGISTRATION_FRIEND
	};
}