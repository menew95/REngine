#pragma once
#ifdef SERIALIZE_EXPORTS
#define SERIALIZE_API __declspec(dllexport)
#else
#define SERIALIZE_API __declspec(dllimport)
#endif