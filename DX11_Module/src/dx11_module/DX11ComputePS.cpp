#include "dx11_module_pch.h"

#include "dx11_module\DX11ComputePS.h"
#include "dx11_module\DX11Shader.h"
#include "dx11_module\DX11StateManager.h"

namespace graphics
{
	namespace DX11
	{

		DX11ComputePS::DX11ComputePS(ID3D11Device* device, const ComputePipelineDesc& desc)
		{
			if (desc._shaderProgram._computeShader != nullptr)
			{
				auto _castShader = reinterpret_cast<DX11Shader*>(desc._shaderProgram._computeShader);
				m_CS = _castShader->GetNativeShader()._computeShader;
			}
			else
			{
				assert(false);
			}
		}

		void DX11ComputePS::Bind(DX11StateManager& stateMngr)
		{
			stateMngr.SetComputeShader(m_CS.Get());
		}

		void DX11ComputePS::SetShaderObjects(const ShaderProgram& shaderProgram)
		{
			if (shaderProgram._computeShader != nullptr)
			{
				auto _castShader = reinterpret_cast<DX11Shader*>(shaderProgram._computeShader);
				m_CS = _castShader->GetNativeShader()._computeShader;
			}
			else
			{
				assert(false);
			}
		}

	}
}