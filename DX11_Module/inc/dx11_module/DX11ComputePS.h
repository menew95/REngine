#pragma once

#include "graphics_module\PipelineState.h"
#include "graphics_module\Common\ComPtr.h"
#include "dx11_module\Direct3D11.h"

namespace graphics
{
	namespace DX11
	{

		class DX11StateManager;

		class DX11ComputePS : public PipelineState
		{
		public:
			DX11ComputePS(ID3D11Device* device, const ComputePipelineDesc& desc);

			virtual void Bind(DX11StateManager& stateMngr);

			void SetShaderObjects(const ShaderProgram& shaderProgram) override;

		private:
			ComPtr<ID3D11ComputeShader> m_CS;
		};
	}
}