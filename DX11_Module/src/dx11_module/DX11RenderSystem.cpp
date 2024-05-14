#include "dx11_module_pch.h"

#include "dx11_module\DX11RenderSystem.h"
#include "dx11_module\DX11StateManager.h"
#include "dx11_module\DX11SwapChain.h"
#include "dx11_module\DX11CommandBuffer.h"
#include "dx11_module\DX11Buffer.h"
#include "dx11_module\DX11BufferRV.h"
#include "dx11_module\DX11Texture.h"
#include "dx11_module\DX11Sampler.h"
#include "dx11_module\DX11RenderPass.h"
#include "dx11_module\DX11RenderTarget.h"
#include "dx11_module\DX11Shader.h"
#include "dx11_module\DX11PipelineLayout.h"
#include "dx11_module\DX11PipelineState.h"
#include "dx11_module\DX11ComputePS.h"
#include "dx11_module\DX11ResourceFlags.h"
#include "dx11_module\DX11ResourceView.h"

namespace graphics
{
	namespace DX11
	{

		DX11RenderSystem::DX11RenderSystem()
		{
			CreateFactory();
			QueryVideoAdapters();
			CreateDevice(nullptr);
			CreateStateManager();
		}

		DX11RenderSystem::~DX11RenderSystem()
		{
			m_CommandBufferContainer.clear();
			m_BufferContainer.clear();
			m_RenderTargetContainer.clear();
			m_SamplerContainer.clear();
			m_ShaderContainer.clear();
			m_PipelineLayoutContainer.clear();
			m_PipelineStateContainer.clear();

			m_StateManager.reset();

			m_Context->ClearState();
			m_Context->Flush();
			m_Context.Reset();

#if defined(DEBUG) || defined(_DEBUG)
			ComPtr<ID3D11Debug> Debug;
			m_Device->QueryInterface(Debug.GetAddressOf());

			OutputDebugStringA("-------누수 오브젝트 목록입니다--------\r\n");
			Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
			OutputDebugStringA("-------------------------------------\r\n");
			Debug.Reset();
#endif

			m_Adapter.Reset();
			m_Factory.Reset();
		}

		void* DX11RenderSystem::GetDevice()
		{
			return m_Device.Get();
		}

		void* DX11RenderSystem::GetDeviceContext()
		{
			return m_Context.Get();
		}

		SwapChain* DX11RenderSystem::CreateSwapChain(uuid uuid, const SwapChainDesc& desc)
		{
			return TakeOwnership(m_SwapChainContainer, uuid, MakeUnique<DX11SwapChain>(m_Factory.Get(), m_Device.Get(), desc));
		}

		void DX11RenderSystem::Release(SwapChain& swapChain)
		{
			RemoveFromUniqueUnorderedMap(m_SwapChainContainer, &swapChain);
		}

		void DX11RenderSystem::WriteBuffer(Buffer& buffer, const void* data, uint32 dataSize, uint32 offset)
		{
			DX11Buffer& _castBuffer = reinterpret_cast<DX11Buffer&>(buffer);

			_castBuffer.UpdateSubresource(m_Context.Get(), data, dataSize, offset);
		}

		void DX11RenderSystem::ReadBuffer(Buffer& buffer, void* data, uint32 dataSize, uint32 offset)
		{
			DX11Buffer& _castBuffer = reinterpret_cast<DX11Buffer&>(buffer);

			_castBuffer.ReadSubresource(m_Context.Get(), data, dataSize, offset);
		}

		void* DX11RenderSystem::MapBuffer(Buffer& buffer, const CPUAccess access)
		{
			DX11Buffer& _castBuffer = reinterpret_cast<DX11Buffer&>(buffer);

			return _castBuffer.Map(m_Context.Get(), access, 0, 0);
		}

		void* DX11RenderSystem::MapBuffer(Buffer& buffer, const CPUAccess access, uint32 offset, uint32 length)
		{
			DX11Buffer& _castBuffer = reinterpret_cast<DX11Buffer&>(buffer);

			return _castBuffer.Map(m_Context.Get(), access, offset, length);
		}

		void DX11RenderSystem::UnmapBuffer(Buffer& buffer)
		{
			DX11Buffer& _castBuffer = reinterpret_cast<DX11Buffer&>(buffer);

			_castBuffer.Unmap(m_Context.Get());
		}

