/**

    @file      GameObjectSerializer.h
    @brief     gameobject serializer class
    @details   게임 오브젝트를 직렬화 역질렬화 하는 클래스
    @author    LWT
    @date      11.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <rengine\core\object\GameObject.h>

#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\json_parser.hpp>

namespace utility
{
	class GameObjectSerializer
	{
	public:
		GameObjectSerializer();

		~GameObjectSerializer();

		void Serialize(rengine::GameObject* object, boost::property_tree::ptree& pt);

		rengine::GameObject* DeSerialize(tstring& path);
	};
}