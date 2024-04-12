#pragma once

#include "graphics_module\Helper.h"

#include "graphics_module\Buffer.h"
#include "graphics_module\Texture.h"
#include "graphics_module\Sampler.h"
#include "graphics_module\CommandBuffer.h"
#include "graphics_module\ResourceFlags.h"
#include "graphics_module\RenderSystemFlags.h"
#include "graphics_module\ImageFlags.h"

#include "graphics_module\Shader.h"
#include "graphics_module\SwapChain.h"
#include "graphics_module\ResourceView.h"
#include "graphics_module\RenderTarget.h"
#include "graphics_module\PipelineLayout.h"
#include "graphics_module\PipelineState.h"

#include "graphics_module\VideoAdapter.h"

namespace graphics
{
	class GRAPHICS_DLL_DECLSPEC RenderSystem
	{
	public:
		RenderSystem() = default;
		virtual ~RenderSystem() = default;

		/*static std::vector<std::string> FindModules();
		static std::unique_ptr<RenderSystem> Load(RenderSystemDesc& desc);
		static void Unload(std::unique_ptr<RenderSystem>&& renderSystem);*/

		virtual void* GetDevice() abstract;
		virtual void* GetDeviceContext() abstract;

		/* ----- SwapChain ----- */
		virtual SwapChain* CreateSwapChain(uuid uuid, const SwapChainDesc& desc) abstract;
		virtual void Release(SwapChain& swapChain) abstract;

		/* ----- Command Buffer ----- */
		virtual CommandBuffer* CreateCommandBuffer(uuid uuid, const CommandBufferDesc& desc) abstract;
		virtual void Release(CommandBuffer& commandBuffer) abstract;

		/* ----- Buffer ----- */
		virtual Buffer* CreateBuffer(uuid uuid, const BufferDesc& desc, const void* initData = nullptr) abstract;
		virtual void Release(Buffer& buffer) abstract;

		virtual void WriteBuffer(Buffer& buffer, const void* data, uint32 dataSize, uint32 offset) abstract;
		virtual void ReadBuffer(Buffer& buffer, void* data, uint32 dataSize, uint32 offset) abstract;

		virtual void* MapBuffer(Buffer& buffer, const CPUAccess access) abstract;
		virtual void* MapBuffer(Buffer& buffer, const CPUAccess access, uint32 offset, uint32 length) abstract;
		virtual void UnmapBuffer(Buffer& buffer) abstract;

		/* ----- Textures ----- */
		virtual Texture* CreateTexture(uuid uuid, const TextureDesc& desc, const struct ImageDesc* imageDesc = nullptr) abstract;
		virtual void Release(Texture& texture) abstract;

		virtual void WriteTexture(Texture& texture, const TextureRegion& textureRegion, const ImageDesc& imageDesc) abstract;
		virtual void ReadTexture(Texture& texture, const TextureRegion& textureRegion, struct CopyImageView& copyImageView) abstract;

		/* ----- Sampler States ---- */
		virtual Sampler* CreateSampler(uuid uuid, const SamplerDesc& desc) abstract;
		virtual void Release(Sampler& sampler) abstract;

		/* ----- Resource Views ---- */
		virtual ResourceView* CreateResoureView(uuid uuid, const ResourceViewDesc& desc) abstract;
		virtual void Release(ResourceView& resourceView) abstract;

		///* ----- Render Passes ----- */
		//virtual RenderPass* CreateRenderPass(uuid uuid, const RenderPassDesc& desc) abstract;
		//virtual void Release(RenderPass& renderPass) abstract;

		/* ----- Render Targets ----- */
		virtual RenderTarget* CreateRenderTarget(uuid uuid, const RenderTargetDesc& desc) abstract;
		virtual void Release(RenderTarget& renderTarget) abstract;

		/* ----- Shader ----- */
		virtual Shader* CreateShader(uuid uuid, const ShaderDesc& desc) abstract;
		virtual void Release(Shader& shader) abstract;

		/* ----- Pipeline Layouts ----- */
		virtual PipelineLayout* CreatePipelineLayout(uuid uuid, const PipelineLayoutDesc& desc) abstract;
		virtual void Release(PipelineLayout& pipelineLayout) abstract;

		/* ----- Pipeline States ----- */
		virtual PipelineState* CreatePipelineState(uuid uuid, const GraphicsPipelineDesc& desc) abstract;
		virtual PipelineState* CreatePipelineState(uuid uuid, const ComputePipelineDesc& desc) abstract;
		virtual void Release(PipelineState& pipelineState) abstract;

	};
}