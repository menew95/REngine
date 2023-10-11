#pragma once

#if defined(_MSC_VER) && defined(GRAPHICS_MODULE_EXPORT)
#ifndef GRAPHICS_DLL_DECLSPEC
#define GRAPHICS_DLL_DECLSPEC __declspec(dllexport)
//#define GRAPHICS_DLL_DECLSPEC 
#endif
#else
#ifndef GRAPHICS_DLL_DECLSPEC
#define GRAPHICS_DLL_DECLSPEC dll_import
//#define GRAPHICS_DLL_DECLSPEC 
#endif
#endif