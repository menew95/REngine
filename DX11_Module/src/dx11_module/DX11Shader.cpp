#include "dx11_module_pch.h"

#include "dx11_module\DX11Shader.h"
#include "dx11_module\DX11Utilitys.h"
#include "graphics_module\ResourceFlags.h"

#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>

namespace graphics
{
	namespace DX11
	{

		DX11Shader::DX11Shader(ID3D11Device* device, const ShaderDesc& desc)
			: m_ShaderDesc(desc)
		{
			BuildShader(device, desc);
		}

		void DX11Shader::SetName(const char* name)
		{
			DX11SetObjectName(static_cast<ID3D11DeviceChild*>(m_NativeShader._vertexShader.Get()), name);
		}

		HRESULT DX11Shader::ReflectShader(const PropertyDesc** propertyDesc)
		{
			if (m_ShaderReflectResult == S_FALSE)
			{
				m_properties._bindBuffers.clear();
				m_properties._bindResources.clear();

				m_ShaderReflectResult = ReflectShaderProperty(m_properties);
			}

			if (m_ShaderReflectResult == S_OK)
			{
				/* Return cached constnat buffer reflections */
				if (propertyDesc != nullptr)
					*propertyDesc = &m_properties;
				return S_OK;
			}

			return m_ShaderReflectResult;
		}

		void DX11Shader::BuildShader(ID3D11Device* device, const ShaderDesc& desc)
		{
			switch (desc._sourceType)
			{
				case ShaderSourceType::CSO: LoadShader(device, desc); break;
				case ShaderSourceType::HLSL: CompileShader(device, desc); break;
			}
		}

		void DX11Shader::CompileShader(ID3D11Device* device, const ShaderDesc& desc)
		{
			tstring _fileContent;
			const char* _sourceCode = nullptr;
			size_t _sourceLength = 0;

			/* Get parameter from union */
			const char* entry = desc._entryPoint;
			const char* target = (desc._profile != nullptr ? desc._profile : "");
			auto        defines = reinterpret_cast<const D3D_SHADER_MACRO*>(desc._defines);
			auto        flags = desc._flags;

			flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

#if defined(_DEBUG) || defined(DEBUG)
			flags |= D3DCOMPILE_DEBUG; // 쉐이더 디버깅할 때 필요

			flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

			/* Compile shader code */
			auto _hr = D3DCompileFromFile(
				desc._filePath.c_str(),
				defines,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,
				entry,
				target,
				flags,
				0,
				&m_Blob,
				&m_Errors);
			HR(_hr, "failed to complie shader file");

			if (_hr == E_FAIL)
			{
				auto error = (char*)m_Errors->GetBufferPointer();

				assert(false);
			}

			if (m_Blob.Get() != nullptr && m_Blob->GetBufferSize() > 0)
			{
				CreateNativeShader(device, desc);
			}
		}

		void DX11Shader::LoadShader(ID3D11Device* device, const ShaderDesc& desc)
		{
			auto _hr = D3DReadFileToBlob(desc._filePath.c_str(), m_Blob.ReleaseAndGetAddressOf());
			HR(_hr, "failed to read shader file");

			if (m_Blob.Get() != nullptr && m_Blob->GetBufferSize() > 0)
			{
				CreateNativeShader(device, desc);
			}
		}

		void DX11Shader::CreateNativeShader(ID3D11Device* device, const ShaderDesc& desc)
		{
			CreateShader(device, desc._shaderType);
		}