		void DX11RenderSystem::WriteTexture(Texture& texture, const TextureRegion& textureRegion, const ImageDesc& imageDesc)
		{
			auto& textureD3D = reinterpret_cast<DX11Texture&>(texture);

			switch (texture.GetType())
			{
				case TextureType::Texture1D:
				case TextureType::Texture1DArray:
					textureD3D.UpdateSubresource(
						m_Context.Get(),
						textureRegion._subresource._baseMipLevel,
						textureRegion._subresource._baseArrayLayer,
						0,
						CD3D11_BOX(
							textureRegion._offset.x,
							0,
							0,
							textureRegion._offset.x + static_cast<LONG>(textureRegion._extent._width),
							static_cast<LONG>(textureRegion._subresource._numArrayLayers),
							1
						),
						imageDesc
					);
					break;

				case TextureType::Texture2D:
				case TextureType::TextureCube:
				case TextureType::Texture2DArray:
				case TextureType::TextureCubeArray:
					textureD3D.UpdateSubresource(
						m_Context.Get(),
						textureRegion._subresource._baseMipLevel,
						textureRegion._subresource._baseArrayLayer,
						0,
						CD3D11_BOX(
							textureRegion._offset.x,
							textureRegion._offset.y,
							0,
							textureRegion._offset.x + static_cast<LONG>(textureRegion._extent._width),
							textureRegion._offset.y + static_cast<LONG>(textureRegion._extent._height),
							static_cast<LONG>(textureRegion._subresource._numArrayLayers)
						),
						imageDesc
					);
					break;

				case TextureType::Texture2DMS:
				case TextureType::Texture2DMSArray:
					break;

				case TextureType::Texture3D:
					textureD3D.UpdateSubresource(
						m_Context.Get(),
						textureRegion._subresource._baseMipLevel,
						0,
						0,
						CD3D11_BOX(
							textureRegion._offset.x,
							textureRegion._offset.y,
							textureRegion._offset.z,
							textureRegion._offset.x + static_cast<LONG>(textureRegion._extent._width),
							textureRegion._offset.y + static_cast<LONG>(textureRegion._extent._height),
							textureRegion._offset.z + static_cast<LONG>(textureRegion._extent._depth)
						),
						imageDesc
					);
					break;
			}
		}

		void DX11RenderSystem::ReadTexture(Texture& texture, const TextureRegion& textureRegion, CopyImageView& copyImageView)
		{
			DX11Texture& _castTex = reinterpret_cast<DX11Texture&>(texture);

			DX11NativeTexture copy;
			_castTex.CreateSubresourceCopyWithCPUAccess(m_Device.Get(), m_Context.Get(), copy, D3D11_CPU_ACCESS_READ, textureRegion);

			const UINT subresource = 0;

			D3D11_MAPPED_SUBRESOURCE _mappedSubresource;
			auto _hr = m_Context->Map(copy._resource.Get(), subresource, D3D11_MAP_READ, 0, &_mappedSubresource);
			
			HR(_hr, "read texture mapping failed");

			/* Copy host visible resource to CPU accessible resource */
			auto& format = _castTex.GetDesc()._format;
			UINT32 copydataSize = 0;

			switch (_castTex.GetDesc()._format)
			{
				case Format::R32G32B32A32_TYPELESS:
				case Format::R32G32B32A32_FLOAT:
				case Format::R32G32B32A32_UINT:
				case Format::R32G32B32A32_SINT:
				{
					copydataSize = 16;
					break;
				}
				case Format::R8G8B8A8_TYPELESS:
				case Format::R8G8B8A8_UNORM:
				case Format::R8G8B8A8_UNORM_SRGB:
				case Format::R8G8B8A8_UINT:
				case Format::R8G8B8A8_SNORM:
				case Format::R8G8B8A8_SINT:
				{
					copydataSize = 4;
					break;
				}
				default:
					break;
			}

			if (copyImageView.dataSize < copydataSize)
			{
				copydataSize = copyImageView.dataSize;
			}

			memcpy(copyImageView.data, _mappedSubresource.pData, copydataSize);
			/* Unmap resource */
			m_Context->Unmap(copy._resource.Get(), subresource);

		}

