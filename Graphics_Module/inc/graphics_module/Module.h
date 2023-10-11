#pragma once

#include <graphics_module\graphics_module_dllexport.h>

#ifdef __cplusplus
extern "C"
{
#endif

	GRAPHICS_DLL_DECLSPEC int RenderSystem_BuildID();

	GRAPHICS_DLL_DECLSPEC int RenderSystem_RendererID();

	GRAPHICS_DLL_DECLSPEC const char* RenderSystem_Name();

	GRAPHICS_DLL_DECLSPEC void* RenderSystem_Alloc(const void* renderSystemDesc, int renderSystemDescSize);

	GRAPHICS_DLL_DECLSPEC void RenderSystem_Free(void* renderSystem);

	using RenderSystemAlloc = void*(*)(const void*, int);
	using RenderSystemFree = void*(*)(void*);

#ifdef __cplusplus
} // /extern "C"
#endif