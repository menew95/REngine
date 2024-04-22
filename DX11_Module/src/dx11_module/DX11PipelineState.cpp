#include "dx11_module_pch.h"

#include "graphics_module\PipelineStateUtilis.h"

#include "dx11_module\DX11PipeLineState.h"
#include "dx11_module\DX11PipelineLayout.h"
#include "dx11_module\DX11StateManager.h"
#include "dx11_module\DX11Type.h"
#include "dx11_module\DX11Shader.h"

namespace graphics
{
	namespace DX11
	{

		DX11PipelineState::DX11PipelineState(ID3D11Device* device, const GraphicsPipelineDesc& desc)
			: m_Viewports(desc._viewports)
			, m_Scissors(desc._scissors)
		{
			m_PrimitiveTopology = MapPrimitive(desc._primitiveTopology);

			m_StencilRefDynamic = false/*desc._stencilDesc.referenceDynamic*/;
			m_StencilRef = desc._stencilDesc._stencilRef;
			m_BlendFactorDynamic = desc._blendDesc._blendFactorDynamic;
			m_BlendFactor[0] = desc._blendDesc._blendFactor.R();
			m_BlendFactor[1] = desc._blendDesc._blendFactor.G();
			m_BlendFactor[2] = desc._blendDesc._blendFactor.B();
			m_BlendFactor[3] = desc._blendDesc._blendFactor.A();
			m_SampleMask = desc._blendDesc._sampleMask;

			if (desc._hasDSS == true)
			{
				CreateDepthStencilState(device, desc);
			}
			if (desc._hasRRS == true)
			{
				CreateRasterizerState(device, desc);
			}
			if (desc._hasBS == true)
			{
				CreateBlendState(device, desc);
			}

			SetShaderObjects(desc._shaderProgram);

			auto _shaders = GetShadersAsArray(desc);

			PipelineReflect(CastShaderArray<DX11Shader>(_shaders));
		}

		void DX11PipelineState::Bind(DX11StateManager& stateManager)
		{
			///* Bind rasterizer state */
			stateManager.SetRasterizerState(m_RasterizerState.Get());

			/* Bind depth-stencil state */
			if (IsStencilRefDynamic())
				stateManager.SetDepthStencilState(m_DepthStencilState.Get());
			else
				stateManager.SetDepthStencilState(m_DepthStencilState.Get(), GetStencilRef());

			/* Bind blend state */
			if (IsBlendFactorDynamic())
				stateManager.SetBlendState(m_BlendState.Get(), GetSampleMask());
			else
				stateManager.SetBlendState(m_BlendState.Get(), GetBlendFactor(), GetSampleMask());

			/* Set input-assembly states */
			stateManager.SetPrimitiveTopology(m_PrimitiveTopology);
			if(m_PrimitiveTopology == D3D11_PRIMITIVE_TOPOLOGY_POINTLIST)
				stateManager.SetInputLayout(nullptr);
			else
				stateManager.SetInputLayout(m_InputLayout.Get());

			/* Set shader states */
			stateManager.SetVertexShader(m_VS.Get());
			stateManager.SetHullShader(m_HS.Get());
			stateManager.SetDomainShader(m_DS.Get());
			stateManager.SetGeometryShader(m_GS.Get());
			stateManager.SetPixelShader(m_PS.Get());
			stateManager.SetComputeShader(m_CS.Get());

			/* Set static viewports and scissors */
			SetStaticViewportsAndScissors(stateManager);
		}

		void DX11PipelineState::SetStaticViewportsAndScissors(DX11StateManager& stateManager)
		{
			if (m_Viewports.size() > 0L)
			{
				stateManager.GetContext()->RSSetViewports(
					static_cast<uint32>(m_Viewports.size()),
					(D3D11_VIEWPORT*)m_Viewports.data()
				);
			}
			if (m_Scissors.size() > 0L)
			{
				stateManager.GetContext()->RSSetScissorRects(
					static_cast<uint32>(m_Scissors.size()),
					(D3D11_RECT*)m_Scissors.data()
				);
			}
		}

		void DX11PipelineState::CreateDepthStencilState(ID3D11Device* device, const GraphicsPipelineDesc& pipelineDesc)
		{
			D3D11_DEPTH_STENCIL_DESC _depthStencilDesc;

			GetDXDepthStencil(_depthStencilDesc, pipelineDesc._depthDesc, pipelineDesc._stencilDesc);

			auto _hr = device->CreateDepthStencilState(&_depthStencilDesc, m_DepthStencilState.GetAddressOf());
			HR(_hr, "failed to create depthstencil state");
		}

