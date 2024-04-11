/**

	@file      IBLRenderPass.cpp
	@brief	   ibl resource를 생성하는 패스
	@details   cube map texture로 부터 조도와 사전 필터링된 맵을 생성 및 brdf lookup table를 생성하는 패스
	@author    LWT
	@date      3.04.2024
	@copyright © Cool Guy, 2024. All right reserved.

**/
#pragma once

#include <graphics_core\RenderPass.h>

#define MAXMIPMAP 5

namespace graphics
{
	class CommandBuffer;

	class RenderTarget;
	class RenderPass;
	class ResourceManager;

	class MaterialBuffer;
	class TextureBuffer;
	class MeshBuffer;

	class Buffer;

	class IBLRenderPass : public RenderPass
	{
	public:
		IBLRenderPass();
		~IBLRenderPass();

		void Init() override;

		/**
			@brief pass에 설정 된 pipeline를 바인드
			@param command - command buffer ptr
		**/
		void Bind(class CommandBuffer* command) override;

		void BeginExcute(class CommandBuffer* command, class CameraBuffer* camBuffer) override;

		void Excute(class CommandBuffer* command) override;

		void EndExcute(class CommandBuffer* command) override;

		/**
		    @brief brdf look up table texture를 생성
		    @param command - command buffer
		**/
		void CreateIntegrateBRDFMap(CommandBuffer* command);

		/**
		    @brief 사전 필터링 된 환경 맵를 생성
		    @param command       - command buffer
		    @param textureBuffer - 큐브맵 텍스처 리소스
		**/
		void CreatePreFilteredMap(CommandBuffer* command, TextureBuffer* textureBuffer);
		
		/**
		    @brief 사전 필터링 된 환경 맵를 생성
		    @param command       - command buffer
		    @param textureBuffer - 큐브맵 텍스처 리소스
		    @param renderTarget  - 생성할 큐브맵 렌더 타겟
		**/
		//void CreatePreFilteredMapToRenderTarget(CommandBuffer* command, TextureBuffer* textureBuffer, RenderTarget* renderTarget);

		/**
			@brief조도 맵을 생성
			@param command       - command buffer
			@param textureBuffer - 큐브맵 텍스처 리소스
		**/
		void CreateIrradianceMap(CommandBuffer* command, TextureBuffer* textureBuffer);
		
		/**
			@brief 조도 맵을 생성
			@param command       - command buffer
			@param textureBuffer - 큐브맵 텍스처 리소스
			@param renderTarget  - 생성할 큐브맵 렌더 타겟
		**/
		//void CreateIrradianceMapToRenderTarget(CommandBuffer* command, TextureBuffer* textureBuffer, RenderTarget* renderTarget);

	private:

		void CreatePreFilteredRenderTargetFromTexture(vector<RenderTarget*>& rtvs, class Texture* texture);

		void CreateIrradianceRenderTargetFromTexture(vector<RenderTarget*>& rtvs, class Texture* texture);

		Buffer* m_pBuffer = nullptr;
		
		MeshBuffer* m_pQuardMeshBuffer = nullptr;
		MeshBuffer* m_pCubeMeshBuffer = nullptr;

		RenderTarget* m_integrateBRDF_RT = nullptr;
		RenderTarget* m_preFiltered_RT = nullptr;
		RenderTarget* m_irradiance_RT = nullptr;

		MaterialBuffer* m_integrateBRDF_Material = nullptr;
		MaterialBuffer* m_preFiltered_Material = nullptr;
		MaterialBuffer* m_irradiance_Material = nullptr;
	};
}