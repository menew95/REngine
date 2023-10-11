#include "dx11_module_pch.h"

#include <graphics_module/Module.h>

#include <dx11_module/DX11RenderSystem.h>

static std::shared_ptr<graphics::DX11::DX11RenderSystem> g_DX11RenderSystem = nullptr;

/*namespace graphics
{

    int GetRendererID()
    {
        return RendererID::Direct3D11;
    }

    const char* GetModuleName()
    {
        return "Direct3D11";
    }

    const char* GetRendererName()
    {
        return "Direct3D 11";
    }

    RenderSystem* AllocRenderSystem(const graphics::RenderSystemDesc* renderSystemDesc)
    {
        g_DX11RenderSystem = std::make_shared<DX11::DX11RenderSystem>();

        return reinterpret_cast<RenderSystem*>(g_DX11RenderSystem.get());
    }

    bool FreeRenderSystem(RenderSystem* renderSystem)
    {
        delete renderSystem;
    }
}*/

extern "C"
{
    GRAPHICS_DLL_DECLSPEC int GetRendererID()
    {
        return graphics::RendererID::Direct3D11;
    }

    GRAPHICS_DLL_DECLSPEC const char* GetModuleName()
    {
        return "Direct3D11";
    }

    GRAPHICS_DLL_DECLSPEC const char* GetRendererName()
    {
        return "Direct3D 11";
    }

    GRAPHICS_DLL_DECLSPEC void* AllocRenderSystem(const void* renderSystemDesc, int renderSystemDescSize)
    {
        g_DX11RenderSystem = std::make_shared<graphics::DX11::DX11RenderSystem>();

        return reinterpret_cast<void*>(g_DX11RenderSystem.get());
    }

    GRAPHICS_DLL_DECLSPEC bool FreeRenderSystem(void* renderSystem)
    {
        return false;
    }
}