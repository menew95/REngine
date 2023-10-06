#ifdef RENGINE_EXPORT
#ifndef RENGINE_API
#define RENGINE_API __declspec(dllexport)
#endif
#else
#ifndef RENGINE_API
#define RENGINE_API __declspec(dllimport)
#endif
#endif