		void DX11Shader::CreateShader(ID3D11Device* device, ShaderType type)
		{
			HRESULT _hr = 0;

			switch (type)
			{
				case ShaderType::Pixel:
				{
					_hr = device->CreatePixelShader(
						m_Blob->GetBufferPointer(),
						m_Blob->GetBufferSize(),
						nullptr,
						m_NativeShader._pixelShader.ReleaseAndGetAddressOf());

					HR(_hr, "faild to create pixel shader");

					break;
				}
				case ShaderType::Vertex:
				{
					_hr = device->CreateVertexShader(
						m_Blob->GetBufferPointer(),
						m_Blob->GetBufferSize(),
						nullptr,
						m_NativeShader._vertexShader.ReleaseAndGetAddressOf());

					HR(_hr, "faild to create vertex shader");
					
					Reflect(device);

					break;
				}
				case ShaderType::Geometry:
				{
					_hr = device->CreateGeometryShader(
						m_Blob->GetBufferPointer(),
						m_Blob->GetBufferSize(),
						nullptr,
						m_NativeShader._geometryShader.ReleaseAndGetAddressOf());

					HR(_hr, "faild to create geometry shader");
					break;
				}
				case ShaderType::Hull:
				{
					_hr = device->CreateHullShader(
						m_Blob->GetBufferPointer(),
						m_Blob->GetBufferSize(),
						nullptr,
						m_NativeShader._hullShader.ReleaseAndGetAddressOf());

					HR(_hr, "faild to create hull shader");
					break;
				}
				case ShaderType::Domain:
				{
					_hr = device->CreateDomainShader(
						m_Blob->GetBufferPointer(),
						m_Blob->GetBufferSize(),
						nullptr,
						m_NativeShader._domainShader.ReleaseAndGetAddressOf());

					HR(_hr, "faild to create domain shader");
					break;
				}
				case ShaderType::Compute:
				{
					_hr = device->CreateComputeShader(
						m_Blob->GetBufferPointer(),
						m_Blob->GetBufferSize(),
						nullptr,
						m_NativeShader._computeShader.ReleaseAndGetAddressOf());

					HR(_hr, "faild to create compute shader");
					break;
				}
				default:
					break;
			}

			HR(_hr, "failed to create DX11 shader");
		}

		ShaderType DX11Shader::GetDXShaderType(uint32 shaderType)
		{
			switch (shaderType)
			{
				case D3D11_SHVER_PIXEL_SHADER:		return ShaderType::Pixel;
				case D3D11_SHVER_VERTEX_SHADER:		return ShaderType::Vertex;
				case D3D11_SHVER_GEOMETRY_SHADER:	return ShaderType::Geometry;
				case D3D11_SHVER_HULL_SHADER:		return ShaderType::Hull;
				case D3D11_SHVER_DOMAIN_SHADER:		return ShaderType::Domain;
				case D3D11_SHVER_COMPUTE_SHADER:	return ShaderType::Compute;
				case D3D11_SHVER_RESERVED0:			return ShaderType::Reserved0;
			}

			assert(false);
			return ShaderType::Reserved0;
		}

		void DX11Shader::Reflect(ID3D11Device* device)
		{
			ComPtr<ID3D11ShaderReflection> reflection = nullptr;

			HRESULT hr;
			hr = D3DReflect(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)reflection.GetAddressOf());

			if (reflection == nullptr)
			{
				assert(false);
			}

			D3D11_SHADER_DESC _shaderDesc;
			reflection->GetDesc(&_shaderDesc);

			ReflectInputLayout(device, reflection.Get(), _shaderDesc);
		}

		void DX11Shader::ReflectInputLayout(ID3D11Device* device, ID3D11ShaderReflection* reflection, D3D11_SHADER_DESC& desc)
		{
			// Create InputLayout
			std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
			int idx = 0;
			for (int idx = 0; idx < (int)desc.InputParameters; idx++)
			{
				D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
				reflection->GetInputParameterDesc(idx, &paramDesc);

				D3D11_INPUT_ELEMENT_DESC inputDesc;
				inputDesc.SemanticName = paramDesc.SemanticName;
				inputDesc.SemanticIndex = paramDesc.SemanticIndex;

				if (idx == 0)
				{
					inputDesc.AlignedByteOffset = 0;
				}
				else
				{
					inputDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
				}

				std::string semanticName = paramDesc.SemanticName;

				if (semanticName.find("INSTANCE_") == 0)
				{
					inputDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
					inputDesc.InputSlot = 1;
					inputDesc.InstanceDataStepRate = 1;
				}
				else
				{
					inputDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
					inputDesc.InputSlot = 0;
					inputDesc.InstanceDataStepRate = 0;
				}

				if (paramDesc.Mask == 1)
				{
					if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputDesc.Format = DXGI_FORMAT_R32_UINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputDesc.Format = DXGI_FORMAT_R32_SINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputDesc.Format = DXGI_FORMAT_R32_FLOAT;
				}
				else if (paramDesc.Mask <= 3)
				{
					if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputDesc.Format = DXGI_FORMAT_R32G32_UINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputDesc.Format = DXGI_FORMAT_R32G32_SINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
				}
				else if (paramDesc.Mask <= 7)
				{
					if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				}
				else if (paramDesc.Mask <= 15)
				{
					if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				}

				inputLayoutDesc.push_back(inputDesc);
			}

			auto _hr = device->CreateInputLayout(inputLayoutDesc.data(),
				(UINT)inputLayoutDesc.size(), m_Blob->GetBufferPointer(),
				m_Blob->GetBufferSize(), m_InputLayout.ReleaseAndGetAddressOf());

			HR(_hr, "Faild to create inputlayout");
		}

