#pragma once

#include <common\common.h>

#include <jsonReader\Export.h>
#include <jsonReader\JsonMacro.h>

#include <rapidjson\document.h>

namespace utility
{
	using JsonUnicode = rapidjson::UTF8<TCHAR>;
	using TDocument = rapidjson::GenericDocument<JsonUnicode>;

	class JsonReader
	{
	protected:
		JsonReader();
		~JsonReader() = default;

	public:
		JsonReader(const JsonReader& other) = delete;
		JsonReader& operator =(const JsonReader& other) = delete;

	public:
		struct JSON_API Deleter
		{
			void operator()(JsonReader* instance)
			{
				delete instance;
			}
		};

	private:
		static std::unique_ptr<JsonReader, Deleter> _instance;

		static std::once_flag _flag;

		std::unordered_map<tstring, std::unique_ptr<TDocument>> _documents;

	public:
		JSON_API static  JsonReader* const GetInstance();

		JSON_API static void Release();

	public:
		JSON_API TDocument* LoadJson(const tstring& jsonPath);

		JSON_API void UnloadJson(const tstring& jsonPath);
		JSON_API bool WriteJson(const tstring& jsonPath, rapidjson::Document& document);
	};

}