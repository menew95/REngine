﻿#pragma once

#include <common\common.h>

#include <common\math.h>

#include <serialize\Serialize_dllexport.h>

namespace rengine
{
	class Object;
	class GameObject;
	class Scene;
}

namespace utility
{
	class Serializer
	{
	public:
		SERIALIZE_API Serializer();

		SERIALIZE_API ~Serializer();

		//SERIALIZE_API static bool Serialize(rengine::Scene* scene);

		SERIALIZE_API static bool CreateMetaInfo(const tstring& path, rengine::Object* object);

		SERIALIZE_API static bool Serialize(const tstring& path, rengine::Object* object);
		/*template<class T>
		static bool Serialize(rengine::GameObject* object, T& pt);*/

		SERIALIZE_API static shared_ptr<rengine::Object> DeSerialize(tstring path);
	};

}