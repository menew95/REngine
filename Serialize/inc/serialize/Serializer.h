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
		SERIALIZE_API Serializer();

		SERIALIZE_API ~Serializer();

		/**
			@brief  해당 경로에 오브젝트의 종류(확장자)에 따른 .meta 파일 생성
			@param  path   - 파일 위치
			@param  object - 해당 파일에 해당하는 객체
			@retval        - .meta 파일 생성 여부
		**/
		SERIALIZE_API static bool CreateMetaInfo(const tstring& path, rengine::Object* object);

		/**
		    @brief  오브젝트를 직렬화
		    @param  path   - 직렬화된 파일의 위치
		    @param  object - 직렬화할 오브젝트
		    @retval        - 직렬화 성공 여부
		**/
		SERIALIZE_API static bool Serialize(const tstring& path, rengine::Object* object);

		/**
		    @brief  파일 경로에 해당하는 파일의 메타파일를 읽고 역직렬화하여 해당 오브젝트를 생성
		    @param  path - 역직렬화할 파일 경로
		    @retval      - 생성된 오브젝트
		**/
		SERIALIZE_API static shared_ptr<rengine::Object> DeSerialize(tstring path);
	};

}