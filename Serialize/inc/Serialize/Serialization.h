#pragma once

#include <common\common.h>

#include <common\math.h>

#include <boost/property_tree/ptree.hpp>

namespace rengine
{
	class Object;
	class GameObject;
	class Scene;
}

namespace utility
{
	class Serialization
	{
	public:

		static bool Serialize(rengine::Scene* scene);

		/*template<class T>
		static bool Serialize(rengine::GameObject* object, T& pt);*/

		static rengine::Object* DeSerialize(tstring& path);
	};

	template<typename T>
	void serializeConfig(T&, std::string&, boost::property_tree::ptree& pt)
	{
		//static_assert(false, "need template specialization for T");
	}

}