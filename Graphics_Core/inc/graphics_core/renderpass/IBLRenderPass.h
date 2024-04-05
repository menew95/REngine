/**

	@file      IBLRenderPass.cpp
	@brief
	@details   ~
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
	class MeshBuffer;

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

		void CreatePreFilteredMap();

		void CreateIrradianceMap();

		void CreateIntegrateBRDFMap();

	private:
		MaterialBuffer* m_integrateBRDFMap = nullptr;
		MaterialBuffer* m_preFilteredMap = nullptr;
		MaterialBuffer* mirradianceMap = nullptr;
	};
}