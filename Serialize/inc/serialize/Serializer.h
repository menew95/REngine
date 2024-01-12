#pragma once

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
		SERIALIZE_API Serializer(rengine::Object* object);

		SERIALIZE_API ~Serializer();

		SERIALIZE_API bool Serialize(rengine::Scene* scene);

		SERIALIZE_API static bool Serialize(rengine::Object* object);
		/*template<class T>
		static bool Serialize(rengine::GameObject* object, T& pt);*/

		SERIALIZE_API static rengine::Object* DeSerialize(tstring path);

	private:
		rengine::Object* m_pObject;
	};

}