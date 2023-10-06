#pragma once
#ifdef GRAPHICS_EXPORT
#ifndef GRAPHICS_API
#define GRAPHICS_API __declspec(dllexport)
#endif
#else
#ifndef GRAPHICS_API
#define GRAPHICS_API __declspec(dllimport)
#endif
#endif