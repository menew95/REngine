/**

	@file      ResourceSerializer.h
	@brief     resource serializer class
	@details   resource를 직렬화 역질렬화
	@author    LWT
	@date      18.01.2024
	@copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <rengine\core\object\object.h>

#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\json_parser.hpp>

namespace utility
{
	class ResourceSerializer
	{
	public:
		ResourceSerializer() = default;

		~ResourceSerializer() = default;

		static void Serialize(rengine::Object* object, boost::property_tree::ptree& pt);

		static std::shared_ptr<rengine::Object> DeSerialize(const tstring& path, const rengine::MetaInfo& metaInfo, const boost::property_tree::ptree& pt);
	};
}