		CommandBuffer* DX11RenderSystem::CreateCommandBuffer(uuid uuid, const CommandBufferDesc& desc)
		{
			return TakeOwnership(m_CommandBufferContainer, uuid, MakeUnique<DX11CommandBuffer>(m_Device.Get(), m_Context, m_StateManager, desc));;
		}

		void DX11RenderSystem::Release(CommandBuffer& commandBuffer)
		{
			RemoveFromUniqueUnorderedMap(m_CommandBufferContainer, &commandBuffer);
		}

		static std::unique_ptr<DX11Buffer> MakeD3D11Buffer(ID3D11Device* device, const BufferDesc& desc, const void* initialData)
		{
			/* Make respective buffer type */
			if (DXBindFlagsNeedBufferWithRV(desc._bindFlags))
				return MakeUnique<DX11BufferRV>(device, desc, initialData);
			else
				return MakeUnique<DX11Buffer>(device, desc, initialData);
		}

		Buffer* DX11RenderSystem::CreateBuffer(uuid uuid, const BufferDesc& desc, const void* initData /*= nullptr*/)
		{
			return TakeOwnership(m_BufferContainer, uuid, MakeD3D11Buffer(m_Device.Get(), desc, initData));
		}

		void DX11RenderSystem::Release(Buffer& buffer)
		{
			RemoveFromUniqueUnorderedMap(m_BufferContainer, &buffer);
		}

		Texture* DX11RenderSystem::CreateTexture(uuid uuid, const TextureDesc& desc, const ImageDesc* imageDesc /*= nullptr*/)
		{
			DX11Texture* _texture = TakeOwnership(m_TextureContainer, uuid, MakeUnique<DX11Texture>(m_Device.Get(), desc));

			//if (imageDesc == nullptr)
			//{
			//	/*switch (desc._textureType)
			//	{
			//		case TextureType::Texture1D:
			//		case TextureType::Texture1DArray:
			//		{
			//			_texture->CreateTexture1D(m_Device.Get(), desc);
			//			break;
			//		}
			//		case TextureType::Texture2D:
			//		case TextureType::Texture2DArray:
			//		case TextureType::TextureCube:
			//		case TextureType::TextureCubeArray:
			//		case TextureType::Texture2DMS:
			//		case TextureType::Texture2DMSArray:
			//		{
			//			_texture->CreateTexture2D(m_Device.Get(), desc);
			//			break;
			//		}
			//		case TextureType::Texture3D:
			//		{
			//			_texture->CreateTexture3D(m_Device.Get(), desc);
			//			break;
			//		}
			//		default:
			//			AssertMessageBox(false, "failed to create texture with invaild texture type");
			//			break;
			//	}*/
			//	if ((desc._bindFlags & BindFlags::ShaderResource) != 0)
			//	{
			//		_texture->CreateShaderResourceView(m_Device.Get(), 0, desc._mipLevels, 0, desc._arrayLayers);
			//	}
			//	if ((desc._bindFlags & BindFlags::UnorderedAccess) != 0)
			//	{
			//		_texture->CreateUnorderedAccessView(m_Device.Get(), 0, 0, desc._arrayLayers);
			//	}
			//}
			//else
			//{
			//	//_texture->CreateTextureFromFile(m_Device.Get(), *imageDesc);

			//	_texture->UpdateSubresource(
			//		m_Context.Get(),
			//		0,
			//		0,
			//		desc._arrayLayers,
			//		D3D11_BOX{0, 0, 0, desc._extend._width, desc._extend._height, desc._extend._depth },
			//		*imageDesc
			//	);
			//}


			if (imageDesc != nullptr)
			{
				D3D11_BOX _region{ 0, 0, 0, desc._extend._width, desc._extend._height, desc._extend._depth};

				for (size_t i = 0; i < desc._mipLevels; i++)
				{
					_texture->UpdateSubresource(
						m_Context.Get(),
						static_cast<uint32>(i),
						0,
						desc._arrayLayers,
						_region,
						&imageDesc[i * desc._arrayLayers]
					);

					_region.right /= 2;
					_region.bottom /= 2;

					if(desc._textureType == TextureType::Texture3D)
						_region.back /= 2;
				}
			}

			/*if ((desc._bindFlags & BindFlags::ShaderResource) != 0)
			{
				_texture->CreateShaderResourceView(m_Device.Get(), 0, desc._mipLevels, 0, desc._arrayLayers);
			}
			if ((desc._bindFlags & BindFlags::UnorderedAccess) != 0)
			{
				_texture->CreateUnorderedAccessView(m_Device.Get(), 0, 0, desc._arrayLayers);
			}*/

			return _texture;
		}

