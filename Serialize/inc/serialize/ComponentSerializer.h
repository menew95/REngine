/**

    @file      ComponentSerializer.h
    @brief     component serializer class
    @details   컴포넌트를 직렬화 역질렬화
    @author    LWT
    @date      11.01.2024
    @copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once


#include <rengine\core\component\Component.h>

#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\json_parser.hpp>

namespace utility
{
	class ComponentSerializer
	{
	public:
		ComponentSerializer(rengine::Component* object);

		~ComponentSerializer();

		void Serialize(boost::property_tree::ptree& pt);

		rengine::Component* DeSerialize(tstring& path);

	private:
		rengine::Component* m_pObject;
	};
}