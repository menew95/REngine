#pragma once

#if defined(_MSC_VER) && defined(GRAPHICS_CORE_EXPORT)
#ifndef GRAPHICS_API
#define GRAPHICS_API __declspec(dllexport)
//#define GRAPHICS_DLL_DECLSPEC 
#endif
#else
#ifndef GRAPHICS_API
#define GRAPHICS_API dll_import
//#define GRAPHICS_DLL_DECLSPEC 
#endif
#endif