		void DX11RenderSystem::Release(Texture& texture)
		{
			RemoveFromUniqueUnorderedMap(m_TextureContainer, &texture);
		}

		graphics::Sampler* DX11RenderSystem::CreateSampler(uuid uuid, const SamplerDesc& desc)
		{
			return TakeOwnership(m_SamplerContainer, uuid, MakeUnique<DX11Sampler>(m_Device.Get(), desc));
		}

		void DX11RenderSystem::Release(Sampler& sampler)
		{
			RemoveFromUniqueUnorderedMap(m_SamplerContainer, &sampler);
		}

		graphics::ResourceView* DX11RenderSystem::CreateResoureView(uuid uuid, const ResourceViewDesc& desc)
		{
			return TakeOwnership(m_ResourceViewContainer, uuid, MakeUnique<DX11ResourceView>(m_Device.Get(), desc));
		}

		void DX11RenderSystem::Release(ResourceView& resourceView)
		{
			RemoveFromUniqueUnorderedMap(m_ResourceViewContainer, &resourceView);
		}

		/*graphics::RenderPass* DX11RenderSystem::CreateRenderPass(uuid uuid, const RenderPassDesc& desc)
		{
			DX11RenderPass* _pass = TakeOwnership(m_RenderPassContainer, uuid, MakeUnique<DX11RenderPass>(desc));

			return _pass;
		}

		void DX11RenderSystem::Release(RenderPass& renderPass)
		{
			RemoveFromUniqueUnorderedMap(m_RenderPassContainer, &renderPass);
		}*/

		graphics::RenderTarget* DX11RenderSystem::CreateRenderTarget(uuid uuid, const RenderTargetDesc& desc)
		{
			return TakeOwnership(m_RenderTargetContainer, uuid, MakeUnique<DX11RenderTarget>(m_Device.Get(), desc));
		}

		void DX11RenderSystem::Release(RenderTarget& renderTarget)
		{
			RemoveFromUniqueUnorderedMap(m_RenderTargetContainer, &renderTarget);
		}

		graphics::Shader* DX11RenderSystem::CreateShader(uuid uuid, const ShaderDesc& desc)
		{
			return TakeOwnership(m_ShaderContainer, uuid, MakeUnique<DX11Shader>(m_Device.Get(), desc));
		}

		void DX11RenderSystem::Release(Shader& shader)
		{
			RemoveFromUniqueUnorderedMap(m_ShaderContainer, &shader);
		}

		graphics::PipelineLayout* DX11RenderSystem::CreatePipelineLayout(uuid uuid, const PipelineLayoutDesc& desc)
		{
			return TakeOwnership(m_PipelineLayoutContainer, uuid, MakeUnique<DX11PipelineLayout>(desc));
		}

		void DX11RenderSystem::Release(PipelineLayout& pipelineLayout)
		{
			RemoveFromUniqueUnorderedMap(m_PipelineLayoutContainer, &pipelineLayout);
		}

		graphics::PipelineState* DX11RenderSystem::CreatePipelineState(uuid uuid, const GraphicsPipelineDesc& desc)
		{
			return TakeOwnership(m_PipelineStateContainer, uuid, MakeUnique<DX11PipelineState>(m_Device.Get(), desc));
		}

		graphics::PipelineState* DX11RenderSystem::CreatePipelineState(uuid uuid, const ComputePipelineDesc& desc)
		{
			return TakeOwnership(m_PipelineStateContainer, uuid, MakeUnique<DX11ComputePS>(m_Device.Get(), desc));
		}

		DXGI_SAMPLE_DESC DX11RenderSystem::FindSuitableSampleDesc(ID3D11Device* device, DXGI_FORMAT format, uint32 maxSampleCount)
		{
			for (; maxSampleCount > 1; --maxSampleCount)
			{
				UINT numQualityLevels = 0;
				if (device->CheckMultisampleQualityLevels(format, maxSampleCount, &numQualityLevels) == S_OK)
				{
					if (numQualityLevels > 0)
						return { maxSampleCount, numQualityLevels - 1 };
				}
			}
			return { 1u, 0u };
		}

