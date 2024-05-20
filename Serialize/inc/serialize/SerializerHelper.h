/**

    @file      SerializerHelper.h
    @brief     serializer helper
    @details   직렬화 열직렬화 헬퍼 함수
    @author    LWT
    @date      11.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <common\math.h>

#include <rttr\type.h>

#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\json_parser.hpp>

#include <rengine\core\object\object.h>

namespace utility
{
	template<typename T>
	void serializeConfig(const T&, std::string&, boost::property_tree::ptree& pt)
	{
		//static_assert(false, "need template specialization for T");
	}
	
	template<>
	void serializeConfig(const math::Vector2& v, std::string& valueName, boost::property_tree::ptree& pt);

	template<>
	void serializeConfig(const math::Vector3& v, std::string& valueName, boost::property_tree::ptree& pt);

	template<>
	void serializeConfig(const math::Vector4& v, std::string& valueName, boost::property_tree::ptree& pt);

	template<>
	void serializeConfig(const math::Color& m, std::string& valueName, boost::property_tree::ptree& pt);
	
	template<>
	void serializeConfig(const math::Matrix& m, std::string& valueName, boost::property_tree::ptree& pt);

	template<typename T>
	T parseConfig(const boost::property_tree::ptree& pt)
	{
		//static_assert(false, "need template specialization for T");
	}

	template<>
	math::Vector2 parseConfig(const boost::property_tree::ptree& pt);

	template<>
	math::Vector3 parseConfig(const boost::property_tree::ptree& pt);

	template<>
	math::Vector4 parseConfig(const boost::property_tree::ptree& pt);

	template<>
	math::Color parseConfig(const boost::property_tree::ptree& pt);

	template<>
	math::Matrix parseConfig(const boost::property_tree::ptree& pt);

	void GetProperty(boost::property_tree::ptree& pt, const rttr::property& property, const rttr::instance& object);

	void SetProperty(const boost::property_tree::ptree& pt, const rttr::property& property, rttr::variant& object);
}