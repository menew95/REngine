#pragma once

#include "graphics_module\PipelineState.h"
#include "graphics_module\Common\ComPtr.h"
#include "dx11_module\Direct3D11.h"
#include "graphics_module\ShaderFlags.h"
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

			/**
				@brief 머티리얼한테 파이프라인 리소스 레이아웃을 알려주기 위해 사용
			**/
			struct PropertyDesc& GetPropertyDesc() override
			{
				return m_PropertyDesc;
			}

		private:
			/**
				@brief 머티리얼한테 파이프라인 리소스 레이아웃을 알려주기 위해 사용
			**/
			void PipelineReflect(const vector<class DX11Shader*>& shaders);

			ComPtr<ID3D11ComputeShader> m_CS;

			PropertyDesc m_PropertyDesc;
		};
	}
}