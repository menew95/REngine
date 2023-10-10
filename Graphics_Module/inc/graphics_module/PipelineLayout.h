#pragma once

#include <graphics_module/graphics_module_dllexport.h>

#include "graphics_module\PipelineLayoutFlags.h"

namespace graphics
{
	class Buffer;
	class Texture;
	class Sampler;
	class ResourceView;

	class GRAPHICS_DLL_DECLSPEC PipelineLayout
	{
	public:
		virtual void SetResources(std::vector<void*>& resources) abstract;
		virtual void SetResource(uint32 index, void* resource) abstract;
		virtual uint32 GetNumBindings() const abstract;
		virtual void* GetResource(uint32 index) abstract;
		virtual Buffer* GetBuffer(uint32 index) abstract;
		virtual Texture* GetTexture(uint32 index) abstract;
		virtual Sampler* GetSampler(uint32 index) abstract;
		virtual ResourceView* GetResourceView(uint32 index) abstract;
	};
}