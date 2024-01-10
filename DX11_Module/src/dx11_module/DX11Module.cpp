#include "dx11_module_pch.h"

#include <graphics_module/Module.h>

#include <dx11_module/DX11RenderSystem.h>

static std::shared_ptr<graphics::DX11::DX11RenderSystem> g_DX11RenderSystem = nullptr;

extern "C"
{
    GRAPHICS_DLL_DECLSPEC int RenderSystem_BuildID()
    {
        assert(false);

        return 0;
    }

    GRAPHICS_DLL_DECLSPEC int RenderSystem_RendererID()
    {

        return graphics::RendererID::Direct3D11;
    }

    GRAPHICS_DLL_DECLSPEC const char* RenderSystem_Name()
    {
        return "Direct3D 11";
    }

    GRAPHICS_DLL_DECLSPEC void* RenderSystem_Alloc(const void* renderSystemDesc, int renderSystemDescSize)
    {
        g_DX11RenderSystem = std::make_shared<graphics::DX11::DX11RenderSystem>();

        return reinterpret_cast<void*>(g_DX11RenderSystem.get());
    }

    GRAPHICS_DLL_DECLSPEC void RenderSystem_Free(void* renderSystem)
    {

    }
}