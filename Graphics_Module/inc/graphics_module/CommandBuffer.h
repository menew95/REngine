﻿#pragma once

#include "common\common.h"

#include "graphics_module\RenderSystemObject.h"

#include "graphics_module\CommandBufferFlags.h"
#include "graphics_module\ShaderFlags.h"
#include "graphics_module\ResourceFlags.h"

namespace graphics
{
	class Buffer;
	class Texture;
	class Sampler;
	class ResourceView;
	class PipelineState;
	class PipelineLayout;
	class Resource;
	class RenderPass;
	class RenderTarget;
	struct TextureSubresource;
	struct TextureLocation;

	class GRAPHICS_DLL_DECLSPEC CommandBuffer : public RenderSystemObject
	{
	public:
		CommandBuffer() = default;
		virtual ~CommandBuffer() = default;

		virtual void Begin() abstract;
		virtual void BeginEvent(const wchar_t* event) abstract;
		virtual void End() abstract;
		virtual void EndEvent() abstract;
		virtual void Execute(CommandBuffer& deferredCommandBuffer) abstract;

		virtual void CopyTexture(
			Texture& dstTexture,
			const TextureLocation& dstLocation,
			Texture& srcTexture,
			const TextureLocation& srcLocation,
			const Extent3D& extent
		) abstract;

		/* ----- Buffer ----- */
		virtual void UpdateBuffer(Buffer& destBuffer, uint32 destBufferOffset, const void* data, uint32 dataSize) abstract;

		virtual void CopyBuffer(Buffer& dstBuffer, uint32 dstOffset, Buffer& srcBuffer, uint32 srcOffset, uint32 size) abstract;

		virtual void GenerateMips(Texture& texture) abstract;
		virtual void GenerateMips(Texture& texture, const TextureSubresource& _subresource) abstract;

		/* ----- Viewport and Scissor ----- */

		virtual void SetViewport(const math::Viewport& viewport) abstract;
		virtual void SetViewports(uint32 numViewports, const math::Viewport* viewport) abstract;

		virtual void SetScissor(const math::Scissor& scissor) abstract;
		virtual void SetScissors(uint32 numScissors, const math::Scissor* scissors) abstract;

		/* ----- Input Assembly ------ */

		virtual void SetVertexBuffer(Buffer& buffer) abstract;

		virtual void SetVertexBuffer(Buffer** buffer) abstract;

		virtual void SetIndexBuffer(Buffer& buffer) abstract;

		/* ----- Resources ----- */

		virtual void SetResource(Resource& resource, uint32 slot, long bindFlags, long stageFlags = StageFlags::AllStages) abstract;
		
		virtual void SetResources(PipelineLayout& pipelineLayout) abstract;
		
		virtual void SetResourceView(ResourceView& resource, uint32 slot, uint32 count, long bindFlags, long stageFlags = StageFlags::AllStages) abstract;

		virtual void ResetResourceSlots(const ResourceType resourceType, uint32 firstSlot, uint32 numSlots, long bindFlags, long stageFlags = StageFlags::AllStages) abstract;
	
		/* ----- Render Passes ----- */

		virtual void BeginRenderPass(
			RenderPass& renderPass,
			std::uint32_t       numClearValues,
			const ClearValue* clearValues
		) abstract;

		virtual void EndRenderPass() abstract;

		virtual void Clear(long flags, const ClearValue& clearValue = {}) abstract;
		virtual void ClearAttachments(uint32 numAttachments, const AttachmentClear* attachments) abstract;
		
		virtual void ClearState() abstract;

		/* ----- Render Target ----- */
		virtual void SetRenderTarget(RenderTarget& renderTarget, uint32 numAttachments, const AttachmentClear* attachments) abstract;

		/* ----- Pipeline States ----- */

		virtual void SetPipelineState(PipelineState& pipelineState) abstract;

		/* ----- Drawing ----- */

		virtual void Draw(uint32 numVertices, uint32 firstVertex) abstract;

		virtual void DrawIndexed(uint32 numIndices, uint32 firstIndex, uint32 _offset = 0) abstract;

		virtual void DrawInstanced(uint32 numVertices, uint32 firstVertex, uint32 numInstances, uint32 firstInstance = 0) abstract;

		virtual void DrawIndexedInstanced(uint32 numIndices, uint32 numInstances, uint32 firstIndex, int32 vertexOffset = 0) abstract;

		/* ----- Compute ----- */

		virtual void Dispatch(uint32 numWorkGroupsX, uint32 numWorkGroupsY, uint32 numWorkGroupsZ) abstract;
		virtual void DispatchIndirect(Buffer& buffer, uint32 _offset) abstract;
	};

}