		void DX11RenderSystem::CreateFactory()
		{
			HR(CreateDXGIFactory(IID_PPV_ARGS(&m_Factory)), "faild to create IDXGIFactory");
		}

		void DX11RenderSystem::QueryVideoAdapters()
		{
			ComPtr<IDXGIAdapter> _adapter;
			uint32 _index = 0;
			
			while (m_Factory->EnumAdapters(_index, _adapter.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND)
			{
				ComPtr<IDXGIOutput> _output;

				uint32 _outIndex = 0;

				DXGI_ADAPTER_DESC desc;
				_adapter->GetDesc(&desc);

				VideoAdapterDesc _adapterDesc;

				_adapterDesc._name = tstring(desc.Description);
				_adapterDesc._vendorID = desc.VendorId;
				_adapterDesc._videoMemory = static_cast<uint64_t>(desc.DedicatedVideoMemory);

				while (_adapter->EnumOutputs(_outIndex, _output.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND)
				{
					UINT _numModes = 0;
					std::vector<DXGI_MODE_DESC> _displayModes;
					DXGI_FORMAT _format = DXGI_FORMAT_R8G8B8A8_UNORM;

					HRESULT hr = _output->GetDisplayModeList(_format, DXGI_ENUM_MODES_INTERLACED, &_numModes, NULL);

					_displayModes.resize(_numModes);

					hr = _output->GetDisplayModeList(_format, 0, &_numModes, _displayModes.data());

					VideoOutputDesc _videoOutputDesc;

					for (size_t i = 0; i < _numModes; i++)
					{
						DisplayModeDesc _modeDesc;

						_modeDesc._resolution._width	= _displayModes[i].Width;
						_modeDesc._resolution._height	= _displayModes[i].Height;
						_modeDesc._refreshRate = (_displayModes[i].RefreshRate.Denominator > 0 ? _displayModes[i].RefreshRate.Numerator / _displayModes[i].RefreshRate.Denominator : 0);

						_videoOutputDesc._displayModes.push_back(_modeDesc);
					}

					m_VideoAdapters.push_back(_adapterDesc);
					_outIndex++;
				}

				_index++;
			}
		}

		void DX11RenderSystem::CreateDevice(IDXGIAdapter* adapter)
		{
			std::vector<D3D_FEATURE_LEVEL> featureLevels =
			{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1,
			};


			HRESULT hr = 0;
#if defined(_DEBUG) || defined(DEBUG)
			if (!CreateDeviceWithFlags(adapter, featureLevels, D3D11_CREATE_DEVICE_DEBUG, hr))
				CreateDeviceWithFlags(adapter, featureLevels, 0, hr);
#else
			CreateDeviceWithFlags(adapter, featureLevels, 0, hr);
#endif
			uint32 _4xMsaaQuality = 0;

			m_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &_4xMsaaQuality);
			assert(_4xMsaaQuality > 0);


		}

		bool DX11RenderSystem::CreateDeviceWithFlags(IDXGIAdapter* adapter, const std::vector<D3D_FEATURE_LEVEL>& featureLevels, UINT flags, HRESULT& hr)
		{
			for (D3D_DRIVER_TYPE driver : { D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE })
			{
				hr = D3D11CreateDevice(
					adapter,                                    // Video adapter
					driver,                                     // Driver type
					0,                                          // Software rasterizer module (none)
					flags,                                      // Flags
					featureLevels.data(),                       // Feature level
					static_cast<UINT>(featureLevels.size()),    // Num feature levels
					D3D11_SDK_VERSION,                          // SDK version
					m_Device.ReleaseAndGetAddressOf(),           // Output device
					&m_FetureLevel,                             // Output feature level
					m_Context.ReleaseAndGetAddressOf()           // Output device context
				);
				if (SUCCEEDED(hr))
					return true;
			}
			return false;
		}

		void DX11RenderSystem::CreateStateManager()
		{
			m_StateManager = std::make_shared<DX11StateManager>(m_Device.Get(), m_Context);
		}

		void DX11RenderSystem::Release(PipelineState& pipelineState)
		{
			RemoveFromUniqueUnorderedMap(m_PipelineStateContainer, &pipelineState);
		}
	}
}