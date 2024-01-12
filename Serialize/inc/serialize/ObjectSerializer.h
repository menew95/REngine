/**

	@file      ComponentSerializer.h
	@brief     component serializer class
	@details   컴포넌트를 직렬화 역질렬화
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

		rengine::Object* DeSerialize(pair<const string, boost::property_tree::ptree>& node);

		rengine::Object* DeSerialize(tstring& path);
	};
}