		void DX11PipelineState::CreateRasterizerState(ID3D11Device* device, const GraphicsPipelineDesc& desc)
		{
			D3D11_RASTERIZER_DESC _rasterizerDesc;

			GetDXRasterizer(_rasterizerDesc, desc._rasterizerDesc);

			auto _hr = device->CreateRasterizerState(&_rasterizerDesc, m_RasterizerState.GetAddressOf());
			HR(_hr, "failed to create rasterizer state");
		}

		void DX11PipelineState::CreateBlendState(ID3D11Device* device, const GraphicsPipelineDesc& desc)
		{
			D3D11_BLEND_DESC _blendDesc;

			GetDXBlend(_blendDesc, desc._blendDesc);

			auto _hr = device->CreateBlendState(&_blendDesc, m_BlendState.GetAddressOf());
			HR(_hr, "failed to create blend state");
		}

		void DX11PipelineState::SetShaderObjects(const ShaderProgram& shaderProgram)
		{
			if (shaderProgram._vertexShader != nullptr)
			{
				auto _castShader = reinterpret_cast<DX11Shader*>(shaderProgram._vertexShader);
				m_VS = _castShader->GetNativeShader()._vertexShader;

				m_InputLayout = _castShader->GetInputLayout();
			}
			if (shaderProgram._hullShader != nullptr)
			{
				auto _castShader = reinterpret_cast<DX11Shader*>(shaderProgram._hullShader);
				m_HS = _castShader->GetNativeShader()._hullShader;
			}
			if (shaderProgram._domainShader != nullptr)
			{
				auto _castShader = reinterpret_cast<DX11Shader*>(shaderProgram._domainShader);
				m_DS = _castShader->GetNativeShader()._domainShader;
			}
			if (shaderProgram._geometryShader != nullptr)
			{
				auto _castShader = reinterpret_cast<DX11Shader*>(shaderProgram._geometryShader);
				m_GS = _castShader->GetNativeShader()._geometryShader;
			}
			if (shaderProgram._pixelShader != nullptr)
			{
				auto _castShader = reinterpret_cast<DX11Shader*>(shaderProgram._pixelShader);
				m_PS = _castShader->GetNativeShader()._pixelShader;
			}
		}

