#pragma once

#include <graphics_module/RenderSystemObject.h>
#include <graphics_module/ResourceFlags.h>

namespace graphics
{
    class GRAPHICS_DLL_DECLSPEC Resource : public RenderSystemObject
    {
    public:
        Resource() = default;

        virtual ResourceType GetResourceType() const = 0;
    };


}