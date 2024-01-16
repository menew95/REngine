/**

	@file      ObjectSerializer.h
	@brief     object serializer class
	@details   오브젝트를 직렬화 역질렬화
	@author    LWT
	@date      11.01.2024
	@copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <rengine\core\object\object.h>

#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\json_parser.hpp>

namespace utility
{
	class ObjectSerializer
	{
	public:
		ObjectSerializer();

		~ObjectSerializer();

		void Serialize(rengine::Object* object, boost::property_tree::ptree& pt);

		shared_ptr<rengine::Object> DeSerialize(pair<const string, boost::property_tree::ptree>& node, std::shared_ptr<rengine::Object> object = nullptr);

		shared_ptr<rengine::Object> DeSerialize(tstring& path);
	};
}