		void DX11PipelineState::PipelineReflect(const vector<DX11Shader*>& shaders)
		{
			long _bufferStageFlags[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT] = {};
			long _resourceStageFlags[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = {};
			long _samplerStageFlags[D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT] = {};

			// 모든 쉐이더로 부터 프로퍼티 블록을 얻어옴
			for (auto* _shader : shaders)
			{
				assert(_shader != nullptr);

				const PropertyDesc* _curPropertyDesc = nullptr;
				HRESULT _hr = _shader->ReflectShader(&_curPropertyDesc);

				assert(SUCCEEDED(_hr) || _curPropertyDesc != nullptr);

				m_PropertyDesc._bindBuffers.reserve(m_PropertyDesc._bindBuffers.size() + _curPropertyDesc->_bindBuffers.size());
				
				for (const auto& _curBindBuf : _curPropertyDesc->_bindBuffers)
				{
					auto _iter = find_if(begin(m_PropertyDesc._bindBuffers), end(m_PropertyDesc._bindBuffers), 
						[&_curBindBuf](auto& _desc)
						{
							return _desc._name == _curBindBuf._name;
						});

					if (_iter == m_PropertyDesc._bindBuffers.end())
					{
						m_PropertyDesc._bindBuffers.push_back(_curBindBuf);
						m_PropertyDesc._bindBuffers.back()._stageFlags |= GetStageFlags(_shader->GetShaderType());
					}
					else
					{
						_iter->_stageFlags |= GetStageFlags(_shader->GetShaderType());
					}

					_bufferStageFlags[_curBindBuf._boundSlot] |= GetStageFlags(_shader->GetShaderType());
				}

				m_PropertyDesc._bindResources.reserve(m_PropertyDesc._bindResources.size() + _curPropertyDesc->_bindResources.size());
				
				for (const auto& _curBindRes : _curPropertyDesc->_bindResources)
				{
					auto _iter = find_if(begin(m_PropertyDesc._bindResources), end(m_PropertyDesc._bindResources),
						[&_curBindRes](auto& _desc)
						{
							return _desc._name == _curBindRes._name;
						});

					if (_iter == m_PropertyDesc._bindResources.end())
					{
						m_PropertyDesc._bindResources.push_back(_curBindRes);
						m_PropertyDesc._bindResources.back()._stageFlags |= GetStageFlags(_shader->GetShaderType());
					}
					else
					{
						_iter->_stageFlags |= GetStageFlags(_shader->GetShaderType());
					}

					_resourceStageFlags[_curBindRes._boundSlot] |= GetStageFlags(_shader->GetShaderType());
				}

				/*_propertyDesc._bindSamplers.reserve(_propertyDesc._bindResources.size() + _curPropertyDesc->_bindResources.size());
				for (const auto& _curBindRes : _curPropertyDesc->_bindResources)
				{
					_propertyDesc._bindResources.push_back(_curBindRes);
					_bufferStageFlags[_curBindRes._boundSlot] |= GetStageFlags(_shader->GetShaderType());
				}*/
			}
		}

		void DX11PipelineState::GetDXDepthStencil(D3D11_DEPTH_STENCIL_DESC& desc, const DepthDesc& depthDesc, const StencilDesc stencilDesc)
		{
			desc.DepthEnable = depthDesc._depthEnabled;
			desc.DepthFunc = MapCompareOp(depthDesc.compareOp);
			desc.DepthWriteMask = depthDesc._writeEnabled ? D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;

			desc.StencilEnable = stencilDesc._stencilEnable;
			desc.StencilReadMask = stencilDesc._readMask;
			desc.StencilWriteMask = stencilDesc._writeMask;

			desc.FrontFace.StencilFailOp = MapStencilOp(stencilDesc._front._stencilFailOp);
			desc.FrontFace.StencilDepthFailOp = MapStencilOp(stencilDesc._front._depthFailOp);
			desc.FrontFace.StencilPassOp = MapStencilOp(stencilDesc._front._depthPassOp);
			desc.FrontFace.StencilFunc = MapCompareOp(stencilDesc._front._compareOp);

			desc.BackFace.StencilFailOp = MapStencilOp(stencilDesc._back._stencilFailOp);
			desc.BackFace.StencilDepthFailOp = MapStencilOp(stencilDesc._back._depthFailOp);
			desc.BackFace.StencilPassOp = MapStencilOp(stencilDesc._back._depthPassOp);
			desc.BackFace.StencilFunc = MapCompareOp(stencilDesc._back._compareOp);
		}

		void DX11PipelineState::GetDXRasterizer(D3D11_RASTERIZER_DESC& desc, const RasterizerDesc& rasterizerDesc)
		{
			desc.FillMode = MapFillMode(rasterizerDesc._fillMode);
			desc.CullMode = MapCullMode(rasterizerDesc._cullMode);
			desc.FrontCounterClockwise = rasterizerDesc._frontCCW;
			desc.DepthBias = static_cast<INT>(rasterizerDesc._depthBias.constantFactor);
			desc.DepthBiasClamp = rasterizerDesc._depthBias.clamp;
			desc.SlopeScaledDepthBias = rasterizerDesc._depthBias.slopeFactor;
			desc.DepthClipEnable = rasterizerDesc._depthClampEnabled;
			desc.ScissorEnable = rasterizerDesc._scissorTestEnabled;
			desc.MultisampleEnable = rasterizerDesc._multiSampleEnabled;
			desc.AntialiasedLineEnable = rasterizerDesc._antiAliasedLineEnabled;
		}

		void DX11PipelineState::GetDXBlend(D3D11_BLEND_DESC& desc, const BlendDesc& blendDesc)
		{
			desc.AlphaToCoverageEnable = blendDesc._alphaToCoverageEnable;
			desc.IndependentBlendEnable = blendDesc._independentBlendEnable;

			for (int i = 0; i < 8; ++i)
			{
				desc.RenderTarget[i].BlendEnable = blendDesc._renderTarget[i]._blendEnable;
				desc.RenderTarget[i].SrcBlend = MapBlend(blendDesc._renderTarget[i]._srcBlend);
				desc.RenderTarget[i].DestBlend = MapBlend(blendDesc._renderTarget[i]._destBlend);
				desc.RenderTarget[i].BlendOp = MapBlendOp(blendDesc._renderTarget[i]._blendOp);
				desc.RenderTarget[i].SrcBlendAlpha = MapBlend(blendDesc._renderTarget[i]._srcBlendAlpha);
				desc.RenderTarget[i].DestBlendAlpha = MapBlend(blendDesc._renderTarget[i]._destBlendAlpha);
				desc.RenderTarget[i].BlendOpAlpha = MapBlendOp(blendDesc._renderTarget[i]._blendOpAlpha);
				desc.RenderTarget[i].RenderTargetWriteMask = blendDesc._renderTarget[i]._renderTargetWriteMask;
			}
		}

	}
}