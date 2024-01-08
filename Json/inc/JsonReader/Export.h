#pragma once

#if defined(_MSC_VER) && defined(JSON_EXPORT)
#ifndef JSON_API
#define JSON_API __declspec(dllexport)
#endif
#else
#ifndef JSON_API
#define JSON_API dll_import
#endif
#endif