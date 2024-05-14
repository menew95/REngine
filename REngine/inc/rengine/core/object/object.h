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
		Object(const uuid& uuid, const tstring& name, const tstring& type);

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
		
		static void Destroy(Object* obj, float t = 0.f);

		static void DestroyImmediate(Object* obj);


	protected:
		/**
		    @brief 오브젝트가 파괴 되기전에 해야할 작업을 하는 함수
		**/
		virtual void PreDestroy() {};

	private:
		class Impl* m_pImpl;


		friend class ObjectFactory;

		RTTR_ENABLE()

		RTTR_REGISTRATION_FRIEND
	};
}