		HRESULT DX11Shader::ReflectShaderProperty(PropertyDesc& propertyDesc)
		{
			HRESULT _hr = S_OK;

			/* Get shader reflection */
			ComPtr<ID3D11ShaderReflection> _reflection;
			_hr = D3DReflect(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), IID_PPV_ARGS(_reflection.ReleaseAndGetAddressOf()));
			if (FAILED(_hr))
				return _hr;

			D3D11_SHADER_DESC _desc;
			_hr = _reflection->GetDesc(&_desc);
			if (FAILED(_hr))
				return _hr;

			for (UINT i = 0; i < _desc.BoundResources; i++)
			{
				D3D11_SHADER_INPUT_BIND_DESC _bindDesc;
				_hr = _reflection->GetResourceBindingDesc(i, &_bindDesc);

				if(_hr == S_OK)
				{
					switch (_bindDesc.Type)
					{
						case D3D_SIT_TEXTURE:
						{
							BindResourceDesc _resourceDesc;

							_resourceDesc._name = StringHelper::StringToWString(_bindDesc.Name);
							_resourceDesc._boundSlot = _bindDesc.BindPoint;
							_resourceDesc._boundCount = _bindDesc.BindCount;

							_resourceDesc._resourceType = (uint32)ResourceType::Texture;

							propertyDesc._bindResources.push_back(_resourceDesc);
							break;
						}
						break;
						case D3D_SIT_SAMPLER:
						{
							BindResourceDesc _resourceDesc;

							_resourceDesc._name = StringHelper::StringToWString(_bindDesc.Name);
							_resourceDesc._boundSlot = _bindDesc.BindPoint;
							_resourceDesc._boundCount = _bindDesc.BindCount;

							_resourceDesc._resourceType = (uint32)ResourceType::Sampler;

							propertyDesc._bindResources.push_back(_resourceDesc);
							break;
						}
						break;
						case D3D_SIT_UAV_RWTYPED:
						{
							assert(false);
							break;
						}
						case D3D_SIT_CBUFFER:
						{
							ID3D11ShaderReflectionConstantBuffer* buffer = _reflection->GetConstantBufferByIndex(i);
							D3D11_SHADER_BUFFER_DESC _bufferDesc;
							buffer->GetDesc(&_bufferDesc);

							ConstantBufferDesc _constBufferDesc;

							_constBufferDesc._name = StringHelper::StringToWString(_bindDesc.Name);
							_constBufferDesc._boundSlot = _bindDesc.BindPoint;
							_constBufferDesc._boundCount = _bindDesc.BindCount;
							for (UINT j = 0; j < _bufferDesc.Variables; ++j)
							{
								ID3D11ShaderReflectionVariable* var = buffer->GetVariableByIndex(j);
								D3D11_SHADER_VARIABLE_DESC varDesc;
								var->GetDesc(&varDesc);

								BufferField _info;
								_info._name = StringHelper::StringToWString(varDesc.Name);
								_info._size = varDesc.Size;
								_info._offset = varDesc.StartOffset;
								_constBufferDesc._fields.push_back(_info);
							}

							propertyDesc._bindBuffers.push_back(_constBufferDesc);

							break;
						}
						default:
						{
							// 아직 예상 하지 못한 타입이 들어왔음
							assert(false);
							break;
						}
					}
				}
			}

			return _hr;
		}


	}
}