#pragma once

#include <graphics_module/Resource.h>
#include <graphics_module/BufferFlags.h>

namespace graphics
{
	class GRAPHICS_DLL_DECLSPEC Buffer : public Resource
	{
    public:
        Buffer(uint32 bindFlags);
        virtual ~Buffer() = default;

        ResourceType GetResourceType() const override final;

        inline long GetBindFlags() const
        {
            return m_BindFlags;
        }

        
    protected:

        Buffer(long bindFlags);

    private:

        long m_BindFlags = 0;
	};
}