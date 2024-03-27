#pragma once

#include "graphics_module\Shader.h"
#include "graphics_module\Common\ComPtr.h"

#include "dx11_module\Direct3D11.h"

struct ID3D11ShaderReflection;
typedef struct _D3D11_SHADER_DESC D3D11_SHADER_DESC;

namespace graphics
{
	namespace DX11
	{
		union DX11NativeShader
		{
			inline DX11NativeShader() :
				_vertexShader{ nullptr }
			{
			}
			inline DX11NativeShader(const DX11NativeShader& rhs) :
				_vertexShader{ rhs._vertexShader }
			{
			}
			inline DX11NativeShader& operator = (const DX11NativeShader& rhs)
			{
				_vertexShader = rhs._vertexShader;
				return *this;
			}
			inline ~DX11NativeShader()
			{
				_vertexShader.Reset();
			}

			ComPtr<ID3D11PixelShader>		_pixelShader;
			ComPtr<ID3D11VertexShader>		_vertexShader;
			ComPtr<ID3D11GeometryShader>	_geometryShader;
			ComPtr<ID3D11HullShader>		_hullShader;
			ComPtr<ID3D11DomainShader>		_domainShader;
			ComPtr<ID3D11ComputeShader>		_computeShader;
		};

		struct D3D11ConstantReflection
		{
			std::string name;   // Name of the constant buffer field.
			UINT        offset; // Offset (in bytes) within the constant buffer.
			UINT        size;   // Size (in bytes) of this uniform.
		};

		struct D3D11ConstantBufferReflection
		{
			UINT                                    slot;
			UINT                                    size;
			std::vector<D3D11ConstantReflection>    fields;
		};

		class DX11Shader : public Shader
		{
		public:
			DX11Shader(ID3D11Device* device, const ShaderDesc& desc);

			inline const DX11NativeShader& GetNativeShader() const {  return m_NativeShader;  }
			inline const ID3DBlob* GetBlob() const { return m_Blob.Get(); }
			inline const ComPtr<ID3D11InputLayout>& GetInputLayout() const { return m_InputLayout; }

			void SetName(const char* name) override;
			ShaderType GetShaderType() { return m_ShaderDesc._shaderType; }

			HRESULT ReflectShader(const PropertyDesc** propertyDesc);

			void ReflectInputLayout(ID3D11Device* device, ID3D11ShaderReflection* reflection, D3D11_SHADER_DESC& desc);
		private:
			// shader 파일의 종류에 따라 컴파일 혹은 로드를 해줌
			void BuildShader(ID3D11Device* device, const ShaderDesc& desc);
			void CompileShader(ID3D11Device* device, const ShaderDesc& desc);
			void LoadShader(ID3D11Device* device, const ShaderDesc& desc);

			void CreateNativeShader(ID3D11Device* device, const ShaderDesc& desc);
			void CreateShader(ID3D11Device* device, ShaderType type);
			ShaderType GetDXShaderType(uint32 shaderType);

			void Reflect(ID3D11Device* device);
			HRESULT ReflectShaderProperty(PropertyDesc& propertyDesc);

			ShaderDesc m_ShaderDesc;

			// reflection property
			HRESULT m_ShaderReflectResult = S_FALSE;
			PropertyDesc m_properties;

			DX11NativeShader m_NativeShader;

			ComPtr<ID3DBlob> m_Blob;
			ComPtr<ID3DBlob> m_Errors;

			ComPtr<ID3D11InputLayout> m_InputLayout;
		};
	}
}