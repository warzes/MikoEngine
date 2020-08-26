#pragma once

namespace Direct3D12Rhi
{
	//[-------------------------------------------------------]
	//[ Global functions                                      ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Creates, loads and compiles a shader from source code
	*
	*  @param[in] shaderModel
	*    ASCII shader model (for example "vs_5_0", "gs_5_0", "ps_5_0"), must be a valid pointer
	*  @param[in] sourceCode
	*    ASCII shader ASCII source code, must be a valid pointer
	*  @param[in] entryPoint
	*    Optional ASCII entry point, if null pointer "main" is used
	*  @param[in] optimizationLevel
	*    Optimization level
	*
	*  @return
	*    The loaded and compiled shader, can be a null pointer, release the instance if you no longer need it
	*/
	[[nodiscard]] ID3DBlob* loadShaderFromSourcecode(const char* shaderModel, const char* sourceCode, const char* entryPoint, Rhi::IShaderLanguage::OptimizationLevel optimizationLevel)
	{
		// Sanity checks
		SE_ASSERT(nullptr != shaderModel, "Invalid Direct3D 12 shader model")
			SE_ASSERT(nullptr != sourceCode, "Invalid Direct3D 12 shader source code")

			// Get compile flags
			// -> "DX12 Do's And Don'ts" ( https://developer.nvidia.com/dx12-dos-and-donts ) "Use the /all_resources_bound / D3DCOMPILE_ALL_RESOURCES_BOUND compile flag if possible"
			UINT compileFlags = (D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_ALL_RESOURCES_BOUND);
		switch ( optimizationLevel )
		{
		case Rhi::IShaderLanguage::OptimizationLevel::DEBUG:
			compileFlags |= D3DCOMPILE_DEBUG;
			compileFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
			break;

		case Rhi::IShaderLanguage::OptimizationLevel::NONE:
			compileFlags |= D3DCOMPILE_SKIP_VALIDATION;
			compileFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
			break;

		case Rhi::IShaderLanguage::OptimizationLevel::LOW:
			compileFlags |= D3DCOMPILE_SKIP_VALIDATION;
			compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL0;
			break;

		case Rhi::IShaderLanguage::OptimizationLevel::MEDIUM:
			compileFlags |= D3DCOMPILE_SKIP_VALIDATION;
			compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL1;
			break;

		case Rhi::IShaderLanguage::OptimizationLevel::HIGH:
			compileFlags |= D3DCOMPILE_SKIP_VALIDATION;
			compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL2;
			break;

		case Rhi::IShaderLanguage::OptimizationLevel::ULTRA:
			compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
			break;
		}

		// Compile
		ID3DBlob* d3dBlob = nullptr;
		ID3DBlob* errorD3dBlob = nullptr;
		if ( FAILED(D3DCompile(sourceCode, strlen(sourceCode), nullptr, nullptr, nullptr, entryPoint ? entryPoint : "main", shaderModel, compileFlags, 0, &d3dBlob, &errorD3dBlob)) )
		{
			if ( nullptr != errorD3dBlob )
			{
				if ( GetLog().print(DefaultLog::Type::CRITICAL, sourceCode, __FILE__, static_cast<uint32_t>(__LINE__), static_cast<char*>(errorD3dBlob->GetBufferPointer())) )
				{
					SE_DEBUG_BREAK;
				}
				errorD3dBlob->Release();
			}
			return nullptr;
		}
		if ( nullptr != errorD3dBlob )
		{
			errorD3dBlob->Release();
		}

		// Done
		return d3dBlob;
	}

	void handleDeviceLost(const Direct3D12Rhi& direct3D12Rhi, HRESULT result)
	{
		// If the device was removed either by a disconnection or a driver upgrade, we must recreate all device resources
		if ( DXGI_ERROR_DEVICE_REMOVED == result || DXGI_ERROR_DEVICE_RESET == result )
		{
			if ( DXGI_ERROR_DEVICE_REMOVED == result )
			{
				result = direct3D12Rhi.getD3D12Device().GetDeviceRemovedReason();
			}
			RHI_LOG(CRITICAL, "Direct3D 12 device lost on present: Reason code 0x%08X", static_cast<unsigned int>(result))

				// TODO(co) Add device lost handling if needed. Probably more complex to recreate all device resources.
		}
	}

	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Mapping.h                               ]
	//[-------------------------------------------------------]
	class Mapping final
	{
	public:
		[[nodiscard]] static DXGI_FORMAT getDirect3D12Format(Rhi::VertexAttributeFormat vertexAttributeFormat)
		{
			static constexpr DXGI_FORMAT MAPPING[] =
			{
				DXGI_FORMAT_R32_FLOAT,			// Rhi::VertexAttributeFormat::FLOAT_1
				DXGI_FORMAT_R32G32_FLOAT,		// Rhi::VertexAttributeFormat::FLOAT_2
				DXGI_FORMAT_R32G32B32_FLOAT,	// Rhi::VertexAttributeFormat::FLOAT_3
				DXGI_FORMAT_R32G32B32A32_FLOAT,	// Rhi::VertexAttributeFormat::FLOAT_4
				DXGI_FORMAT_R8G8B8A8_UNORM,		// Rhi::VertexAttributeFormat::R8G8B8A8_UNORM
				DXGI_FORMAT_R8G8B8A8_UINT,		// Rhi::VertexAttributeFormat::R8G8B8A8_UINT
				DXGI_FORMAT_R16G16_SINT,		// Rhi::VertexAttributeFormat::SHORT_2
				DXGI_FORMAT_R16G16B16A16_SINT,	// Rhi::VertexAttributeFormat::SHORT_4
				DXGI_FORMAT_R32_UINT			// Rhi::VertexAttributeFormat::UINT_1
			};
			return MAPPING[static_cast<int>(vertexAttributeFormat)];
		}

		//[-------------------------------------------------------]
		//[ Rhi::BufferUsage                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "Rhi::BufferUsage" to Direct3D 12 usage and CPU access flags
		*
		*  @param[in]  bufferUsage
		*    "Rhi::BufferUsage" to map
		*  @param[out] cpuAccessFlags
		*    Receives the CPU access flags
		*
		*  @return
		*    Direct3D 12 usage // TODO(co) Use correct Direct3D 12 type
		*/
		[[nodiscard]] static uint32_t getDirect3D12UsageAndCPUAccessFlags([[maybe_unused]] Rhi::BufferUsage bufferUsage, [[maybe_unused]] uint32_t& cpuAccessFlags)
		{
			// TODO(co) Direct3D 12
			/*
			// Direct3D 12 only supports a subset of the OpenGL usage indications
			// -> See "D3D12_USAGE enumeration "-documentation at http://msdn.microsoft.com/en-us/library/windows/desktop/ff476259%28v=vs.85%29.aspx
			switch (bufferUsage)
			{
				case Rhi::BufferUsage::STREAM_DRAW:
				case Rhi::BufferUsage::STREAM_COPY:
				case Rhi::BufferUsage::STATIC_DRAW:
				case Rhi::BufferUsage::STATIC_COPY:
					cpuAccessFlags = 0;
					return D3D12_USAGE_IMMUTABLE;

				case Rhi::BufferUsage::STREAM_READ:
				case Rhi::BufferUsage::STATIC_READ:
					cpuAccessFlags = D3D12_CPU_ACCESS_READ;
					return D3D12_USAGE_STAGING;

				case Rhi::BufferUsage::DYNAMIC_DRAW:
				case Rhi::BufferUsage::DYNAMIC_COPY:
					cpuAccessFlags = D3D12_CPU_ACCESS_WRITE;
					return D3D12_USAGE_DYNAMIC;

				default:
				case Rhi::BufferUsage::DYNAMIC_READ:
					cpuAccessFlags = 0;
					return D3D12_USAGE_DEFAULT;
			}
			*/
			return 0;
		}

		//[-------------------------------------------------------]
		//[ Rhi::IndexBufferFormat                                ]
		//[-------------------------------------------------------]
		[[nodiscard]] static DXGI_FORMAT getDirect3D12Format(Rhi::IndexBufferFormat::Enum indexBufferFormat)
		{
			static constexpr DXGI_FORMAT MAPPING[] =
			{
				DXGI_FORMAT_R32_UINT,	// Rhi::IndexBufferFormat::UNSIGNED_CHAR  - One byte per element, uint8_t (may not be supported by each API) - Not supported by Direct3D 12
				DXGI_FORMAT_R16_UINT,	// Rhi::IndexBufferFormat::UNSIGNED_SHORT - Two bytes per element, uint16_t
				DXGI_FORMAT_R32_UINT	// Rhi::IndexBufferFormat::UNSIGNED_INT   - Four bytes per element, uint32_t (may not be supported by each API)
			};
			return MAPPING[indexBufferFormat];
		}

		//[-------------------------------------------------------]
		//[ Rhi::TextureFormat                                    ]
		//[-------------------------------------------------------]
		[[nodiscard]] static DXGI_FORMAT getDirect3D12Format(Rhi::TextureFormat::Enum textureFormat)
		{
			static constexpr DXGI_FORMAT MAPPING[] =
			{
				DXGI_FORMAT_R8_UNORM,				// Rhi::TextureFormat::R8            - 8-bit pixel format, all bits red
				DXGI_FORMAT_B8G8R8X8_UNORM,			// Rhi::TextureFormat::R8G8B8        - 24-bit pixel format, 8 bits for red, green and blue
				DXGI_FORMAT_R8G8B8A8_UNORM,			// Rhi::TextureFormat::R8G8B8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
				DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,	// Rhi::TextureFormat::R8G8B8A8_SRGB - 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				DXGI_FORMAT_B8G8R8A8_UNORM,			// Rhi::TextureFormat::B8G8R8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
				DXGI_FORMAT_R11G11B10_FLOAT,		// Rhi::TextureFormat::R11G11B10F    - 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent
				DXGI_FORMAT_R16G16B16A16_FLOAT,		// Rhi::TextureFormat::R16G16B16A16F - 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
				DXGI_FORMAT_R32G32B32A32_FLOAT,		// Rhi::TextureFormat::R32G32B32A32F - 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
				DXGI_FORMAT_BC1_UNORM,				// Rhi::TextureFormat::BC1           - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block)
				DXGI_FORMAT_BC1_UNORM_SRGB,			// Rhi::TextureFormat::BC1_SRGB      - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				DXGI_FORMAT_BC2_UNORM,				// Rhi::TextureFormat::BC2           - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
				DXGI_FORMAT_BC2_UNORM_SRGB,			// Rhi::TextureFormat::BC2_SRGB      - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				DXGI_FORMAT_BC3_UNORM,				// Rhi::TextureFormat::BC3           - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
				DXGI_FORMAT_BC3_UNORM_SRGB,			// Rhi::TextureFormat::BC3_SRGB      - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				DXGI_FORMAT_BC4_UNORM,				// Rhi::TextureFormat::BC4           - 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block)
				DXGI_FORMAT_BC5_UNORM,				// Rhi::TextureFormat::BC5           - 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block)
				DXGI_FORMAT_UNKNOWN,				// Rhi::TextureFormat::ETC1          - 3 component texture compression meant for mobile devices - not supported in Direct3D 12
				DXGI_FORMAT_R16_UNORM,				// Rhi::TextureFormat::R16_UNORM     - 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
				DXGI_FORMAT_R32_UINT,				// Rhi::TextureFormat::R32_UINT      - 32-bit unsigned integer format
				DXGI_FORMAT_R32_FLOAT,				// Rhi::TextureFormat::R32_FLOAT     - 32-bit float format
				DXGI_FORMAT_D32_FLOAT,				// Rhi::TextureFormat::D32_FLOAT     - 32-bit float depth format
				DXGI_FORMAT_R16G16_SNORM,			// Rhi::TextureFormat::R16G16_SNORM  - A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
				DXGI_FORMAT_R16G16_FLOAT,			// Rhi::TextureFormat::R16G16_FLOAT  - A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
				DXGI_FORMAT_UNKNOWN					// Rhi::TextureFormat::UNKNOWN       - Unknown
			};
			return MAPPING[textureFormat];
		}
	};

	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/TextureHelper.h                         ]
	//[-------------------------------------------------------]
	class TextureHelper final
	{
	public:
		enum class TextureType
		{
			TEXTURE_1D,
			TEXTURE_1D_ARRAY,
			TEXTURE_2D,
			TEXTURE_2D_ARRAY,
			TEXTURE_CUBE,
			TEXTURE_CUBE_ARRAY,
			TEXTURE_3D
		};

	public:
		[[nodiscard]] static ID3D12Resource* CreateTexture(ID3D12Device& d3d12Device, TextureType textureType, uint32_t width, uint32_t height, uint32_t depth, uint32_t numberOfSlices, Rhi::TextureFormat::Enum textureFormat, uint8_t numberOfMultisamples, uint32_t numberOfMipmaps, uint32_t textureFlags, const Rhi::OptimizedTextureClearValue* optimizedTextureClearValue)
		{
			D3D12_HEAP_PROPERTIES d3d12HeapProperties = {};
			d3d12HeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

			// Get Direct3D 12 resource description
			D3D12_RESOURCE_DESC d3d12ResourceDesc = {};
			d3d12ResourceDesc.Dimension = (textureType <= TextureType::TEXTURE_1D_ARRAY) ? D3D12_RESOURCE_DIMENSION_TEXTURE1D : ((textureType <= TextureType::TEXTURE_CUBE_ARRAY) ? D3D12_RESOURCE_DIMENSION_TEXTURE2D : D3D12_RESOURCE_DIMENSION_TEXTURE3D);
			d3d12ResourceDesc.Width = width;
			d3d12ResourceDesc.Height = height;
			d3d12ResourceDesc.DepthOrArraySize = static_cast<uint16_t>((TextureType::TEXTURE_3D == textureType) ? depth : numberOfSlices);
			d3d12ResourceDesc.MipLevels = static_cast<uint16_t>(numberOfMipmaps);
			d3d12ResourceDesc.Format = Mapping::getDirect3D12Format(textureFormat);
			d3d12ResourceDesc.SampleDesc.Count = numberOfMultisamples;
			d3d12ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

			{ // Get Direct3D 12 resource description flags
				uint32_t descriptionFlags = 0;
				if ( textureFlags & Rhi::TextureFlag::RENDER_TARGET )
				{
					descriptionFlags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
					if ( Rhi::TextureFormat::isDepth(textureFormat) )
					{
						descriptionFlags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
						if ( (textureFlags & Rhi::TextureFlag::SHADER_RESOURCE) == 0 )
						{
							descriptionFlags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
						}
					}
				}
				if ( textureFlags & Rhi::TextureFlag::UNORDERED_ACCESS )
				{
					descriptionFlags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
				}
				d3d12ResourceDesc.Flags = static_cast<D3D12_RESOURCE_FLAGS>(descriptionFlags);
			}

			// Get Direct3D 12 resource states and clear value
			D3D12_RESOURCE_STATES d3d12ResourceStates = D3D12_RESOURCE_STATE_COPY_DEST;
			D3D12_CLEAR_VALUE d3d12ClearValue = {};
			if ( textureFlags & Rhi::TextureFlag::RENDER_TARGET )
			{
				if ( Rhi::TextureFormat::isDepth(textureFormat) )
				{
					d3d12ResourceStates = D3D12_RESOURCE_STATE_DEPTH_WRITE;
					d3d12ClearValue.Format = d3d12ResourceDesc.Format;
					if ( nullptr != optimizedTextureClearValue )
					{
						d3d12ClearValue.DepthStencil.Depth = optimizedTextureClearValue->DepthStencil.depth;
					}
				}
				else
				{
					d3d12ResourceStates = D3D12_RESOURCE_STATE_RENDER_TARGET;
					if ( nullptr != optimizedTextureClearValue )
					{
						d3d12ClearValue.Format = d3d12ResourceDesc.Format;
						memcpy(d3d12ClearValue.Color, optimizedTextureClearValue->color, sizeof(float) * 4);
					}
				}
			}

			// Create the Direct3D 12 texture resource
			ID3D12Resource* d3d12Texture = nullptr;
			const HRESULT result = d3d12Device.CreateCommittedResource(&d3d12HeapProperties, D3D12_HEAP_FLAG_NONE, &d3d12ResourceDesc, d3d12ResourceStates, d3d12ClearValue.Format ? &d3d12ClearValue : nullptr, __uuidof(ID3D12Resource), reinterpret_cast<void**>(&d3d12Texture));
			return (SUCCEEDED(result) ? d3d12Texture : nullptr);
		}

		static void SetTextureData(::detail::UploadContext& uploadContext, ID3D12Resource& d3d12Resource, uint32_t width, uint32_t height, uint32_t depth, Rhi::TextureFormat::Enum textureFormat, uint32_t numberOfMipmaps, uint32_t mip, uint32_t slice, const void* data, [[maybe_unused]] uint32_t size, uint32_t pitch)
		{
			// TODO(co) This should never ever happen
			if ( nullptr == uploadContext.getUploadCommandListAllocator() )
			{
				return;
			}

			const D3D12_RESOURCE_DESC d3d12ResourceDesc = d3d12Resource.GetDesc();

			// Texture copy destination
			D3D12_TEXTURE_COPY_LOCATION d3d12TextureCopyLocationDestination;
			d3d12TextureCopyLocationDestination.pResource = &d3d12Resource;
			d3d12TextureCopyLocationDestination.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;

			// Texture copy source
			D3D12_TEXTURE_COPY_LOCATION d3d12TextureCopyLocationSource;
			d3d12TextureCopyLocationSource.pResource = uploadContext.getUploadCommandListAllocator()->getD3D12ResourceUploadBuffer();
			d3d12TextureCopyLocationSource.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
			d3d12TextureCopyLocationSource.PlacedFootprint.Footprint.Format = d3d12ResourceDesc.Format;

			// Get the number of rows
			uint32_t numberOfColumns = width;
			uint32_t numberOfRows = height;
			const bool isCompressed = Rhi::TextureFormat::isCompressed(textureFormat);
			if ( isCompressed )
			{
				numberOfColumns = (numberOfColumns + 3) >> 2;
				numberOfRows = (numberOfRows + 3) >> 2;
			}
			numberOfRows *= depth;
			SE_ASSERT(pitch * numberOfRows == size, "Direct3D 12: Invalid size")

				// Grab upload buffer space
				const uint32_t destinationPitch = ::detail::align(pitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);
			const uint32_t destinationOffset = uploadContext.getUploadCommandListAllocator()->allocateUploadBuffer(destinationPitch * numberOfRows, D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT);

			// Copy data in place
			const uint8_t* sourceData = reinterpret_cast<const uint8_t*>(data);
			uint8_t* destinationData = uploadContext.getUploadCommandListAllocator()->getData() + destinationOffset;
			const uint32_t sourcePitch = pitch;
			for ( uint32_t r = 0; r < numberOfRows; ++r )
			{
				memcpy(destinationData, sourceData, sourcePitch);
				destinationData += destinationPitch;
				sourceData += sourcePitch;
			}

			// Issue a copy from upload buffer to texture
			d3d12TextureCopyLocationDestination.SubresourceIndex = mip + slice * numberOfMipmaps;
			d3d12TextureCopyLocationSource.PlacedFootprint.Offset = destinationOffset;
			if ( isCompressed )
			{
				d3d12TextureCopyLocationSource.PlacedFootprint.Footprint.Width = ::detail::align(width, 4);
				d3d12TextureCopyLocationSource.PlacedFootprint.Footprint.Height = ::detail::align(height, 4);
			}
			else
			{
				d3d12TextureCopyLocationSource.PlacedFootprint.Footprint.Width = width;
				d3d12TextureCopyLocationSource.PlacedFootprint.Footprint.Height = height;
			}
			d3d12TextureCopyLocationSource.PlacedFootprint.Footprint.Depth = depth;
			d3d12TextureCopyLocationSource.PlacedFootprint.Footprint.RowPitch = destinationPitch;
			uploadContext.getD3d12GraphicsCommandList()->CopyTextureRegion(&d3d12TextureCopyLocationDestination, 0, 0, 0, &d3d12TextureCopyLocationSource, nullptr);

			D3D12_RESOURCE_BARRIER d3d12ResourceBarrier;
			d3d12ResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			d3d12ResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			d3d12ResourceBarrier.Transition.pResource = &d3d12Resource;
			d3d12ResourceBarrier.Transition.Subresource = d3d12TextureCopyLocationDestination.SubresourceIndex;
			d3d12ResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
			d3d12ResourceBarrier.Transition.StateAfter = static_cast<D3D12_RESOURCE_STATES>(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
			uploadContext.getD3d12GraphicsCommandList()->ResourceBarrier(1, &d3d12ResourceBarrier);
		}
	};

	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/RootSignature.h                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 root signature ("pipeline layout" in Vulkan terminology) class
	*/
	class RootSignature final : public Rhi::IRootSignature
	{
		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] rootSignature
		*    Root signature to use
		*/
		RootSignature(Direct3D12Rhi& direct3D12Rhi, const Rhi::RootSignature& rootSignature RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IRootSignature(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mRootSignature(rootSignature),
			mD3D12RootSignature(nullptr)
		{
			{ // We need a backup of the given root signature
				{ // Copy the parameter data
					const uint32_t numberOfParameters = mRootSignature.numberOfParameters;
					if ( numberOfParameters > 0 )
					{
						mRootSignature.parameters = RHI_MALLOC_TYPED(Rhi::RootParameter, numberOfParameters);
						Rhi::RootParameter* destinationRootParameters = const_cast<Rhi::RootParameter*>(mRootSignature.parameters);
						memcpy(destinationRootParameters, rootSignature.parameters, sizeof(Rhi::RootParameter) * numberOfParameters);

						// Copy the descriptor table data
						for ( uint32_t i = 0; i < numberOfParameters; ++i )
						{
							Rhi::RootParameter& destinationRootParameter = destinationRootParameters[i];
							const Rhi::RootParameter& sourceRootParameter = rootSignature.parameters[i];
							if ( Rhi::RootParameterType::DESCRIPTOR_TABLE == destinationRootParameter.parameterType )
							{
								const uint32_t numberOfDescriptorRanges = destinationRootParameter.descriptorTable.numberOfDescriptorRanges;
								destinationRootParameter.descriptorTable.descriptorRanges = reinterpret_cast<uintptr_t>(RHI_MALLOC_TYPED(Rhi::DescriptorRange, numberOfDescriptorRanges));
								memcpy(reinterpret_cast<Rhi::DescriptorRange*>(destinationRootParameter.descriptorTable.descriptorRanges), reinterpret_cast<const Rhi::DescriptorRange*>(sourceRootParameter.descriptorTable.descriptorRanges), sizeof(Rhi::DescriptorRange) * numberOfDescriptorRanges);
							}
						}
					}
				}

				{ // Copy the static sampler data
					const uint32_t numberOfStaticSamplers = mRootSignature.numberOfStaticSamplers;
					if ( numberOfStaticSamplers > 0 )
					{
						mRootSignature.staticSamplers = RHI_MALLOC_TYPED(Rhi::StaticSampler, numberOfStaticSamplers);
						memcpy(const_cast<Rhi::StaticSampler*>(mRootSignature.staticSamplers), rootSignature.staticSamplers, sizeof(Rhi::StaticSampler) * numberOfStaticSamplers);
					}
				}
			}

			// Create temporary Direct3D 12 root signature instance data
			// -> "Rhi::RootSignature" is not identical to "D3D12_ROOT_SIGNATURE_DESC" because it had to be extended by information required by OpenGL
			D3D12_ROOT_SIGNATURE_DESC d3d12RootSignatureDesc;
			{
				{ // Copy the parameter data
					const uint32_t numberOfRootParameters = rootSignature.numberOfParameters;
					d3d12RootSignatureDesc.NumParameters = numberOfRootParameters;
					if ( numberOfRootParameters > 0 )
					{
						d3d12RootSignatureDesc.pParameters = RHI_MALLOC_TYPED(D3D12_ROOT_PARAMETER, numberOfRootParameters);
						D3D12_ROOT_PARAMETER* d3dRootParameters = const_cast<D3D12_ROOT_PARAMETER*>(d3d12RootSignatureDesc.pParameters);
						for ( uint32_t parameterIndex = 0; parameterIndex < numberOfRootParameters; ++parameterIndex )
						{
							D3D12_ROOT_PARAMETER& d3dRootParameter = d3dRootParameters[parameterIndex];
							const Rhi::RootParameter& rootParameter = rootSignature.parameters[parameterIndex];

							// Copy the descriptor table data and determine the shader visibility of the Direct3D 12 root parameter
							uint32_t shaderVisibility = ~0u;
							if ( Rhi::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType )
							{
								const uint32_t numberOfDescriptorRanges = rootParameter.descriptorTable.numberOfDescriptorRanges;
								d3dRootParameter.DescriptorTable.NumDescriptorRanges = numberOfDescriptorRanges;
								d3dRootParameter.DescriptorTable.pDescriptorRanges = RHI_MALLOC_TYPED(D3D12_DESCRIPTOR_RANGE, numberOfDescriptorRanges);

								// "Rhi::DescriptorRange" is not identical to "D3D12_DESCRIPTOR_RANGE" because it had to be extended by information required by OpenGL
								for ( uint32_t descriptorRangeIndex = 0; descriptorRangeIndex < numberOfDescriptorRanges; ++descriptorRangeIndex )
								{
									const Rhi::DescriptorRange& descriptorRange = reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges)[descriptorRangeIndex];
									memcpy(const_cast<D3D12_DESCRIPTOR_RANGE*>(&d3dRootParameter.DescriptorTable.pDescriptorRanges[descriptorRangeIndex]), &descriptorRange, sizeof(D3D12_DESCRIPTOR_RANGE));
									if ( ~0u == shaderVisibility )
									{
										shaderVisibility = static_cast<uint32_t>(descriptorRange.shaderVisibility);
										if ( shaderVisibility == static_cast<uint32_t>(Rhi::ShaderVisibility::COMPUTE) || shaderVisibility == static_cast<uint32_t>(Rhi::ShaderVisibility::ALL_GRAPHICS) )
										{
											shaderVisibility = static_cast<uint32_t>(Rhi::ShaderVisibility::ALL);
										}
									}
									else if ( shaderVisibility != static_cast<uint32_t>(descriptorRange.shaderVisibility) )
									{
										shaderVisibility = static_cast<uint32_t>(Rhi::ShaderVisibility::ALL);
									}
								}
							}
							if ( ~0u == shaderVisibility )
							{
								shaderVisibility = static_cast<uint32_t>(Rhi::ShaderVisibility::ALL);
							}

							// Set root parameter
							d3dRootParameter.ParameterType = static_cast<D3D12_ROOT_PARAMETER_TYPE>(rootParameter.parameterType);
							d3dRootParameter.ShaderVisibility = static_cast<D3D12_SHADER_VISIBILITY>(shaderVisibility);
						}
					}
					else
					{
						d3d12RootSignatureDesc.pParameters = nullptr;
					}
				}

				{ // Copy the static sampler data
				  // -> "Rhi::StaticSampler" is identical to "D3D12_STATIC_SAMPLER_DESC" so there's no additional mapping work to be done in here
					const uint32_t numberOfStaticSamplers = rootSignature.numberOfStaticSamplers;
					d3d12RootSignatureDesc.NumStaticSamplers = numberOfStaticSamplers;
					if ( numberOfStaticSamplers > 0 )
					{
						d3d12RootSignatureDesc.pStaticSamplers = RHI_MALLOC_TYPED(D3D12_STATIC_SAMPLER_DESC, numberOfStaticSamplers);
						memcpy(const_cast<D3D12_STATIC_SAMPLER_DESC*>(d3d12RootSignatureDesc.pStaticSamplers), rootSignature.staticSamplers, sizeof(Rhi::StaticSampler) * numberOfStaticSamplers);
					}
					else
					{
						d3d12RootSignatureDesc.pStaticSamplers = nullptr;
					}
				}

				// Copy flags
				// -> "Rhi::RootSignatureFlags" is identical to "D3D12_ROOT_SIGNATURE_FLAGS" so there's no additional mapping work to be done in here
				d3d12RootSignatureDesc.Flags = static_cast<D3D12_ROOT_SIGNATURE_FLAGS>(rootSignature.flags);
			}

			{ // Create the Direct3D 12 root signature instance
				ID3DBlob* d3dBlobSignature = nullptr;
				ID3DBlob* d3dBlobError = nullptr;
				if ( SUCCEEDED(D3D12SerializeRootSignature(&d3d12RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &d3dBlobSignature, &d3dBlobError)) )
				{
					if ( SUCCEEDED(direct3D12Rhi.getD3D12Device().CreateRootSignature(0, d3dBlobSignature->GetBufferPointer(), d3dBlobSignature->GetBufferSize(), IID_PPV_ARGS(&mD3D12RootSignature))) )
					{
						// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
						RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Root signature", 17)	// 17 = "Root signature: " including terminating zero
							FAILED_DEBUG_BREAK(mD3D12RootSignature->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
#endif
					}
					else
					{
						RHI_LOG(CRITICAL, "Failed to create the Direct3D 12 root signature instance")
					}
					d3dBlobSignature->Release();
				}
				else
				{
					RHI_LOG(CRITICAL, "Failed to create the Direct3D 12 root signature instance: %s", (nullptr != d3dBlobError) ? reinterpret_cast<char*>(d3dBlobError->GetBufferPointer()) : "Unknown error")
						if ( nullptr != d3dBlobError )
						{
							d3dBlobError->Release();
						}
				}
			}

			// Free temporary Direct3D 12 root signature instance data
			if ( nullptr != d3d12RootSignatureDesc.pParameters )
			{
				for ( uint32_t i = 0; i < d3d12RootSignatureDesc.NumParameters; ++i )
				{
					const D3D12_ROOT_PARAMETER& d3d12RootParameter = d3d12RootSignatureDesc.pParameters[i];
					if ( D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE == d3d12RootParameter.ParameterType )
					{
						RHI_FREE(const_cast<D3D12_DESCRIPTOR_RANGE*>(d3d12RootParameter.DescriptorTable.pDescriptorRanges));
					}
				}
				RHI_FREE(const_cast<D3D12_ROOT_PARAMETER*>(d3d12RootSignatureDesc.pParameters));
			}
			RHI_FREE(const_cast<D3D12_STATIC_SAMPLER_DESC*>(d3d12RootSignatureDesc.pStaticSamplers));
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RootSignature() override
		{
			// Release the Direct3D 12 root signature
			if ( nullptr != mD3D12RootSignature )
			{
				mD3D12RootSignature->Release();
			}

			// Destroy the backup of the given root signature
			if ( nullptr != mRootSignature.parameters )
			{
				for ( uint32_t i = 0; i < mRootSignature.numberOfParameters; ++i )
				{
					const Rhi::RootParameter& rootParameter = mRootSignature.parameters[i];
					if ( Rhi::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType )
					{
						RHI_FREE(reinterpret_cast<Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges));
					}
				}
				RHI_FREE(const_cast<Rhi::RootParameter*>(mRootSignature.parameters));
			}
			RHI_FREE(const_cast<Rhi::StaticSampler*>(mRootSignature.staticSamplers));
		}

		/**
		*  @brief
		*    Return the root signature data
		*
		*  @return
		*    The root signature data
		*/
		[[nodiscard]] inline const Rhi::RootSignature& getRootSignature() const
		{
			return mRootSignature;
		}

		/**
		*  @brief
		*    Return the Direct3D 12 root signature
		*
		*  @return
		*    The Direct3D 12 root signature, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3D12RootSignature* getD3D12RootSignature() const
		{
			return mD3D12RootSignature;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IRootSignature methods            ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual Rhi::IResourceGroup* createResourceGroup(uint32_t rootParameterIndex, uint32_t numberOfResources, Rhi::IResource** resources, Rhi::ISamplerState** samplerStates = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(RootSignature, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit RootSignature(const RootSignature&) = delete;
		RootSignature& operator =(const RootSignature&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		Rhi::RootSignature	 mRootSignature;
		ID3D12RootSignature* mD3D12RootSignature;	// Direct3D 12 root signature, can be a null pointer


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Buffer/VertexBuffer.h                   ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 vertex buffer object (VBO, "array buffer" in OpenGL terminology) class
	*/
	class VertexBuffer final : public Rhi::IVertexBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the vertex buffer, must be valid
		*  @param[in] data
		*    Vertex buffer data, can be a null pointer (empty buffer)
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*/
		VertexBuffer(Direct3D12Rhi& direct3D12Rhi, uint32_t numberOfBytes, const void* data, [[maybe_unused]] Rhi::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IVertexBuffer(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mNumberOfBytes(numberOfBytes),
			mD3D12Resource(nullptr)
		{
			// TODO(co) This is only meant for the Direct3D 12 RHI implementation kickoff.
			// Note: using upload heaps to transfer static data like vert buffers is not 
			// recommended. Every time the GPU needs it, the upload heap will be marshalled 
			// over. Please read up on Default Heap usage. An upload heap is used here for 
			// code simplicity and because there are very few verts to actually transfer.

			// TODO(co) Add buffer usage setting support

			const CD3DX12_HEAP_PROPERTIES d3d12XHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
			const CD3DX12_RESOURCE_DESC d3d12XResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(mNumberOfBytes);
			if ( SUCCEEDED(direct3D12Rhi.getD3D12Device().CreateCommittedResource(
				&d3d12XHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&d3d12XResourceDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&mD3D12Resource))) )
			{
				// Data given?
				if ( nullptr != data )
				{
					// Copy the data to the vertex buffer
					UINT8* pVertexDataBegin;
					CD3DX12_RANGE readRange(0, 0);	// We do not intend to read from this resource on the CPU
					if ( SUCCEEDED(mD3D12Resource->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin))) )
					{
						memcpy(pVertexDataBegin, data, mNumberOfBytes);
						mD3D12Resource->Unmap(0, nullptr);
					}
					else
					{
						RHI_LOG(CRITICAL, "Failed to map Direct3D 12 vertex buffer")
					}
				}

				// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "VBO", 6)	// 6 = "VBO: " including terminating zero
					FAILED_DEBUG_BREAK(mD3D12Resource->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
#endif
			}
			else
			{
				RHI_LOG(CRITICAL, "Failed to create Direct3D 12 vertex buffer resource")
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~VertexBuffer() override
		{
			if ( nullptr != mD3D12Resource )
			{
				mD3D12Resource->Release();
			}
		}

		/**
		*  @brief
		*    Return the number of bytes within the vertex buffer
		*
		*  @return
		*    The number of bytes within the vertex buffer
		*/
		[[nodiscard]] inline uint32_t getNumberOfBytes() const
		{
			return mNumberOfBytes;
		}

		/**
		*  @brief
		*    Return the Direct3D vertex buffer resource instance
		*
		*  @return
		*    The Direct3D vertex buffer resource instance, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3D12Resource* getD3D12Resource() const
		{
			return mD3D12Resource;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(VertexBuffer, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator =(const VertexBuffer&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		uint32_t		mNumberOfBytes;	// Number of bytes within the vertex buffer
		ID3D12Resource* mD3D12Resource;


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Buffer/IndexBuffer.h                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 index buffer object (IBO, "element array buffer" in OpenGL terminology) class
	*/
	class IndexBuffer final : public Rhi::IIndexBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the index buffer, must be valid
		*  @param[in] data
		*    Index buffer data, can be a null pointer (empty buffer)
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*  @param[in] indexBufferFormat
		*    Index buffer data format
		*/
		IndexBuffer(Direct3D12Rhi& direct3D12Rhi, uint32_t numberOfBytes, const void* data, [[maybe_unused]] Rhi::BufferUsage bufferUsage, Rhi::IndexBufferFormat::Enum indexBufferFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IIndexBuffer(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mD3D12Resource(nullptr)
		{
			// "Rhi::IndexBufferFormat::UnsignedChar" is not supported by Direct3D 12
			// TODO(co) Check this, there's "DXGI_FORMAT_R8_UINT" which might work in Direct3D 12
			if ( Rhi::IndexBufferFormat::UNSIGNED_CHAR == indexBufferFormat )
			{
				RHI_LOG(CRITICAL, "\"Rhi::IndexBufferFormat::UNSIGNED_CHAR\" is not supported by Direct3D 12")
					mD3D12IndexBufferView.BufferLocation = 0;
				mD3D12IndexBufferView.SizeInBytes = 0;
				mD3D12IndexBufferView.Format = DXGI_FORMAT_UNKNOWN;
			}
			else
			{
				// TODO(co) This is only meant for the Direct3D 12 RHI implementation kickoff.
				// Note: using upload heaps to transfer static data like vert buffers is not 
				// recommended. Every time the GPU needs it, the upload heap will be marshalled 
				// over. Please read up on Default Heap usage. An upload heap is used here for 
				// code simplicity and because there are very few verts to actually transfer.

				// TODO(co) Add buffer usage setting support

				const CD3DX12_HEAP_PROPERTIES d3d12XHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
				const CD3DX12_RESOURCE_DESC d3d12XResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(numberOfBytes);
				if ( SUCCEEDED(direct3D12Rhi.getD3D12Device().CreateCommittedResource(
					&d3d12XHeapProperties,
					D3D12_HEAP_FLAG_NONE,
					&d3d12XResourceDesc,
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&mD3D12Resource))) )
				{
					// Data given?
					if ( nullptr != data )
					{
						// Copy the data to the index buffer
						UINT8* pIndexDataBegin;
						CD3DX12_RANGE readRange(0, 0);	// We do not intend to read from this resource on the CPU
						if ( SUCCEEDED(mD3D12Resource->Map(0, &readRange, reinterpret_cast<void**>(&pIndexDataBegin))) )
						{
							memcpy(pIndexDataBegin, data, numberOfBytes);
							mD3D12Resource->Unmap(0, nullptr);
						}
						else
						{
							RHI_LOG(CRITICAL, "Failed to map Direct3D 12 index buffer")
						}
					}

					// Fill the Direct3D 12 index buffer view
					mD3D12IndexBufferView.BufferLocation = mD3D12Resource->GetGPUVirtualAddress();
					mD3D12IndexBufferView.SizeInBytes = numberOfBytes;
					mD3D12IndexBufferView.Format = Mapping::getDirect3D12Format(indexBufferFormat);

					// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "IBO", 6)	// 6 = "IBO: " including terminating zero
						FAILED_DEBUG_BREAK(mD3D12Resource->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
#endif
				}
				else
				{
					RHI_LOG(CRITICAL, "Failed to create Direct3D 12 index buffer resource")
						mD3D12IndexBufferView.BufferLocation = 0;
					mD3D12IndexBufferView.SizeInBytes = 0;
					mD3D12IndexBufferView.Format = DXGI_FORMAT_UNKNOWN;
				}
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~IndexBuffer() override
		{
			if ( nullptr != mD3D12Resource )
			{
				mD3D12Resource->Release();
			}
		}

		/**
		*  @brief
		*    Return the Direct3D index buffer resource instance
		*
		*  @return
		*    The Direct3D index buffer resource instance, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3D12Resource* getD3D12Resource() const
		{
			return mD3D12Resource;
		}

		/**
		*  @brief
		*    Return the Direct3D 12 index buffer view
		*
		*  @return
		*    The Direct3D 12 index buffer view
		*/
		[[nodiscard]] inline const D3D12_INDEX_BUFFER_VIEW& getD3D12IndexBufferView() const
		{
			return mD3D12IndexBufferView;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(IndexBuffer, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator =(const IndexBuffer&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		ID3D12Resource*			mD3D12Resource;
		D3D12_INDEX_BUFFER_VIEW	mD3D12IndexBufferView;


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Buffer/VertexArray.h                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 vertex array class
	*/
	class VertexArray final : public Rhi::IVertexArray
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] vertexAttributes
		*    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
		*  @param[in] numberOfVertexBuffers
		*    Number of vertex buffers, having zero vertex buffers is valid
		*  @param[in] vertexBuffers
		*    At least numberOfVertexBuffers instances of vertex array vertex buffers, can be a null pointer in case there are zero vertex buffers, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] indexBuffer
		*    Optional index buffer to use, can be a null pointer, the vertex array instance keeps a reference to the index buffer
		*  @param[in] id
		*    The unique compact vertex array ID
		*/
		VertexArray(Direct3D12Rhi& direct3D12Rhi, const Rhi::VertexAttributes& vertexAttributes, uint32_t numberOfVertexBuffers, const Rhi::VertexArrayVertexBuffer* vertexBuffers, IndexBuffer* indexBuffer, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IVertexArray(direct3D12Rhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mIndexBuffer(indexBuffer),
			mNumberOfSlots(numberOfVertexBuffers),
			mD3D12VertexBufferViews(nullptr),
			mVertexBuffers(nullptr)
		{
			// Add a reference to the given index buffer
			if ( nullptr != mIndexBuffer )
			{
				mIndexBuffer->AddReference();
			}

			// Add a reference to the used vertex buffers
			if ( mNumberOfSlots > 0 )
			{
				mD3D12VertexBufferViews = RHI_MALLOC_TYPED(D3D12_VERTEX_BUFFER_VIEW, mNumberOfSlots);
				mVertexBuffers = RHI_MALLOC_TYPED(VertexBuffer*, mNumberOfSlots);

				{ // Loop through all vertex buffers
					D3D12_VERTEX_BUFFER_VIEW* currentD3D12VertexBufferView = mD3D12VertexBufferViews;
					VertexBuffer** currentVertexBuffer = mVertexBuffers;
					const Rhi::VertexArrayVertexBuffer* vertexBufferEnd = vertexBuffers + mNumberOfSlots;
					for ( const Rhi::VertexArrayVertexBuffer* vertexBuffer = vertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer, ++currentD3D12VertexBufferView, ++currentVertexBuffer )
					{
						// TODO(co) Add security check: Is the given resource one of the currently used RHI?
						*currentVertexBuffer = static_cast<VertexBuffer*>(vertexBuffer->vertexBuffer);
						(*currentVertexBuffer)->AddReference();
						currentD3D12VertexBufferView->BufferLocation = (*currentVertexBuffer)->getD3D12Resource()->GetGPUVirtualAddress();
						currentD3D12VertexBufferView->SizeInBytes = (*currentVertexBuffer)->getNumberOfBytes();
					}
				}

				{ // Gather slot related data
					const Rhi::VertexAttribute* attribute = vertexAttributes.attributes;
					const Rhi::VertexAttribute* attributesEnd = attribute + vertexAttributes.numberOfAttributes;
					for ( ; attribute < attributesEnd; ++attribute )
					{
						mD3D12VertexBufferViews[attribute->inputSlot].StrideInBytes = attribute->strideInBytes;
					}
				}
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~VertexArray() override
		{
			// Release the index buffer reference
			if ( nullptr != mIndexBuffer )
			{
				mIndexBuffer->ReleaseReference();
			}

			// Cleanup Direct3D 12 input slot data, if needed
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());
			RHI_FREE(mD3D12VertexBufferViews);

			// Release the reference to the used vertex buffers
			if ( nullptr != mVertexBuffers )
			{
				// Release references
				VertexBuffer** vertexBuffersEnd = mVertexBuffers + mNumberOfSlots;
				for ( VertexBuffer** vertexBuffer = mVertexBuffers; vertexBuffer < vertexBuffersEnd; ++vertexBuffer )
				{
					(*vertexBuffer)->ReleaseReference();
				}

				// Cleanup
				RHI_FREE(mVertexBuffers);
			}

			// Free the unique compact vertex array ID
			direct3D12Rhi.VertexArrayMakeId.DestroyID(getId());
		}

		/**
		*  @brief
		*    Return the used index buffer
		*
		*  @return
		*    The used index buffer, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline IndexBuffer* getIndexBuffer() const
		{
			return mIndexBuffer;
		}

		/**
		*  @brief
		*    Set the Direct3D 12 vertex declaration and stream source
		*
		*  @param[in] d3d12GraphicsCommandList
		*    Direct3D 12 graphics command list to feed
		*/
		void setDirect3DIASetInputLayoutAndStreamSource(ID3D12GraphicsCommandList& d3d12GraphicsCommandList) const
		{
			d3d12GraphicsCommandList.IASetVertexBuffers(0, mNumberOfSlots, mD3D12VertexBufferViews);

			// Set the used index buffer
			// -> In case of no index buffer we don't set null indices, there's not really a point in it
			if ( nullptr != mIndexBuffer )
			{
				// Set the Direct3D 12 indices
				d3d12GraphicsCommandList.IASetIndexBuffer(&static_cast<IndexBuffer*>(mIndexBuffer)->getD3D12IndexBufferView());
			}
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(VertexArray, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit VertexArray(const VertexArray&) = delete;
		VertexArray& operator =(const VertexArray&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		IndexBuffer*			  mIndexBuffer;				// Optional index buffer to use, can be a null pointer, the vertex array instance keeps a reference to the index buffer
		// Direct3D 12 input slots
		UINT					  mNumberOfSlots;			// Number of used Direct3D 12 input slots
		D3D12_VERTEX_BUFFER_VIEW* mD3D12VertexBufferViews;
		// For proper vertex buffer reference counter behaviour
		VertexBuffer**			  mVertexBuffers;			// Vertex buffers (we keep a reference to it) used by this vertex array, can be a null pointer


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Buffer/TextureBuffer.h                  ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 texture buffer object (TBO) class
	*/
	class TextureBuffer final : public Rhi::ITextureBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the texture buffer, must be valid
		*  @param[in] data
		*    Texture buffer data, can be a null pointer (empty buffer)
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*  @param[in] textureFormat
		*    Texture buffer data format
		*/
		TextureBuffer(Direct3D12Rhi& direct3D12Rhi, uint32_t numberOfBytes, const void* data, [[maybe_unused]] Rhi::BufferUsage bufferUsage, Rhi::TextureFormat::Enum textureFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITextureBuffer(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mNumberOfBytes(numberOfBytes),
			mTextureFormat(textureFormat),
			mD3D12Resource(nullptr)
		{
			// Sanity check
			SE_ASSERT((numberOfBytes % Rhi::TextureFormat::GetNumberOfBytesPerElement(textureFormat)) == 0, "The Direct3D 12 texture buffer size must be a multiple of the selected texture format bytes per texel")

				// TODO(co) This is only meant for the Direct3D 12 RHI implementation kickoff.
				// Note: using upload heaps to transfer static data like vert buffers is not 
				// recommended. Every time the GPU needs it, the upload heap will be marshalled 
				// over. Please read up on Default Heap usage. An upload heap is used here for 
				// code simplicity and because there are very few verts to actually transfer.
				const CD3DX12_HEAP_PROPERTIES d3d12XHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
			const CD3DX12_RESOURCE_DESC d3d12XResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(numberOfBytes);
			ID3D12Device& d3d12Device = direct3D12Rhi.getD3D12Device();
			if ( SUCCEEDED(d3d12Device.CreateCommittedResource(
				&d3d12XHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&d3d12XResourceDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&mD3D12Resource))) )
			{
				// Data given?
				if ( nullptr != data )
				{
					// Copy the data to the texture buffer
					UINT8* pTextureDataBegin;
					CD3DX12_RANGE readRange(0, 0);	// We do not intend to read from this resource on the CPU
					if ( SUCCEEDED(mD3D12Resource->Map(0, &readRange, reinterpret_cast<void**>(&pTextureDataBegin))) )
					{
						memcpy(pTextureDataBegin, data, numberOfBytes);
						mD3D12Resource->Unmap(0, nullptr);
					}
					else
					{
						RHI_LOG(CRITICAL, "Failed to map Direct3D 12 texture buffer")
					}
				}

				// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TBO", 6)	// 6 = "TBO: " including terminating zero
					FAILED_DEBUG_BREAK(mD3D12Resource->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
#endif
			}
			else
			{
				RHI_LOG(CRITICAL, "Failed to create Direct3D 12 texture buffer resource")
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TextureBuffer() override
		{
			if ( nullptr != mD3D12Resource )
			{
				mD3D12Resource->Release();
			}
		}

		/**
		*  @brief
		*    Return the number of bytes
		*
		*  @return
		*    The number of bytes
		*/
		[[nodiscard]] inline uint32_t getNumberOfBytes() const
		{
			return mNumberOfBytes;
		}

		/**
		*  @brief
		*    Return the texture format
		*
		*  @return
		*    The texture format
		*/
		[[nodiscard]] inline Rhi::TextureFormat::Enum getTextureFormat() const
		{
			return mTextureFormat;
		}

		/**
		*  @brief
		*    Return the Direct3D 12 texture buffer resource instance
		*
		*  @return
		*    The Direct3D 12 texture buffer resource instance, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3D12Resource* getD3D12Resource() const
		{
			return mD3D12Resource;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(TextureBuffer, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TextureBuffer(const TextureBuffer&) = delete;
		TextureBuffer& operator =(const TextureBuffer&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		uint32_t				 mNumberOfBytes;
		Rhi::TextureFormat::Enum mTextureFormat;
		ID3D12Resource*			 mD3D12Resource;


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Buffer/StructuredBuffer.h               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 structured buffer object (SBO) class
	*/
	class StructuredBuffer final : public Rhi::IStructuredBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the structured buffer, must be valid
		*  @param[in] data
		*    Structured buffer data, can be a null pointer (empty buffer)
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*  @param[in] numberOfStructureBytes
		*    Number of structure bytes
		*/
		StructuredBuffer(Direct3D12Rhi& direct3D12Rhi, [[maybe_unused]] uint32_t numberOfBytes, [[maybe_unused]] const void* data, [[maybe_unused]] Rhi::BufferUsage bufferUsage, [[maybe_unused]] uint32_t numberOfStructureBytes RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IStructuredBuffer(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
			// TODO(co) Direct3D 12 update
		//	mD3D12Buffer(nullptr),
		//	mD3D12ShaderResourceViewTexture(nullptr)
		{
			// Sanity checks
			SE_ASSERT((numberOfBytes % numberOfStructureBytes) == 0, "The Direct3D 12 structured buffer size must be a multiple of the given number of structure bytes")
				SE_ASSERT((numberOfBytes % (sizeof(float) * 4)) == 0, "Performance: The Direct3D 12 structured buffer should be aligned to a 128-bit stride, see \"Understanding Structured Buffer Performance\" by Evan Hart, posted Apr 17 2015 at 11:33AM - https://developer.nvidia.com/content/understanding-structured-buffer-performance")

				// TODO(co) Direct3D 12 update
				/*
				{ // Buffer part
					// Direct3D 12 buffer description
					D3D12_BUFFER_DESC d3d12BufferDesc;
					d3d12BufferDesc.ByteWidth           = numberOfBytes;
					d3d12BufferDesc.Usage               = Mapping::getDirect3D12UsageAndCPUAccessFlags(bufferUsage, d3d12BufferDesc.CPUAccessFlags);
					d3d12BufferDesc.BindFlags           = D3D12_BIND_SHADER_RESOURCE;
					//d3d12BufferDesc.CPUAccessFlags    = <filled above>;
					d3d12BufferDesc.MiscFlags           = 0;
					d3d12BufferDesc.StructureByteStride = 0;

					// Data given?
					if (nullptr != data)
					{
						// Direct3D 12 subresource data
						D3D12_SUBRESOURCE_DATA d3d12SubresourceData;
						d3d12SubresourceData.pSysMem          = data;
						d3d12SubresourceData.SysMemPitch      = 0;
						d3d12SubresourceData.SysMemSlicePitch = 0;

						// Create the Direct3D 12 constant buffer
						FAILED_DEBUG_BREAK(direct3D12Rhi.getD3D12Device().CreateBuffer(&d3d12BufferDesc, &d3d12SubresourceData, &mD3D12Buffer))
					}
					else
					{
						// Create the Direct3D 12 constant buffer
						FAILED_DEBUG_BREAK(direct3D12Rhi.getD3D12Device().CreateBuffer(&d3d12BufferDesc, nullptr, &mD3D12Buffer))
					}
				}

				{ // Shader resource view part
					// Direct3D 12 shader resource view description
					D3D12_SHADER_RESOURCE_VIEW_DESC d3d12ShaderResourceViewDesc = {};
					d3d12ShaderResourceViewDesc.Format				 = Mapping::getDirect3D12Format(textureFormat);
					d3d12ShaderResourceViewDesc.ViewDimension		 = D3D12_SRV_DIMENSION_BUFFER;
					d3d12ShaderResourceViewDesc.Buffer.ElementOffset = 0;
					d3d12ShaderResourceViewDesc.Buffer.ElementWidth	 = numberOfBytes / Rhi::TextureFormat::GetNumberOfBytesPerElement(textureFormat);

					// Create the Direct3D 12 shader resource view instance
					FAILED_DEBUG_BREAK(direct3D12Rhi.getD3D12Device().CreateShaderResourceView(mD3D12Buffer, &d3d12ShaderResourceViewDesc, &mD3D12ShaderResourceViewTexture))
				}

				// Assign a default name to the resource for debugging purposes
				#if SE_DEBUG
					if (nullptr != mD3D12Resource)
					{
						RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "SBO", 6)	// 6 = "SBO: " including terminating zero
						FAILED_DEBUG_BREAK(mD3D12Resource->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
					}
				#endif
				*/
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~StructuredBuffer() override
		{
			// TODO(co) Direct3D 12 update
			/*
			// Release the used resources
			if (nullptr != mD3D12Buffer)
			{
				mD3D12Buffer->Release();
				mD3D12Buffer = nullptr;
			}
			*/
		}

		/**
		*  @brief
		*    Return the Direct3D structured buffer instance
		*
		*  @return
		*    The Direct3D structured buffer instance, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		// TODO(co) Direct3D 12 update
		//[[nodiscard]] inline ID3D12Buffer* getD3D12Buffer() const
		//{
		//	return mD3D12Buffer;
		//}


	//[-------------------------------------------------------]
	//[ Protected virtual RefCount methods               ]
	//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(StructuredBuffer, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit StructuredBuffer(const StructuredBuffer&) = delete;
		StructuredBuffer& operator =(const StructuredBuffer&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		// TODO(co) Direct3D 12 update
		//ID3D12Buffer* mD3D12Buffer;	// Direct3D texture buffer instance, can be a null pointer


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Buffer/IndirectBuffer.h                 ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 indirect buffer object class
	*/
	class IndirectBuffer final : public Rhi::IIndirectBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the indirect buffer, must be valid
		*  @param[in] data
		*    Indirect buffer data, can be a null pointer (empty buffer)
		*  @param[in] indirectBufferFlags
		*    Indirect buffer flags, see "Rhi::IndirectBufferFlag"
		*/
		IndirectBuffer(Direct3D12Rhi& direct3D12Rhi, uint32_t numberOfBytes, const void* data, uint32_t indirectBufferFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IIndirectBuffer(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mD3D12CommandSignature(nullptr),
			mD3D12Resource(nullptr)
		{
			// Sanity checks
			SE_ASSERT((indirectBufferFlags & Rhi::IndirectBufferFlag::DRAW_ARGUMENTS) != 0 || (indirectBufferFlags & Rhi::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) != 0, "Invalid Direct3D 12 flags, indirect buffer element type specification \"DRAW_ARGUMENTS\" or \"DRAW_INDEXED_ARGUMENTS\" is missing")
				SE_ASSERT(!((indirectBufferFlags & Rhi::IndirectBufferFlag::DRAW_ARGUMENTS) != 0 && (indirectBufferFlags & Rhi::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) != 0), "Invalid Direct3D 12 flags, indirect buffer element type specification \"DRAW_ARGUMENTS\" or \"DRAW_INDEXED_ARGUMENTS\" must be set, but not both at one and the same time")
				SE_ASSERT((indirectBufferFlags & Rhi::IndirectBufferFlag::DRAW_ARGUMENTS) == 0 || (numberOfBytes % sizeof(Rhi::DrawArguments)) == 0, "Direct3D 12 indirect buffer element type flags specification is \"DRAW_ARGUMENTS\" but the given number of bytes don't align to this")
				SE_ASSERT((indirectBufferFlags & Rhi::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS) == 0 || (numberOfBytes % sizeof(Rhi::DrawIndexedArguments)) == 0, "Direct3D 12 indirect buffer element type flags specification is \"DRAW_INDEXED_ARGUMENTS\" but the given number of bytes don't align to this")

				// TODO(co) This is only meant for the Direct3D 12 RHI implementation kickoff.
				// Note: using upload heaps to transfer static data like vert buffers is not 
				// recommended. Every time the GPU needs it, the upload heap will be marshalled 
				// over. Please read up on Default Heap usage. An upload heap is used here for 
				// code simplicity and because there are very few verts to actually transfer.
				const CD3DX12_HEAP_PROPERTIES d3d12XHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
			const CD3DX12_RESOURCE_DESC d3d12XResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(numberOfBytes);
			ID3D12Device& d3d12Device = direct3D12Rhi.getD3D12Device();
			if ( SUCCEEDED(d3d12Device.CreateCommittedResource(
				&d3d12XHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&d3d12XResourceDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&mD3D12Resource))) )
			{
				// Data given?
				if ( nullptr != data )
				{
					// Copy the data to the indirect buffer
					UINT8* pIndirectDataBegin;
					CD3DX12_RANGE readRange(0, 0);	// We do not intend to read from this resource on the CPU
					if ( SUCCEEDED(mD3D12Resource->Map(0, &readRange, reinterpret_cast<void**>(&pIndirectDataBegin))) )
					{
						memcpy(pIndirectDataBegin, data, numberOfBytes);
						mD3D12Resource->Unmap(0, nullptr);
					}
					else
					{
						RHI_LOG(CRITICAL, "Failed to map Direct3D 12 indirect buffer")
					}
				}

				D3D12_INDIRECT_ARGUMENT_DESC d3dIndirectArgumentDescription[1];
				d3dIndirectArgumentDescription[0].Type = (indirectBufferFlags & Rhi::IndirectBufferFlag::DRAW_ARGUMENTS) ? D3D12_INDIRECT_ARGUMENT_TYPE_DRAW : D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;

				D3D12_COMMAND_SIGNATURE_DESC d3d12CommandSignatureDescription;
				d3d12CommandSignatureDescription.ByteStride = (indirectBufferFlags & Rhi::IndirectBufferFlag::DRAW_ARGUMENTS) ? sizeof(Rhi::DrawArguments) : sizeof(Rhi::DrawIndexedArguments);
				d3d12CommandSignatureDescription.NumArgumentDescs = 1;
				d3d12CommandSignatureDescription.pArgumentDescs = d3dIndirectArgumentDescription;
				d3d12CommandSignatureDescription.NodeMask = 0;

				if ( FAILED(d3d12Device.CreateCommandSignature(&d3d12CommandSignatureDescription, nullptr, IID_PPV_ARGS(&mD3D12CommandSignature))) )
				{
					RHI_LOG(CRITICAL, "Failed to create Direct3D 12 command signature")
				}

				// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "IndirectBufferObject", 23)	// 23 = "IndirectBufferObject: " including terminating zero
					if ( nullptr != mD3D12CommandSignature )
					{
						FAILED_DEBUG_BREAK(mD3D12CommandSignature->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
					}
				FAILED_DEBUG_BREAK(mD3D12Resource->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
#endif
			}
			else
			{
				RHI_LOG(CRITICAL, "Failed to create Direct3D 12 indirect buffer resource")
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~IndirectBuffer() override
		{
			// Release the used resources
			if ( nullptr != mD3D12CommandSignature )
			{
				mD3D12CommandSignature->Release();
			}
			if ( nullptr != mD3D12Resource )
			{
				mD3D12Resource->Release();
			}
		}

		/**
		*  @brief
		*    Return the Direct3D 12 command signature instance
		*
		*  @return
		*    The Direct3D 12 command signature instance, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3D12CommandSignature* getD3D12CommandSignature() const
		{
			return mD3D12CommandSignature;
		}

		/**
		*  @brief
		*    Return the Direct3D 12 indirect buffer resource instance
		*
		*  @return
		*    The Direct3D 12 indirect buffer resource instance, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3D12Resource* getD3D12Resource() const
		{
			return mD3D12Resource;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IIndirectBuffer methods           ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const uint8_t* getEmulationData() const override
		{
			return nullptr;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(IndirectBuffer, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit IndirectBuffer(const IndirectBuffer&) = delete;
		IndirectBuffer& operator =(const IndirectBuffer&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		ID3D12CommandSignature* mD3D12CommandSignature;
		ID3D12Resource*			mD3D12Resource;


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Buffer/UniformBuffer.h                  ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 uniform buffer object (UBO, "constant buffer" in Direct3D terminology) interface
	*/
	class UniformBuffer final : public Rhi::IUniformBuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] numberOfBytes
		*    Number of bytes within the uniform buffer, must be valid
		*  @param[in] data
		*    Uniform buffer data, can be a null pointer (empty buffer)
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*/
		UniformBuffer(Direct3D12Rhi& direct3D12Rhi, uint32_t numberOfBytes, const void* data, [[maybe_unused]] Rhi::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			Rhi::IUniformBuffer(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mNumberOfBytesOnGpu(::detail::align(numberOfBytes, 256)),	// Constant buffer size is required to be 256-byte aligned, no assert because other RHI implementations have another alignment (DirectX 11 e.g. 16), see "ID3D12Device::CreateConstantBufferView method" at https://msdn.microsoft.com/de-de/library/windows/desktop/dn788659%28v=vs.85%29.aspx
			mD3D12Resource(nullptr),
			mMappedData(nullptr)
		{
			// TODO(co) Add buffer usage setting support
			const CD3DX12_HEAP_PROPERTIES d3d12XHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
			const CD3DX12_RESOURCE_DESC d3d12XResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(mNumberOfBytesOnGpu);
			if ( SUCCEEDED(direct3D12Rhi.getD3D12Device().CreateCommittedResource(
				&d3d12XHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&d3d12XResourceDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&mD3D12Resource))) )
			{
				// Data given?
				if ( nullptr != data )
				{
					CD3DX12_RANGE readRange(0, 0);	// We do not intend to read from this resource on the CPU
					if ( SUCCEEDED(mD3D12Resource->Map(0, &readRange, reinterpret_cast<void**>(&mMappedData))) )
					{
						memcpy(mMappedData, data, numberOfBytes);
						mD3D12Resource->Unmap(0, nullptr);
					}
					else
					{
						RHI_LOG(CRITICAL, "Failed to map Direct3D 12 uniform buffer")
					}
				}

				// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "UBO", 6)	// 6 = "UBO: " including terminating zero
					FAILED_DEBUG_BREAK(mD3D12Resource->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
#endif
			}
			else
			{
				RHI_LOG(CRITICAL, "Failed to create Direct3D 12 uniform buffer resource")
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~UniformBuffer() override
		{
			// Release the Direct3D 12 constant buffer
			if ( nullptr != mD3D12Resource )
			{
				mD3D12Resource->Release();
			}
		}

		/**
		*  @brief
		*    Return the number of bytes on GPU
		*
		*  @return
		*    The number of bytes on GPU
		*/
		[[nodiscard]] inline uint32_t getNumberOfBytesOnGpu() const
		{
			return mNumberOfBytesOnGpu;
		}

		/**
		*  @brief
		*    Return the Direct3D uniform buffer resource instance
		*
		*  @return
		*    The Direct3D uniform buffer resource instance, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3D12Resource* getD3D12Resource() const
		{
			return mD3D12Resource;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(UniformBuffer, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit UniformBuffer(const UniformBuffer&) = delete;
		UniformBuffer& operator =(const UniformBuffer&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		uint32_t		mNumberOfBytesOnGpu;
		ID3D12Resource* mD3D12Resource;
		uint8_t*		mMappedData;


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Buffer/BufferManager.h                  ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 buffer manager interface
	*/
	class BufferManager final : public Rhi::IBufferManager
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*/
		inline explicit BufferManager(Direct3D12Rhi& direct3D12Rhi) :
			IBufferManager(direct3D12Rhi)
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~BufferManager() override
		{
			// Nothing here
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IBufferManager methods            ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual Rhi::IVertexBuffer* createVertexBuffer(uint32_t numberOfBytes, const void* data = nullptr, [[maybe_unused]] uint32_t bufferFlags = 0, Rhi::BufferUsage bufferUsage = Rhi::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());
			return RHI_NEW(VertexBuffer)(direct3D12Rhi, numberOfBytes, data, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IIndexBuffer* createIndexBuffer(uint32_t numberOfBytes, const void* data = nullptr, [[maybe_unused]] uint32_t bufferFlags = 0, Rhi::BufferUsage bufferUsage = Rhi::BufferUsage::STATIC_DRAW, Rhi::IndexBufferFormat::Enum indexBufferFormat = Rhi::IndexBufferFormat::UNSIGNED_SHORT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());
			return RHI_NEW(IndexBuffer)(direct3D12Rhi, numberOfBytes, data, bufferUsage, indexBufferFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IVertexArray* createVertexArray(const Rhi::VertexAttributes& vertexAttributes, uint32_t numberOfVertexBuffers, const Rhi::VertexArrayVertexBuffer* vertexBuffers, Rhi::IIndexBuffer* indexBuffer = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());

			// Sanity checks
#if SE_DEBUG
			{
				const Rhi::VertexArrayVertexBuffer* vertexBufferEnd = vertexBuffers + numberOfVertexBuffers;
				for ( const Rhi::VertexArrayVertexBuffer* vertexBuffer = vertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer )
				{
					SE_ASSERT(&direct3D12Rhi == &vertexBuffer->vertexBuffer->getRhi(), "Direct3D 12 error: The given vertex buffer resource is owned by another RHI instance")
				}
			}
#endif
			SE_ASSERT(nullptr == indexBuffer || &direct3D12Rhi == &indexBuffer->getRhi(), "Direct3D 12 error: The given index buffer resource is owned by another RHI instance")

				// Create vertex array
				uint16_t id = 0;
			if ( direct3D12Rhi.VertexArrayMakeId.CreateID(id) )
			{
				return RHI_NEW(VertexArray)(direct3D12Rhi, vertexAttributes, numberOfVertexBuffers, vertexBuffers, static_cast<IndexBuffer*>(indexBuffer), id RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			}

			// Error: Ensure a correct reference counter behaviour
			const Rhi::VertexArrayVertexBuffer* vertexBufferEnd = vertexBuffers + numberOfVertexBuffers;
			for ( const Rhi::VertexArrayVertexBuffer* vertexBuffer = vertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer )
			{
				vertexBuffer->vertexBuffer->AddReference();
				vertexBuffer->vertexBuffer->ReleaseReference();
			}
			if ( nullptr != indexBuffer )
			{
				indexBuffer->AddReference();
				indexBuffer->ReleaseReference();
			}
			return nullptr;
		}

		[[nodiscard]] inline virtual Rhi::ITextureBuffer* createTextureBuffer(uint32_t numberOfBytes, const void* data = nullptr, uint32_t = Rhi::BufferFlag::SHADER_RESOURCE, Rhi::BufferUsage bufferUsage = Rhi::BufferUsage::STATIC_DRAW, Rhi::TextureFormat::Enum textureFormat = Rhi::TextureFormat::R32G32B32A32F RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());
			return RHI_NEW(TextureBuffer)(direct3D12Rhi, numberOfBytes, data, bufferUsage, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IStructuredBuffer* createStructuredBuffer(uint32_t numberOfBytes, const void* data, [[maybe_unused]] uint32_t bufferFlags, Rhi::BufferUsage bufferUsage, uint32_t numberOfStructureBytes RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());
			return RHI_NEW(StructuredBuffer)(direct3D12Rhi, numberOfBytes, data, bufferUsage, numberOfStructureBytes RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IIndirectBuffer* createIndirectBuffer(uint32_t numberOfBytes, const void* data = nullptr, uint32_t indirectBufferFlags = 0, [[maybe_unused]] Rhi::BufferUsage bufferUsage = Rhi::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());
			return RHI_NEW(IndirectBuffer)(direct3D12Rhi, numberOfBytes, data, indirectBufferFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IUniformBuffer* createUniformBuffer(uint32_t numberOfBytes, const void* data = nullptr, Rhi::BufferUsage bufferUsage = Rhi::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());

			// Don't remove this reminder comment block: There are no buffer flags by intent since an uniform buffer can't be used for unordered access and as a consequence an uniform buffer must always used as shader resource to not be pointless
			// SE_ASSERT((bufferFlags & Rhi::BufferFlag::UNORDERED_ACCESS) == 0, "Invalid Direct3D 12 buffer flags, uniform buffer can't be used for unordered access")
			// SE_ASSERT((bufferFlags & Rhi::BufferFlag::SHADER_RESOURCE) != 0, "Invalid Direct3D 12 buffer flags, uniform buffer must be used as shader resource")

			// Create the uniform buffer
			return RHI_NEW(UniformBuffer)(direct3D12Rhi, numberOfBytes, data, bufferUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(BufferManager, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit BufferManager(const BufferManager&) = delete;
		BufferManager& operator =(const BufferManager&) = delete;


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Texture/Texture1D.h                     ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 1D texture class
	*/
	class Texture1D final : public Rhi::ITexture1D
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*/
		Texture1D(Direct3D12Rhi& direct3D12Rhi, uint32_t width, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITexture1D(direct3D12Rhi, width RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mDxgiFormat(Mapping::getDirect3D12Format(textureFormat)),
			mNumberOfMipmaps(0),
			mD3D12Resource(nullptr)
		{
			// Calculate the number of mipmaps
			const bool dataContainsMipmaps = (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS);
			const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS));
			mNumberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? Rhi::ITexture::getNumberOfMipmaps(width) : 1;
			SE_ASSERT(!generateMipmaps, "TODO(co) Direct3D 12 texture mipmap generation isn't implemented, yet")
				if ( generateMipmaps )
				{
					mNumberOfMipmaps = 1;
				}

			// Create the Direct3D 12 texture resource
			mD3D12Resource = TextureHelper::CreateTexture(direct3D12Rhi.getD3D12Device(), TextureHelper::TextureType::TEXTURE_1D, width, 1, 1, 1, textureFormat, 1, mNumberOfMipmaps, textureFlags, nullptr);
			if ( nullptr != mD3D12Resource )
			{
				// Upload all mipmaps in case the user provided us with texture data
				if ( nullptr != data )
				{
					for ( uint32_t mipmap = 0; mipmap < mNumberOfMipmaps; ++mipmap )
					{
						// Upload the current mipmap
						const uint32_t numberOfBytesPerRow = Rhi::TextureFormat::getNumberOfBytesPerRow(textureFormat, width);
						const uint32_t numberOfBytesPerSlice = Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, 1);
						TextureHelper::SetTextureData(direct3D12Rhi.getUploadContext(), *mD3D12Resource, width, 1, 1, textureFormat, mNumberOfMipmaps, mipmap, 0, data, numberOfBytesPerSlice, numberOfBytesPerRow);

						// Move on to the next mipmap and ensure the size is always at least 1x1
						data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
						width = getHalfSize(width);
					}
				}

				// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "1D texture", 13)	// 13 = "1D texture: " including terminating zero
					FAILED_DEBUG_BREAK(mD3D12Resource->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
#endif
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Texture1D() override
		{
			if ( nullptr != mD3D12Resource )
			{
				mD3D12Resource->Release();
			}
		}

		/**
		*  @brief
		*    Return the DXGI format
		*
		*  @return
		*    The DDXGI format
		*/
		[[nodiscard]] inline DXGI_FORMAT getDxgiFormat() const
		{
			return mDxgiFormat;
		}

		/**
		*  @brief
		*    Return the number of mipmaps
		*
		*  @return
		*    The number of mipmaps
		*/
		[[nodiscard]] inline uint32_t getNumberOfMipmaps() const
		{
			return mNumberOfMipmaps;
		}

		/**
		*  @brief
		*    Return the Direct3D 12 resource instance
		*
		*  @return
		*    The Direct3D 12 resource instance, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3D12Resource* getD3D12Resource() const
		{
			return mD3D12Resource;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(Texture1D, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Texture1D(const Texture1D&) = delete;
		Texture1D& operator =(const Texture1D&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		DXGI_FORMAT		mDxgiFormat;
		uint32_t		mNumberOfMipmaps;
		ID3D12Resource* mD3D12Resource;


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Texture/Texture1DArray.h                ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 1D array texture class
	*/
	class Texture1DArray final : public Rhi::ITexture1DArray
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] numberOfSlices
		*    Number of slices, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*/
		Texture1DArray(Direct3D12Rhi& direct3D12Rhi, uint32_t width, uint32_t numberOfSlices, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITexture1DArray(direct3D12Rhi, width, numberOfSlices RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mDxgiFormat(Mapping::getDirect3D12Format(textureFormat)),
			mNumberOfMipmaps(0),
			mNumberOfSlices(numberOfSlices),
			mD3D12Resource(nullptr)
		{
			// TODO(co) Add "Rhi::TextureFlag::GENERATE_MIPMAPS" support, also for render target textures

			// Calculate the number of mipmaps
			const bool dataContainsMipmaps = (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS);
			const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS));
			mNumberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? Rhi::ITexture::getNumberOfMipmaps(width) : 1;
			SE_ASSERT(!generateMipmaps, "TODO(co) Direct3D 12 texture mipmap generation isn't implemented, yet")
				if ( generateMipmaps )
				{
					mNumberOfMipmaps = 1;
				}

			// Create the Direct3D 12 texture resource
			mD3D12Resource = TextureHelper::CreateTexture(direct3D12Rhi.getD3D12Device(), TextureHelper::TextureType::TEXTURE_1D_ARRAY, width, 1, 1, numberOfSlices, textureFormat, 1, mNumberOfMipmaps, textureFlags, nullptr);
			if ( nullptr != mD3D12Resource )
			{
				// Upload all mipmaps in case the user provided us with texture data
				if ( nullptr != data )
				{
					// Data layout
					// - Direct3D 12 wants: DDS files are organized in slice-major order, like this:
					//     Slice0: Mip0, Mip1, Mip2, etc.
					//     Slice1: Mip0, Mip1, Mip2, etc.
					//     etc.
					// - The RHI provides: CRN and KTX files are organized in mip-major order, like this:
					//     Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
					//     Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
					//     etc.
					for ( uint32_t mipmap = 0; mipmap < mNumberOfMipmaps; ++mipmap )
					{
						const uint32_t numberOfBytesPerRow = Rhi::TextureFormat::getNumberOfBytesPerRow(textureFormat, width);
						const uint32_t numberOfBytesPerSlice = Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, 1);
						for ( uint32_t arraySlice = 0; arraySlice < numberOfSlices; ++arraySlice )
						{
							// Upload the current mipmap
							TextureHelper::SetTextureData(direct3D12Rhi.getUploadContext(), *mD3D12Resource, width, 1, 1, textureFormat, mNumberOfMipmaps, mipmap, arraySlice, data, numberOfBytesPerSlice, numberOfBytesPerRow);

							// Move on to the next slice
							data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
						}

						// Move on to the next mipmap and ensure the size is always at least 1x1
						width = getHalfSize(width);
					}
				}

				// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "1D texture array", 19)	// 19 = "1D texture array: " including terminating zero
					FAILED_DEBUG_BREAK(mD3D12Resource->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
#endif
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Texture1DArray() override
		{
			if ( nullptr != mD3D12Resource )
			{
				mD3D12Resource->Release();
			}
		}

		/**
		*  @brief
		*    Return the DXGI format
		*
		*  @return
		*    The DDXGI format
		*/
		[[nodiscard]] inline DXGI_FORMAT getDxgiFormat() const
		{
			return mDxgiFormat;
		}

		/**
		*  @brief
		*    Return the number of mipmaps
		*
		*  @return
		*    The number of mipmaps
		*/
		[[nodiscard]] inline uint32_t getNumberOfMipmaps() const
		{
			return mNumberOfMipmaps;
		}

		/**
		*  @brief
		*    Return the number of slices
		*
		*  @return
		*    The number of slices
		*/
		[[nodiscard]] inline uint32_t getNumberOfSlices() const
		{
			return mNumberOfSlices;
		}

		/**
		*  @brief
		*    Return the Direct3D 12 resource instance
		*
		*  @return
		*    The Direct3D 12 resource instance, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3D12Resource* getD3D12Resource() const
		{
			return mD3D12Resource;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(Texture1DArray, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Texture1DArray(const Texture1DArray&) = delete;
		Texture1DArray& operator =(const Texture1DArray&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		DXGI_FORMAT		mDxgiFormat;
		uint32_t		mNumberOfMipmaps;
		uint32_t		mNumberOfSlices;
		ID3D12Resource* mD3D12Resource;


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Texture/Texture2D.h                     ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 2D texture class
	*/
	class Texture2D final : public Rhi::ITexture2D
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] height
		*    Texture height, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] numberOfMultisamples
		*    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
		*  @param[in] optimizedTextureClearValue
		*    Optional optimized texture clear value
		*/
		Texture2D(Direct3D12Rhi& direct3D12Rhi, uint32_t width, uint32_t height, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags, uint8_t numberOfMultisamples, const Rhi::OptimizedTextureClearValue* optimizedTextureClearValue RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITexture2D(direct3D12Rhi, width, height RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mDxgiFormat(Mapping::getDirect3D12Format(textureFormat)),
			mNumberOfMipmaps(0),
			mD3D12Resource(nullptr)
		{
			// Sanity checks
			SE_ASSERT(numberOfMultisamples == 1 || numberOfMultisamples == 2 || numberOfMultisamples == 4 || numberOfMultisamples == 8, "Invalid Direct3D 12 texture parameters")
				SE_ASSERT(numberOfMultisamples == 1 || nullptr == data, "Invalid Direct3D 12 texture parameters")
				SE_ASSERT(numberOfMultisamples == 1 || 0 == (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS), "Invalid Direct3D 12 texture parameters")
				SE_ASSERT(numberOfMultisamples == 1 || 0 == (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS), "Invalid Direct3D 12 texture parameters")
				SE_ASSERT(numberOfMultisamples == 1 || 0 != (textureFlags & Rhi::TextureFlag::RENDER_TARGET), "Invalid Direct3D 12 texture parameters")
				SE_ASSERT(0 == (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS) || nullptr != data, "Invalid Direct3D 12 texture parameters")
				SE_ASSERT((textureFlags & Rhi::TextureFlag::RENDER_TARGET) == 0 || nullptr == data, "Direct3D 12 render target textures can't be filled using provided data")

				// TODO(co) Add "Rhi::TextureFlag::GENERATE_MIPMAPS" support, also for render target textures

				// Calculate the number of mipmaps
				const bool dataContainsMipmaps = (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS);
			const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS));
			mNumberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? Rhi::ITexture::getNumberOfMipmaps(width, height) : 1;
			SE_ASSERT(!generateMipmaps, "TODO(co) Direct3D 12 texture mipmap generation isn't implemented, yet")
				if ( generateMipmaps )
				{
					mNumberOfMipmaps = 1;
				}

			// Create the Direct3D 12 texture resource
			mD3D12Resource = TextureHelper::CreateTexture(direct3D12Rhi.getD3D12Device(), TextureHelper::TextureType::TEXTURE_2D, width, height, 1, 1, textureFormat, numberOfMultisamples, mNumberOfMipmaps, textureFlags, optimizedTextureClearValue);
			if ( nullptr != mD3D12Resource )
			{
				// Upload all mipmaps in case the user provided us with texture data
				if ( nullptr != data )
				{
					for ( uint32_t mipmap = 0; mipmap < mNumberOfMipmaps; ++mipmap )
					{
						// Upload the current mipmap
						const uint32_t numberOfBytesPerRow = Rhi::TextureFormat::getNumberOfBytesPerRow(textureFormat, width);
						const uint32_t numberOfBytesPerSlice = Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height);
						TextureHelper::SetTextureData(direct3D12Rhi.getUploadContext(), *mD3D12Resource, width, height, 1, textureFormat, mNumberOfMipmaps, mipmap, 0, data, numberOfBytesPerSlice, numberOfBytesPerRow);

						// Move on to the next mipmap and ensure the size is always at least 1x1
						data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
						width = getHalfSize(width);
						height = getHalfSize(height);
					}
				}

				// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "2D texture", 13)	// 13 = "2D texture: " including terminating zero
					FAILED_DEBUG_BREAK(mD3D12Resource->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
#endif
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Texture2D() override
		{
			if ( nullptr != mD3D12Resource )
			{
				mD3D12Resource->Release();
			}
		}

		/**
		*  @brief
		*    Return the DXGI format
		*
		*  @return
		*    The DDXGI format
		*/
		[[nodiscard]] inline DXGI_FORMAT getDxgiFormat() const
		{
			return mDxgiFormat;
		}

		/**
		*  @brief
		*    Return the number of mipmaps
		*
		*  @return
		*    The number of mipmaps
		*/
		[[nodiscard]] inline uint32_t getNumberOfMipmaps() const
		{
			return mNumberOfMipmaps;
		}

		/**
		*  @brief
		*    Return the Direct3D 12 resource instance
		*
		*  @return
		*    The Direct3D 12 resource instance, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3D12Resource* getD3D12Resource() const
		{
			return mD3D12Resource;
		}

		/**
		*  @brief
		*    Set minimum maximum mipmap index
		*
		*  @param[in] minimumMipmapIndex
		*    Minimum mipmap index, the most detailed mipmap, also known as base mipmap, 0 by default
		*  @param[in] maximumMipmapIndex
		*    Maximum mipmap index, the least detailed mipmap, <number of mipmaps> by default
		*/
		inline void setMinimumMaximumMipmapIndex([[maybe_unused]] uint32_t minimumMipmapIndex, [[maybe_unused]] uint32_t maximumMipmapIndex)
		{
			// TODO(co) Implement me
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(Texture2D, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Texture2D(const Texture2D&) = delete;
		Texture2D& operator =(const Texture2D&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		DXGI_FORMAT		mDxgiFormat;
		uint32_t		mNumberOfMipmaps;
		ID3D12Resource* mD3D12Resource;


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Texture/Texture2DArray.h                ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 2D array texture class
	*/
	class Texture2DArray final : public Rhi::ITexture2DArray
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] height
		*    Texture height, must be >0
		*  @param[in] numberOfSlices
		*    Number of slices, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*/
		Texture2DArray(Direct3D12Rhi& direct3D12Rhi, uint32_t width, uint32_t height, uint32_t numberOfSlices, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITexture2DArray(direct3D12Rhi, width, height, numberOfSlices RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mDxgiFormat(Mapping::getDirect3D12Format(textureFormat)),
			mNumberOfMipmaps(0),
			mNumberOfSlices(numberOfSlices),
			mD3D12Resource(nullptr)
		{
			// TODO(co) Add "Rhi::TextureFlag::GENERATE_MIPMAPS" support, also for render target textures

			// Calculate the number of mipmaps
			const bool dataContainsMipmaps = (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS);
			const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS));
			mNumberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? Rhi::ITexture::getNumberOfMipmaps(width, height) : 1;
			SE_ASSERT(!generateMipmaps, "TODO(co) Direct3D 12 texture mipmap generation isn't implemented, yet")
				if ( generateMipmaps )
				{
					mNumberOfMipmaps = 1;
				}

			// Create the Direct3D 12 texture resource
			mD3D12Resource = TextureHelper::CreateTexture(direct3D12Rhi.getD3D12Device(), TextureHelper::TextureType::TEXTURE_2D_ARRAY, width, height, 1, numberOfSlices, textureFormat, 1, mNumberOfMipmaps, textureFlags, nullptr);
			if ( nullptr != mD3D12Resource )
			{
				// Upload all mipmaps in case the user provided us with texture data
				if ( nullptr != data )
				{
					// Data layout
					// - Direct3D 12 wants: DDS files are organized in slice-major order, like this:
					//     Slice0: Mip0, Mip1, Mip2, etc.
					//     Slice1: Mip0, Mip1, Mip2, etc.
					//     etc.
					// - The RHI provides: CRN and KTX files are organized in mip-major order, like this:
					//     Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
					//     Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
					//     etc.
					for ( uint32_t mipmap = 0; mipmap < mNumberOfMipmaps; ++mipmap )
					{
						const uint32_t numberOfBytesPerRow = Rhi::TextureFormat::getNumberOfBytesPerRow(textureFormat, width);
						const uint32_t numberOfBytesPerSlice = Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height);
						for ( uint32_t arraySlice = 0; arraySlice < numberOfSlices; ++arraySlice )
						{
							// Upload the current mipmap
							TextureHelper::SetTextureData(direct3D12Rhi.getUploadContext(), *mD3D12Resource, width, height, 1, textureFormat, mNumberOfMipmaps, mipmap, arraySlice, data, numberOfBytesPerSlice, numberOfBytesPerRow);

							// Move on to the next slice
							data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
						}

						// Move on to the next mipmap and ensure the size is always at least 1x1
						width = getHalfSize(width);
						height = getHalfSize(height);
					}
				}

				// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "2D texture array", 19)	// 19 = "2D texture array: " including terminating zero
					FAILED_DEBUG_BREAK(mD3D12Resource->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
#endif
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Texture2DArray() override
		{
			if ( nullptr != mD3D12Resource )
			{
				mD3D12Resource->Release();
			}
		}

		/**
		*  @brief
		*    Return the DXGI format
		*
		*  @return
		*    The DDXGI format
		*/
		[[nodiscard]] inline DXGI_FORMAT getDxgiFormat() const
		{
			return mDxgiFormat;
		}

		/**
		*  @brief
		*    Return the number of mipmaps
		*
		*  @return
		*    The number of mipmaps
		*/
		[[nodiscard]] inline uint32_t getNumberOfMipmaps() const
		{
			return mNumberOfMipmaps;
		}

		/**
		*  @brief
		*    Return the number of slices
		*
		*  @return
		*    The number of slices
		*/
		[[nodiscard]] inline uint32_t getNumberOfSlices() const
		{
			return mNumberOfSlices;
		}

		/**
		*  @brief
		*    Return the Direct3D 12 resource instance
		*
		*  @return
		*    The Direct3D 12 resource instance, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3D12Resource* getD3D12Resource() const
		{
			return mD3D12Resource;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(Texture2DArray, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Texture2DArray(const Texture2DArray&) = delete;
		Texture2DArray& operator =(const Texture2DArray&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		DXGI_FORMAT		mDxgiFormat;
		uint32_t		mNumberOfMipmaps;
		uint32_t		mNumberOfSlices;
		ID3D12Resource* mD3D12Resource;


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Texture/Texture3D.h                     ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 3D texture class
	*/
	class Texture3D final : public Rhi::ITexture3D
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] height
		*    Texture height, must be >0
		*  @param[in] depth
		*    Texture depth, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*/
		Texture3D(Direct3D12Rhi& direct3D12Rhi, uint32_t width, uint32_t height, uint32_t depth, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITexture3D(direct3D12Rhi, width, height, depth RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mDxgiFormat(Mapping::getDirect3D12Format(textureFormat)),
			mNumberOfMipmaps(0),
			mD3D12Resource(nullptr)
		{
			// TODO(co) Add "Rhi::TextureFlag::GENERATE_MIPMAPS" support, also for render target textures

			// Calculate the number of mipmaps
			const bool dataContainsMipmaps = (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS);
			const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS));
			mNumberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? Rhi::ITexture::getNumberOfMipmaps(width, height) : 1;
			SE_ASSERT(!generateMipmaps, "TODO(co) Direct3D 12 texture mipmap generation isn't implemented, yet")
				if ( generateMipmaps )
				{
					mNumberOfMipmaps = 1;
				}

			// Create the Direct3D 12 texture resource
			mD3D12Resource = TextureHelper::CreateTexture(direct3D12Rhi.getD3D12Device(), TextureHelper::TextureType::TEXTURE_3D, width, height, depth, 1, textureFormat, 1, mNumberOfMipmaps, textureFlags, nullptr);
			if ( nullptr != mD3D12Resource )
			{
				// Upload all mipmaps in case the user provided us with texture data
				if ( nullptr != data )
				{
					// Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
					//   Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
					//   Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
					//   etc.
					for ( uint32_t mipmap = 0; mipmap < mNumberOfMipmaps; ++mipmap )
					{
						// Upload the current mipmap
						const uint32_t numberOfBytesPerRow = Rhi::TextureFormat::getNumberOfBytesPerRow(textureFormat, width);
						const uint32_t numberOfBytesPerSlice = Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height) * depth;
						TextureHelper::SetTextureData(direct3D12Rhi.getUploadContext(), *mD3D12Resource, width, height, depth, textureFormat, mNumberOfMipmaps, mipmap, 0, data, numberOfBytesPerSlice, numberOfBytesPerRow);

						// Move on to the next mipmap and ensure the size is always at least 1x1x1
						data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
						width = getHalfSize(width);
						height = getHalfSize(height);
						depth = getHalfSize(depth);
					}
				}

				// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "3D texture", 13)	// 13 = "3D texture: " including terminating zero
					FAILED_DEBUG_BREAK(mD3D12Resource->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
#endif
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Texture3D() override
		{
			if ( nullptr != mD3D12Resource )
			{
				mD3D12Resource->Release();
			}
		}

		/**
		*  @brief
		*    Return the DXGI format
		*
		*  @return
		*    The DDXGI format
		*/
		[[nodiscard]] inline DXGI_FORMAT getDxgiFormat() const
		{
			return mDxgiFormat;
		}

		/**
		*  @brief
		*    Return the number of mipmaps
		*
		*  @return
		*    The number of mipmaps
		*/
		[[nodiscard]] inline uint32_t getNumberOfMipmaps() const
		{
			return mNumberOfMipmaps;
		}

		/**
		*  @brief
		*    Return the Direct3D 12 resource instance
		*
		*  @return
		*    The Direct3D 12 resource instance, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3D12Resource* getD3D12Resource() const
		{
			return mD3D12Resource;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(Texture3D, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Texture3D(const Texture3D&) = delete;
		Texture3D& operator =(const Texture3D&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		DXGI_FORMAT		mDxgiFormat;
		uint32_t		mNumberOfMipmaps;
		ID3D12Resource* mD3D12Resource;


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Texture/TextureCube.h                   ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 cube texture class
	*/
	class TextureCube final : public Rhi::ITextureCube
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] width
		*    Texture width, must be >0
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*/
		TextureCube(Direct3D12Rhi& direct3D12Rhi, uint32_t width, Rhi::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ITextureCube(direct3D12Rhi, width RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mDxgiFormat(Mapping::getDirect3D12Format(textureFormat)),
			mNumberOfMipmaps(0),
			mD3D12Resource(nullptr)
		{
			static constexpr uint32_t NUMBER_OF_SLICES = 6;	// In Direct3D 12, a cube map is a 2D array texture with six slices

			// TODO(co) Add "Rhi::TextureFlag::GENERATE_MIPMAPS" support, also for render target textures

			// Calculate the number of mipmaps
			const bool dataContainsMipmaps = (textureFlags & Rhi::TextureFlag::DATA_CONTAINS_MIPMAPS);
			const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & Rhi::TextureFlag::GENERATE_MIPMAPS));
			mNumberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? Rhi::ITexture::getNumberOfMipmaps(width) : 1;
			SE_ASSERT(!generateMipmaps, "TODO(co) Direct3D 12 texture mipmap generation isn't implemented, yet")
				if ( generateMipmaps )
				{
					mNumberOfMipmaps = 1;
				}

			// Create the Direct3D 12 texture resource
			mD3D12Resource = TextureHelper::CreateTexture(direct3D12Rhi.getD3D12Device(), TextureHelper::TextureType::TEXTURE_CUBE, width, width, 1, NUMBER_OF_SLICES, textureFormat, 1, mNumberOfMipmaps, textureFlags, nullptr);
			if ( nullptr != mD3D12Resource )
			{
				// Upload all mipmaps in case the user provided us with texture data
				if ( nullptr != data )
				{
					// Data layout
					// - Direct3D 12 wants: DDS files are organized in face-major order, like this:
					//     Face0: Mip0, Mip1, Mip2, etc.
					//     Face1: Mip0, Mip1, Mip2, etc.
					//     etc.
					// - The RHI provides: CRN and KTX files are organized in mip-major order, like this:
					//     Mip0: Face0, Face1, Face2, Face3, Face4, Face5
					//     Mip1: Face0, Face1, Face2, Face3, Face4, Face5
					//     etc.
					for ( uint32_t mipmap = 0; mipmap < mNumberOfMipmaps; ++mipmap )
					{
						// TODO(co) Is it somehow possible to upload a whole cube texture mipmap in one burst?
						const uint32_t numberOfBytesPerRow = Rhi::TextureFormat::getNumberOfBytesPerRow(textureFormat, width);
						const uint32_t numberOfBytesPerSlice = Rhi::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, width);
						for ( uint32_t arraySlice = 0; arraySlice < NUMBER_OF_SLICES; ++arraySlice )
						{
							// Upload the current mipmap
							TextureHelper::SetTextureData(direct3D12Rhi.getUploadContext(), *mD3D12Resource, width, width, 1, textureFormat, mNumberOfMipmaps, mipmap, arraySlice, data, numberOfBytesPerSlice, numberOfBytesPerRow);

							// Move on to the next slice
							data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
						}

						// Move on to the next mipmap and ensure the size is always at least 1x1
						width = getHalfSize(width);
					}
				}

				// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Cube texture", 15)	// 15 = "Cube texture: " including terminating zero
					FAILED_DEBUG_BREAK(mD3D12Resource->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
#endif
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TextureCube() override
		{
			if ( nullptr != mD3D12Resource )
			{
				mD3D12Resource->Release();
			}
		}

		/**
		*  @brief
		*    Return the DXGI format
		*
		*  @return
		*    The DXGI format
		*/
		[[nodiscard]] inline DXGI_FORMAT getDxgiFormat() const
		{
			return mDxgiFormat;
		}

		/**
		*  @brief
		*    Return the number of mipmaps
		*
		*  @return
		*    The number of mipmaps
		*/
		[[nodiscard]] inline uint32_t getNumberOfMipmaps() const
		{
			return mNumberOfMipmaps;
		}

		/**
		*  @brief
		*    Return the Direct3D 12 resource instance
		*
		*  @return
		*    The Direct3D 12 resource instance, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3D12Resource* getD3D12Resource() const
		{
			return mD3D12Resource;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(TextureCube, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TextureCube(const TextureCube&) = delete;
		TextureCube& operator =(const TextureCube&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		DXGI_FORMAT		mDxgiFormat;
		uint32_t		mNumberOfMipmaps;
		ID3D12Resource* mD3D12Resource;


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Texture/TextureManager.h                ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 texture manager interface
	*/
	class TextureManager final : public Rhi::ITextureManager
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*/
		inline explicit TextureManager(Direct3D12Rhi& direct3D12Rhi) :
			ITextureManager(direct3D12Rhi)
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TextureManager() override
		{
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::ITextureManager methods           ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual Rhi::ITexture1D* createTexture1D(uint32_t width, Rhi::TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, [[maybe_unused]] Rhi::TextureUsage textureUsage = Rhi::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());

			// Sanity check
			SE_ASSERT(width > 0, "Direct3D 12 create texture 1D was called with invalid parameters")

				// Create 1D texture resource, texture usage isn't supported
				return RHI_NEW(Texture1D)(direct3D12Rhi, width, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] virtual Rhi::ITexture1DArray* createTexture1DArray(uint32_t width, uint32_t numberOfSlices, Rhi::TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, [[maybe_unused]] Rhi::TextureUsage textureUsage = Rhi::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());

			// Sanity check
			SE_ASSERT(width > 0 && numberOfSlices > 0, "Direct3D 12 create texture 1D array was called with invalid parameters")

				// Create 1D texture array resource, texture usage isn't supported
				return RHI_NEW(Texture1DArray)(direct3D12Rhi, width, numberOfSlices, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] virtual Rhi::ITexture2D* createTexture2D(uint32_t width, uint32_t height, Rhi::TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, [[maybe_unused]] Rhi::TextureUsage textureUsage = Rhi::TextureUsage::DEFAULT, uint8_t numberOfMultisamples = 1, const Rhi::OptimizedTextureClearValue* optimizedTextureClearValue = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());

			// Sanity check
			SE_ASSERT(width > 0 && height > 0, "Direct3D 12 create texture 2D was called with invalid parameters")

				// Create 2D texture resource, texture usage isn't supported
				return RHI_NEW(Texture2D)(direct3D12Rhi, width, height, textureFormat, data, textureFlags, numberOfMultisamples, optimizedTextureClearValue RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] virtual Rhi::ITexture2DArray* createTexture2DArray(uint32_t width, uint32_t height, uint32_t numberOfSlices, Rhi::TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, [[maybe_unused]] Rhi::TextureUsage textureUsage = Rhi::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());

			// Sanity check
			SE_ASSERT(width > 0 && height > 0 && numberOfSlices > 0, "Direct3D 12 create texture 2D array was called with invalid parameters")

				// Create 2D texture array resource, texture usage isn't supported
				return RHI_NEW(Texture2DArray)(direct3D12Rhi, width, height, numberOfSlices, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] virtual Rhi::ITexture3D* createTexture3D(uint32_t width, uint32_t height, uint32_t depth, Rhi::TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, [[maybe_unused]] Rhi::TextureUsage textureUsage = Rhi::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());

			// Sanity check
			SE_ASSERT(width > 0 && height > 0 && depth > 0, "Direct3D 12 create texture 3D was called with invalid parameters")

				// Create 3D texture resource, texture usage isn't supported
				return RHI_NEW(Texture3D)(direct3D12Rhi, width, height, depth, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] virtual Rhi::ITextureCube* createTextureCube(uint32_t width, Rhi::TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, [[maybe_unused]] Rhi::TextureUsage textureUsage = Rhi::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());

			// Sanity check
			SE_ASSERT(width > 0, "Direct3D 12 create texture cube was called with invalid parameters")

				// Create cube texture resource, texture usage isn't supported
				return RHI_NEW(TextureCube)(direct3D12Rhi, width, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] virtual Rhi::ITextureCubeArray* createTextureCubeArray([[maybe_unused]] uint32_t width, [[maybe_unused]] uint32_t numberOfSlices, [[maybe_unused]] Rhi::TextureFormat::Enum textureFormat, [[maybe_unused]] const void* data = nullptr, [[maybe_unused]] uint32_t textureFlags = 0, [[maybe_unused]] Rhi::TextureUsage textureUsage = Rhi::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// TODO(co) Implement me
#if SE_DEBUG
			debugName = debugName;
#endif
			return nullptr;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(TextureManager, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TextureManager(const TextureManager&) = delete;
		TextureManager& operator =(const TextureManager&) = delete;


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/State/SamplerState.h                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 sampler state class
	*/
	class SamplerState final : public Rhi::ISamplerState
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] samplerState
		*    Sampler state to use
		*/
		SamplerState(Direct3D12Rhi& direct3D12Rhi, const Rhi::SamplerState& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ISamplerState(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mSamplerState(samplerState)
		{
			// Sanity checks
			SE_ASSERT(samplerState.filter != Rhi::FilterMode::UNKNOWN, "Direct3D 12 filter mode must not be unknown")
				SE_ASSERT(samplerState.maxAnisotropy <= direct3D12Rhi.getCapabilities().maximumAnisotropy, "Maximum Direct3D 12 anisotropy value violated")
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SamplerState() override
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Return the sampler state
		*
		*  @return
		*    The sampler state
		*/
		[[nodiscard]] inline const Rhi::SamplerState& getSamplerState() const
		{
			return mSamplerState;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(SamplerState, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit SamplerState(const SamplerState&) = delete;
		SamplerState& operator =(const SamplerState&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		Rhi::SamplerState mSamplerState;


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/RenderTarget/RenderPass.h               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 render pass interface
	*/
	class RenderPass final : public Rhi::IRenderPass
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] numberOfColorAttachments
		*    Number of color render target textures, must be <="Rhi::Capabilities::maximumNumberOfSimultaneousRenderTargets"
		*  @param[in] colorAttachmentTextureFormats
		*    The color render target texture formats, can be a null pointer or can contain null pointers, if not a null pointer there must be at
		*    least "numberOfColorAttachments" textures in the provided C-array of pointers
		*  @param[in] depthStencilAttachmentTextureFormat
		*    The optional depth stencil render target texture format, can be a "Rhi::TextureFormat::UNKNOWN" if there should be no depth buffer
		*/
		RenderPass(Rhi::IRhi& rhi, uint32_t numberOfColorAttachments, const Rhi::TextureFormat::Enum* colorAttachmentTextureFormats, Rhi::TextureFormat::Enum depthStencilAttachmentTextureFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IRenderPass(rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mNumberOfColorAttachments(numberOfColorAttachments),
			mDepthStencilAttachmentTextureFormat(depthStencilAttachmentTextureFormat)
		{
			SE_ASSERT(mNumberOfColorAttachments < 8, "Invalid number of Direct3D 12 color attachments")
				memcpy(mColorAttachmentTextureFormats, colorAttachmentTextureFormats, sizeof(Rhi::TextureFormat::Enum) * mNumberOfColorAttachments);
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~RenderPass() override
		{
		}

		/**
		*  @brief
		*    Return the number of color render target textures
		*
		*  @return
		*    The number of color render target textures
		*/
		[[nodiscard]] inline uint32_t getNumberOfColorAttachments() const
		{
			return mNumberOfColorAttachments;
		}

		/**
		*  @brief
		*    Return the number of render target textures (color and depth stencil)
		*
		*  @return
		*    The number of render target textures (color and depth stencil)
		*/
		[[nodiscard]] inline uint32_t getNumberOfAttachments() const
		{
			return (mDepthStencilAttachmentTextureFormat != Rhi::TextureFormat::Enum::UNKNOWN) ? (mNumberOfColorAttachments + 1) : mNumberOfColorAttachments;
		}

		/**
		*  @brief
		*    Return the color attachment texture format
		*
		*  @param[in] colorAttachmentIndex
		*    Color attachment index
		*
		*  @return
		*    The color attachment texture format
		*/
		[[nodiscard]] inline Rhi::TextureFormat::Enum getColorAttachmentTextureFormat(uint32_t colorAttachmentIndex) const
		{
			SE_ASSERT(colorAttachmentIndex < mNumberOfColorAttachments, "Invalid Direct3D 12 color attachment index")
				return mColorAttachmentTextureFormats[colorAttachmentIndex];
		}

		/**
		*  @brief
		*    Return the depth stencil attachment texture format
		*
		*  @return
		*    The depth stencil attachment texture format
		*/
		[[nodiscard]] inline Rhi::TextureFormat::Enum getDepthStencilAttachmentTextureFormat() const
		{
			return mDepthStencilAttachmentTextureFormat;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(RenderPass, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit RenderPass(const RenderPass&) = delete;
		RenderPass& operator =(const RenderPass&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		uint32_t				 mNumberOfColorAttachments;
		Rhi::TextureFormat::Enum mColorAttachmentTextureFormats[8];
		Rhi::TextureFormat::Enum mDepthStencilAttachmentTextureFormat;


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/QueryPool.h                             ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 asynchronous query pool interface
	*/
	class QueryPool final : public Rhi::IQueryPool
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] queryType
		*    Query type
		*  @param[in] numberOfQueries
		*    Number of queries
		*/
		QueryPool(Direct3D12Rhi& direct3D12Rhi, Rhi::QueryType queryType, uint32_t numberOfQueries RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IQueryPool(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mQueryType(queryType),
			mNumberOfQueries(numberOfQueries),
			mD3D12QueryHeap(nullptr),
			mD3D12ResourceQueryHeapResultReadback(nullptr),
			mResolveToFrameNumber(0)
		{
			ID3D12Device& d3d12Device = direct3D12Rhi.getD3D12Device();
			uint32_t numberOfBytesPerQuery = 0;

			{ // Get Direct3D 12 query description
				D3D12_QUERY_HEAP_DESC d3d12QueryHeapDesc = {};
				switch ( queryType )
				{
				case Rhi::QueryType::OCCLUSION:
					d3d12QueryHeapDesc.Type = D3D12_QUERY_HEAP_TYPE_OCCLUSION;
					numberOfBytesPerQuery = sizeof(uint64_t);
					break;

				case Rhi::QueryType::PIPELINE_STATISTICS:
					d3d12QueryHeapDesc.Type = D3D12_QUERY_HEAP_TYPE_PIPELINE_STATISTICS;
					numberOfBytesPerQuery = sizeof(D3D12_QUERY_DATA_PIPELINE_STATISTICS);
					break;

				case Rhi::QueryType::TIMESTAMP:
					d3d12QueryHeapDesc.Type = D3D12_QUERY_HEAP_TYPE_TIMESTAMP;
					numberOfBytesPerQuery = sizeof(uint64_t);
					break;
				}
				d3d12QueryHeapDesc.Count = numberOfQueries;

				// Create Direct3D 12 query heap
				FAILED_DEBUG_BREAK(d3d12Device.CreateQueryHeap(&d3d12QueryHeapDesc, IID_PPV_ARGS(&mD3D12QueryHeap)))
			}

			{ // Create the Direct3D 12 resource for query heap result readback
			  // -> Due to the asynchronous nature of queries (see "ID3D12GraphicsCommandList::ResolveQueryData()"), we need a result readback buffer which can hold enough frames
			  // +1 = One more frame as an instance is guaranteed to be written to if "Direct3D12Rhi::NUMBER_OF_FRAMES" frames have been submitted since. This is due to a fact that present stalls when none of the maximum number of frames are done/available.
				const CD3DX12_HEAP_PROPERTIES d3d12XHeapProperties(D3D12_HEAP_TYPE_READBACK);
				const D3D12_RESOURCE_DESC d3d12ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(static_cast<UINT64>(numberOfBytesPerQuery) * numberOfQueries * (Direct3D12Rhi::NUMBER_OF_FRAMES + 1));
				FAILED_DEBUG_BREAK(d3d12Device.CreateCommittedResource(&d3d12XHeapProperties, D3D12_HEAP_FLAG_NONE, &d3d12ResourceDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&mD3D12ResourceQueryHeapResultReadback)))
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			switch ( queryType )
			{
			case Rhi::QueryType::OCCLUSION:
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Occlusion query", 18)	// 18 = "Occlusion query: " including terminating zero
						if ( nullptr != mD3D12QueryHeap )
						{
							FAILED_DEBUG_BREAK(mD3D12QueryHeap->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
						}
					if ( nullptr != mD3D12ResourceQueryHeapResultReadback )
					{
						FAILED_DEBUG_BREAK(mD3D12ResourceQueryHeapResultReadback->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
					}
					break;
				}

			case Rhi::QueryType::PIPELINE_STATISTICS:
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Pipeline statistics query", 28)	// 28 = "Pipeline statistics query: " including terminating zero
						if ( nullptr != mD3D12QueryHeap )
						{
							FAILED_DEBUG_BREAK(mD3D12QueryHeap->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
						}
					if ( nullptr != mD3D12ResourceQueryHeapResultReadback )
					{
						FAILED_DEBUG_BREAK(mD3D12ResourceQueryHeapResultReadback->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
					}
					break;
				}

			case Rhi::QueryType::TIMESTAMP:
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Timestamp query", 18)	// 18 = "Timestamp query: " including terminating zero
						if ( nullptr != mD3D12QueryHeap )
						{
							FAILED_DEBUG_BREAK(mD3D12QueryHeap->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
						}
					if ( nullptr != mD3D12ResourceQueryHeapResultReadback )
					{
						FAILED_DEBUG_BREAK(mD3D12ResourceQueryHeapResultReadback->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
					}
					break;
				}
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~QueryPool() override
		{
			if ( nullptr != mD3D12QueryHeap )
			{
				mD3D12QueryHeap->Release();
			}
			if ( nullptr != mD3D12ResourceQueryHeapResultReadback )
			{
				mD3D12ResourceQueryHeapResultReadback->Release();
			}
		}

		/**
		*  @brief
		*    Return the query type
		*
		*  @return
		*    The query type
		*/
		[[nodiscard]] inline Rhi::QueryType getQueryType() const
		{
			return mQueryType;
		}

		/**
		*  @brief
		*    Return the number of queries
		*
		*  @return
		*    The number of queries
		*/
		[[nodiscard]] inline uint32_t getNumberOfQueries() const
		{
			return mNumberOfQueries;
		}

		/**
		*  @brief
		*    Return the Direct3D 12 query heap
		*
		*  @return
		*    The Direct3D 12 query heap
		*/
		[[nodiscard]] inline ID3D12QueryHeap* getD3D12QueryHeap() const
		{
			return mD3D12QueryHeap;
		}

		/**
		*  @brief
		*    Get asynchronous query pool results
		*
		*  @param[in] numberOfDataBytes
		*    Number of data bytes
		*  @param[out] data
		*    Receives the query data
		*  @param[in] firstQueryIndex
		*    First query index (e.g. 0)
		*  @param[in] numberOfQueries
		*    Number of queries (e.g. 1)
		*  @param[in] strideInBytes
		*    Stride in bytes, 0 is only valid in case there's just a single query
		*  @param[in] d3d12GraphicsCommandList
		*    Direct3D 12 graphics command list
		*/
		void getQueryPoolResults([[maybe_unused]] uint32_t numberOfDataBytes, uint8_t* data, uint32_t firstQueryIndex, uint32_t numberOfQueries, [[maybe_unused]] uint32_t strideInBytes, ID3D12GraphicsCommandList& d3d12GraphicsCommandList)
		{
			// Query pool type dependent processing
			// -> We don't support "Rhi::QueryResultFlags::WAIT"
			SE_ASSERT(firstQueryIndex < mNumberOfQueries, "Direct3D 12 out-of-bounds query index")
				SE_ASSERT((firstQueryIndex + numberOfQueries) <= mNumberOfQueries, "Direct3D 12 out-of-bounds query index")
				D3D12_QUERY_TYPE d3d12QueryType = D3D12_QUERY_TYPE_OCCLUSION;
			uint32_t numberOfBytesPerQuery = 0;
			switch ( mQueryType )
			{
			case Rhi::QueryType::OCCLUSION:
				{
					SE_ASSERT(1 == numberOfQueries || sizeof(uint64_t) == strideInBytes, "Direct3D 12 stride in bytes must be 8 bytes for occlusion query type")
						d3d12QueryType = D3D12_QUERY_TYPE_OCCLUSION;
					numberOfBytesPerQuery = sizeof(uint64_t);
					break;
				}

			case Rhi::QueryType::PIPELINE_STATISTICS:
				{
					static_assert(sizeof(Rhi::PipelineStatisticsQueryResult) == sizeof(D3D12_QUERY_DATA_PIPELINE_STATISTICS), "Direct3D 12 structure mismatch detected");
					SE_ASSERT(numberOfDataBytes >= sizeof(Rhi::PipelineStatisticsQueryResult), "Direct3D 12 out-of-memory query access")
						SE_ASSERT(1 == numberOfQueries || strideInBytes >= sizeof(Rhi::PipelineStatisticsQueryResult), "Direct3D 12 out-of-memory query access")
						SE_ASSERT(1 == numberOfQueries || sizeof(D3D12_QUERY_DATA_PIPELINE_STATISTICS) == strideInBytes, "Direct3D 12 stride in bytes must be 88 bytes for pipeline statistics query type")
						d3d12QueryType = D3D12_QUERY_TYPE_PIPELINE_STATISTICS;
					numberOfBytesPerQuery = sizeof(D3D12_QUERY_DATA_PIPELINE_STATISTICS);
					break;
				}

			case Rhi::QueryType::TIMESTAMP:	// TODO(co) Convert time to nanoseconds, see e.g. http://reedbeta.com/blog/gpu-profiling-101/
				{
					SE_ASSERT(1 == numberOfQueries || sizeof(uint64_t) == strideInBytes, "Direct3D 12 stride in bytes must be 8 bytes for timestamp query type")
						d3d12QueryType = D3D12_QUERY_TYPE_TIMESTAMP;
					numberOfBytesPerQuery = sizeof(uint64_t);
					break;
				}
			}

			{ // Resolve query data from the current frame
				const uint64_t resolveToBaseAddress = static_cast<uint64_t>(numberOfBytesPerQuery) * mNumberOfQueries * mResolveToFrameNumber + static_cast<uint64_t>(numberOfBytesPerQuery) * firstQueryIndex;
				d3d12GraphicsCommandList.ResolveQueryData(mD3D12QueryHeap, d3d12QueryType, firstQueryIndex, numberOfQueries, mD3D12ResourceQueryHeapResultReadback, resolveToBaseAddress);
			}

			// Readback query result by grabbing readback data for the queries from a finished frame "Direct3D12Rhi::NUMBER_OF_FRAMES" ago
			// +1 = One more frame as an instance is guaranteed to be written to if "Direct3D12Rhi::NUMBER_OF_FRAMES" frames have been submitted since. This is due to a fact that present stalls when none of the maximum number of frames are done/available.
			const uint32_t readbackFrameNumber = (mResolveToFrameNumber + 1) % (Direct3D12Rhi::NUMBER_OF_FRAMES + 1);
			const uint32_t readbackBaseOffset = numberOfBytesPerQuery * mNumberOfQueries * readbackFrameNumber + numberOfBytesPerQuery * firstQueryIndex;
			const D3D12_RANGE d3d12Range = { readbackBaseOffset, readbackBaseOffset + numberOfBytesPerQuery * numberOfQueries };
			uint64_t* timingData = nullptr;
			FAILED_DEBUG_BREAK(mD3D12ResourceQueryHeapResultReadback->Map(0, &d3d12Range, reinterpret_cast<void**>(&timingData)))
				memcpy(data, timingData, numberOfBytesPerQuery * numberOfQueries);
			mD3D12ResourceQueryHeapResultReadback->Unmap(0, nullptr);
			mResolveToFrameNumber = readbackFrameNumber;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(QueryPool, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit QueryPool(const QueryPool&) = delete;
		QueryPool& operator =(const QueryPool&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		Rhi::QueryType   mQueryType;
		uint32_t		 mNumberOfQueries;
		ID3D12QueryHeap* mD3D12QueryHeap;
		ID3D12Resource*	 mD3D12ResourceQueryHeapResultReadback;
		uint32_t		 mResolveToFrameNumber;


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/RenderTarget/SwapChain.h                ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 swap chain class
	*/
	class SwapChain final : public Rhi::ISwapChain
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] renderPass
		*    Render pass to use, the swap chain keeps a reference to the render pass
		*  @param[in] windowHandle
		*    Information about the window to render into
		*/
		SwapChain(Rhi::IRenderPass& renderPass, Rhi::WindowHandle windowHandle RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			ISwapChain(renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mDxgiSwapChain3(nullptr),
			mD3D12DescriptorHeapRenderTargetView(nullptr),
			mD3D12DescriptorHeapDepthStencilView(nullptr),
			mRenderTargetViewDescriptorSize(0),
			mD3D12ResourceRenderTargets{},
			mD3D12ResourceDepthStencil(nullptr),
			mSynchronizationInterval(0),
			mFrameIndex(0),
			mFenceEvent(nullptr),
			mD3D12Fence(nullptr),
			mFenceValue(0)
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(renderPass.getRhi());
			const RenderPass& d3d12RenderPass = static_cast<RenderPass&>(renderPass);

			// Sanity check
			SE_ASSERT(1 == d3d12RenderPass.getNumberOfColorAttachments(), "There must be exactly one Direct3D 12 render pass color attachment")

				// Get the native window handle
				const HWND hWnd = reinterpret_cast<HWND>(windowHandle.nativeWindowHandle);

			// Get our IDXGI factory instance
			IDXGIFactory4& dxgiFactory4 = direct3D12Rhi.getDxgiFactory4();

			// Get the width and height of the given native window and ensure they are never ever zero
			// -> See "getSafeWidthAndHeight()"-method comments for details
			long width = 1;
			long height = 1;
			{
				// Get the client rectangle of the given native window
				RECT rect;
				::GetClientRect(hWnd, &rect);

				// Get the width and height...
				width = rect.right - rect.left;
				height = rect.bottom - rect.top;

				// ... and ensure that none of them is ever zero
				if ( width < 1 )
				{
					width = 1;
				}
				if ( height < 1 )
				{
					height = 1;
				}
			}

			// TODO(co) Add tearing support, see Direct3D 11 RHI implementation
			// Determines whether tearing support is available for fullscreen borderless windows
			// -> To unlock frame rates of UWP applications on the Windows Store and providing support for both AMD Freesync and NVIDIA's G-SYNC we must explicitly allow tearing
			// -> See "Windows Dev Center" -> "Variable refresh rate displays": https://msdn.microsoft.com/en-us/library/windows/desktop/mt742104(v=vs.85).aspx

			// Create the swap chain
			DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc = {};
			dxgiSwapChainDesc.BufferCount = Direct3D12Rhi::NUMBER_OF_FRAMES;
			dxgiSwapChainDesc.BufferDesc.Width = static_cast<UINT>(width);
			dxgiSwapChainDesc.BufferDesc.Height = static_cast<UINT>(height);
			dxgiSwapChainDesc.BufferDesc.Format = Mapping::getDirect3D12Format(d3d12RenderPass.getColorAttachmentTextureFormat(0));
			dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
			dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			dxgiSwapChainDesc.OutputWindow = hWnd;
			dxgiSwapChainDesc.SampleDesc.Count = 1;
			dxgiSwapChainDesc.Windowed = TRUE;
			IDXGISwapChain* dxgiSwapChain = nullptr;
			FAILED_DEBUG_BREAK(dxgiFactory4.CreateSwapChain(direct3D12Rhi.getD3D12CommandQueue(), &dxgiSwapChainDesc, &dxgiSwapChain))
				if ( FAILED(dxgiSwapChain->QueryInterface(IID_PPV_ARGS(&mDxgiSwapChain3))) )
				{
					RHI_LOG(CRITICAL, "Failed to retrieve the Direct3D 12 DXGI swap chain 3")
				}
			dxgiSwapChain->Release();

			// Disable alt-return for automatic fullscreen state change
			// -> We handle this manually to have more control over it
			FAILED_DEBUG_BREAK(dxgiFactory4.MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER))

				// Create the Direct3D 12 views
				if ( nullptr != mDxgiSwapChain3 )
				{
					createDirect3D12Views();
				}

			// Create synchronization objects
			if ( SUCCEEDED(direct3D12Rhi.getD3D12Device().CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mD3D12Fence))) )
			{
				mFenceValue = 1;

				// Create an event handle to use for frame synchronization
				mFenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
				if ( nullptr == mFenceEvent )
				{
					RHI_LOG(CRITICAL, "Failed to create an Direct3D 12 event handle to use for frame synchronization. Error code %u", ::GetLastError())
				}
			}
			else
			{
				RHI_LOG(CRITICAL, "Failed to create Direct3D 12 fence instance")
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Swap chain", 13)	// 13 = "Swap chain: " including terminating zero

			// Assign a debug name to the DXGI swap chain
				if ( nullptr != mDxgiSwapChain3 )
				{
					FAILED_DEBUG_BREAK(mDxgiSwapChain3->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
				}

			// Assign a debug name to the Direct3D 12 frame resources
			for ( int frame = 0; frame < Direct3D12Rhi::NUMBER_OF_FRAMES; ++frame )
			{
				if ( nullptr != mD3D12ResourceRenderTargets[frame] )
				{
					FAILED_DEBUG_BREAK(mD3D12ResourceRenderTargets[frame]->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
				}
			}
			if ( nullptr != mD3D12ResourceDepthStencil )
			{
				FAILED_DEBUG_BREAK(mD3D12ResourceDepthStencil->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
			}

			// Assign a debug name to the Direct3D 12 descriptor heaps
			if ( nullptr != mD3D12DescriptorHeapRenderTargetView )
			{
				FAILED_DEBUG_BREAK(mD3D12DescriptorHeapRenderTargetView->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
			}
			if ( nullptr != mD3D12DescriptorHeapDepthStencilView )
			{
				FAILED_DEBUG_BREAK(mD3D12DescriptorHeapDepthStencilView->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SwapChain() override
		{
			// "DXGI Overview - Destroying a Swap Chain" at MSDN http://msdn.microsoft.com/en-us/library/bb205075.aspx states
			//   "You may not release a swap chain in full-screen mode because doing so may create thread contention (which will
			//    cause DXGI to raise a non-continuable exception). Before releasing a swap chain, first switch to windowed mode
			//    (using IDXGISwapChain::SetFullscreenState( FALSE, NULL )) and then call IUnknown::Release."
			if ( getFullscreenState() )
			{
				setFullscreenState(false);
			}

			// Release the used resources
			destroyDirect3D12Views();
			if ( nullptr != mDxgiSwapChain3 )
			{
				mDxgiSwapChain3->Release();
			}

			// Destroy synchronization objects
			if ( nullptr != mFenceEvent )
			{
				::CloseHandle(mFenceEvent);
			}
			if ( nullptr != mD3D12Fence )
			{
				mD3D12Fence->Release();
			}
		}

		/**
		*  @brief
		*    Return the DXGI swap chain 3 instance
		*
		*  @return
		*    The DXGI swap chain 3 instance, null pointer on error, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline IDXGISwapChain3* getDxgiSwapChain3() const
		{
			return mDxgiSwapChain3;
		}

		/**
		*  @brief
		*    Return the Direct3D 12 render target view descriptor heap instance
		*
		*  @return
		*    The Direct3D 12 render target view descriptor heap instance, null pointer on error, do not release the returned instance unless you added an own reference to it
		*
		*  @note
		*    - It's highly recommended to not keep any references to the returned instance, else issues may occur when resizing the swap chain
		*/
		[[nodiscard]] inline ID3D12DescriptorHeap* getD3D12DescriptorHeapRenderTargetView() const
		{
			return mD3D12DescriptorHeapRenderTargetView;
		}

		/**
		*  @brief
		*    Return the Direct3D 12 depth stencil view descriptor heap instance
		*
		*  @return
		*    The Direct3D 12 depth stencil view descriptor heap instance, null pointer on error, do not release the returned instance unless you added an own reference to it
		*
		*  @note
		*    - It's highly recommended to not keep any references to the returned instance, else issues may occur when resizing the swap chain
		*/
		[[nodiscard]] inline ID3D12DescriptorHeap* getD3D12DescriptorHeapDepthStencilView() const
		{
			return mD3D12DescriptorHeapDepthStencilView;
		}

		/**
		*  @brief
		*    Return the render target view descriptor size
		*
		*  @return
		*    The render target view descriptor size
		*
		*  @note
		*    - It's highly recommended to not keep any backups of this value, else issues may occur when resizing the swap chain
		*/
		[[nodiscard]] inline UINT getRenderTargetViewDescriptorSize() const
		{
			return mRenderTargetViewDescriptorSize;
		}

		/**
		*  @brief
		*    Return the index of the Direct3D 12 resource render target which is currently used as back buffer
		*
		*  @return
		*    The index of the Direct3D 12 resource render target which is currently used as back buffer
		*
		*  @note
		*    - It's highly recommended to not keep any references to the returned instance, else issues may occur when resizing the swap chain
		*/
		[[nodiscard]] inline UINT getBackD3D12ResourceRenderTargetFrameIndex() const
		{
			return mFrameIndex;
		}

		/**
		*  @brief
		*    Return the Direct3D 12 resource render target which is currently used as back buffer
		*
		*  @return
		*    The Direct3D 12 resource render target which is currently used as back buffer, null pointer on error, do not release the returned instance unless you added an own reference to it
		*
		*  @note
		*    - It's highly recommended to not keep any references to the returned instance, else issues may occur when resizing the swap chain
		*/
		[[nodiscard]] inline ID3D12Resource* getBackD3D12ResourceRenderTarget() const
		{
			return mD3D12ResourceRenderTargets[mFrameIndex];
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IRenderTarget methods             ]
		//[-------------------------------------------------------]
	public:
		virtual void getWidthAndHeight(uint32_t& width, uint32_t& height) const override
		{
			// Is there a valid swap chain?
			if ( nullptr != mDxgiSwapChain3 )
			{
				// Get the Direct3D 12 swap chain description
				DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
				FAILED_DEBUG_BREAK(mDxgiSwapChain3->GetDesc(&dxgiSwapChainDesc))

					// Get the width and height
					long swapChainWidth = 1;
				long swapChainHeight = 1;
				{
					// Get the client rectangle of the native output window
					// -> Don't use the width and height stored in "DXGI_SWAP_CHAIN_DESC" -> "DXGI_MODE_DESC"
					//    because it might have been modified in order to avoid zero values
					RECT rect;
					::GetClientRect(dxgiSwapChainDesc.OutputWindow, &rect);

					// Get the width and height...
					swapChainWidth = rect.right - rect.left;
					swapChainHeight = rect.bottom - rect.top;

					// ... and ensure that none of them is ever zero
					if ( swapChainWidth < 1 )
					{
						swapChainWidth = 1;
					}
					if ( swapChainHeight < 1 )
					{
						swapChainHeight = 1;
					}
				}

				// Write out the width and height
				width = static_cast<UINT>(swapChainWidth);
				height = static_cast<UINT>(swapChainHeight);
			}
			else
			{
				// Set known default return values
				width = 1;
				height = 1;
			}
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::ISwapChain methods                ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual handle getNativeWindowHandle() const override
		{
			// Is there a valid swap chain?
			if ( nullptr != mDxgiSwapChain3 )
			{
				// Get the Direct3D 12 swap chain description
				DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
				FAILED_DEBUG_BREAK(mDxgiSwapChain3->GetDesc(&dxgiSwapChainDesc))

					// Return the native window handle
					return reinterpret_cast<handle>(dxgiSwapChainDesc.OutputWindow);
			}

			// Error!
			return SE_NULL_HANDLE;
		}

		inline virtual void setVerticalSynchronizationInterval(uint32_t synchronizationInterval) override
		{
			mSynchronizationInterval = synchronizationInterval;
		}

		virtual void present() override
		{
			// Is there a valid swap chain?
			if ( nullptr != mDxgiSwapChain3 )
			{
				handleDeviceLost(static_cast<Direct3D12Rhi&>(getRenderPass().getRhi()), mDxgiSwapChain3->Present(mSynchronizationInterval, 0));

				// Wait for the GPU to be done with all resources
				waitForPreviousFrame();
			}
		}

		virtual void resizeBuffers() override
		{
			// Is there a valid swap chain?
			if ( nullptr != mDxgiSwapChain3 )
			{
				Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());

				// Get the currently set render target
				Rhi::IRenderTarget* renderTargetBackup = direct3D12Rhi.omGetRenderTarget();

				// In case this swap chain is the current render target, we have to unset it before continuing
				if ( this == renderTargetBackup )
				{
					direct3D12Rhi.setGraphicsRenderTarget(nullptr);
				}
				else
				{
					renderTargetBackup = nullptr;
				}

				// Release the views
				destroyDirect3D12Views();

				// Get the swap chain width and height, ensures they are never ever zero
				UINT width = 1;
				UINT height = 1;
				getSafeWidthAndHeight(width, height);

				// Resize the Direct3D 12 swap chain
				// -> Preserve the existing buffer count and format
				const HRESULT result = mDxgiSwapChain3->ResizeBuffers(Direct3D12Rhi::NUMBER_OF_FRAMES, width, height, Mapping::getDirect3D12Format(static_cast<RenderPass&>(getRenderPass()).getColorAttachmentTextureFormat(0)), 0);
				if ( SUCCEEDED(result) )
				{
					// Create the Direct3D 12 views
					// TODO(co) Rescue and reassign the resource debug name
					createDirect3D12Views();

					// If required, restore the previously set render target
					if ( nullptr != renderTargetBackup )
					{
						direct3D12Rhi.setGraphicsRenderTarget(renderTargetBackup);
					}
				}
				else
				{
					handleDeviceLost(direct3D12Rhi, result);
				}
			}
		}

		[[nodiscard]] virtual bool getFullscreenState() const override
		{
			// Window mode by default
			BOOL fullscreen = FALSE;

			// Is there a valid swap chain?
			if ( nullptr != mDxgiSwapChain3 )
			{
				FAILED_DEBUG_BREAK(mDxgiSwapChain3->GetFullscreenState(&fullscreen, nullptr))
			}

			// Done
			return (FALSE != fullscreen);
		}

		virtual void setFullscreenState(bool fullscreen) override
		{
			// Is there a valid swap chain?
			if ( nullptr != mDxgiSwapChain3 )
			{
				const HRESULT result = mDxgiSwapChain3->SetFullscreenState(fullscreen, nullptr);
				if ( FAILED(result) )
				{
					// TODO(co) Better error handling
					RHI_LOG(CRITICAL, "Failed to set Direct3D 12 fullscreen state")
				}
			}
		}

		inline virtual void setRenderWindow([[maybe_unused]] Rhi::IRenderWindow* renderWindow) override
		{
			// TODO(sw) implement me
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(SwapChain, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit SwapChain(const SwapChain&) = delete;
		SwapChain& operator =(const SwapChain&) = delete;

		/**
		*  @brief
		*    Return the swap chain width and height
		*
		*  @param[out] width
		*    Receives the swap chain width
		*  @param[out] height
		*    Receives the swap chain height
		*
		*  @remarks
		*    For instance "IDXGISwapChain::ResizeBuffers()" can automatically choose the width and height to match the client
		*    rectangle of the native window, but as soon as the width or height is zero we will get the error message
		*     "DXGI Error: The buffer height inferred from the output window is zero. Taking 8 as a reasonable default instead"
		*     "D3D12: ERROR: ID3D12Device::CreateTexture2D: The Dimensions are invalid. For feature level D3D_FEATURE_LEVEL_12_0,
		*      the Width (value = 116) must be between 1 and 16384, inclusively. The Height (value = 0) must be between 1 and 16384,
		*      inclusively. And, the ArraySize (value = 1) must be between 1 and 2048, inclusively. [ STATE_CREATION ERROR #101: CREATETEXTURE2D_INVALIDDIMENSIONS ]"
		*   including an evil memory leak. So, best to use this method which gets the width and height of the native output
		*   window manually and ensures it's never zero.
		*
		*  @note
		*    - "mDxgiSwapChain3" must be valid when calling this method
		*/
		void getSafeWidthAndHeight(uint32_t& width, uint32_t& height) const
		{
			// Get the Direct3D 12 swap chain description
			DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
			FAILED_DEBUG_BREAK(mDxgiSwapChain3->GetDesc(&dxgiSwapChainDesc))

				// Get the client rectangle of the native output window
				RECT rect;
			::GetClientRect(dxgiSwapChainDesc.OutputWindow, &rect);

			// Get the width and height...
			long swapChainWidth = rect.right - rect.left;
			long swapChainHeight = rect.bottom - rect.top;

			// ... and ensure that none of them is ever zero
			if ( swapChainWidth < 1 )
			{
				swapChainWidth = 1;
			}
			if ( swapChainHeight < 1 )
			{
				swapChainHeight = 1;
			}

			// Write out the width and height
			width = static_cast<UINT>(swapChainWidth);
			height = static_cast<UINT>(swapChainHeight);
		}

		/**
		*  @brief
		*    Create the Direct3D 12 views
		*/
		void createDirect3D12Views()
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());
			SE_ASSERT(nullptr != mDxgiSwapChain3, "Invalid Direct3D 12 DXGI swap chain 3")

				// TODO(co) Debug name gets lost when resizing a window, fix this

				// Get the Direct3D 12 device instance
				ID3D12Device& d3d12Device = direct3D12Rhi.getD3D12Device();

			{ // Describe and create a render target view (RTV) descriptor heap
				D3D12_DESCRIPTOR_HEAP_DESC d3d12DescriptorHeapDesc = {};
				d3d12DescriptorHeapDesc.NumDescriptors = Direct3D12Rhi::NUMBER_OF_FRAMES;
				d3d12DescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
				d3d12DescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				if ( SUCCEEDED(d3d12Device.CreateDescriptorHeap(&d3d12DescriptorHeapDesc, IID_PPV_ARGS(&mD3D12DescriptorHeapRenderTargetView))) )
				{
					mRenderTargetViewDescriptorSize = d3d12Device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

					{ // Create frame resources
						CD3DX12_CPU_DESCRIPTOR_HANDLE d3d12XCpuDescriptorHandle(mD3D12DescriptorHeapRenderTargetView->GetCPUDescriptorHandleForHeapStart());

						// Create a RTV for each frame
						for ( UINT frame = 0; frame < Direct3D12Rhi::NUMBER_OF_FRAMES; ++frame )
						{
							if ( SUCCEEDED(mDxgiSwapChain3->GetBuffer(frame, IID_PPV_ARGS(&mD3D12ResourceRenderTargets[frame]))) )
							{
								d3d12Device.CreateRenderTargetView(mD3D12ResourceRenderTargets[frame], nullptr, d3d12XCpuDescriptorHandle);
								d3d12XCpuDescriptorHandle.Offset(1, mRenderTargetViewDescriptorSize);
							}
							else
							{
								RHI_LOG(CRITICAL, "Failed to retrieve frame buffer from Direct3D 12 DXGI swap chain")
							}
						}
					}

					mFrameIndex = mDxgiSwapChain3->GetCurrentBackBufferIndex();
				}
				else
				{
					RHI_LOG(CRITICAL, "Failed to describe and create a Direct3D 12 render target view (RTV) descriptor heap")
				}
			}

			// Describe and create a depth stencil view (DSV) descriptor heap
			const Rhi::TextureFormat::Enum depthStencilAttachmentTextureFormat = static_cast<RenderPass&>(getRenderPass()).getDepthStencilAttachmentTextureFormat();
			if ( Rhi::TextureFormat::Enum::UNKNOWN != depthStencilAttachmentTextureFormat )
			{
				D3D12_DESCRIPTOR_HEAP_DESC d3d12DescriptorHeapDesc = {};
				d3d12DescriptorHeapDesc.NumDescriptors = 1;
				d3d12DescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
				d3d12DescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				if ( SUCCEEDED(d3d12Device.CreateDescriptorHeap(&d3d12DescriptorHeapDesc, IID_PPV_ARGS(&mD3D12DescriptorHeapDepthStencilView))) )
				{
					D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
					depthStencilDesc.Format = Mapping::getDirect3D12Format(depthStencilAttachmentTextureFormat);
					depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
					depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

					D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
					depthOptimizedClearValue.Format = depthStencilDesc.Format;
					depthOptimizedClearValue.DepthStencil.Depth = 0.0f;	// z = 0 instead of 1 due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
					depthOptimizedClearValue.DepthStencil.Stencil = 0;

					// Get the swap chain width and height, ensures they are never ever zero
					UINT width = 1;
					UINT height = 1;
					getSafeWidthAndHeight(width, height);

					const CD3DX12_HEAP_PROPERTIES d3d12XHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
					const CD3DX12_RESOURCE_DESC d3d12XResourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(depthStencilDesc.Format, width, height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
					if ( SUCCEEDED(d3d12Device.CreateCommittedResource(
						&d3d12XHeapProperties,
						D3D12_HEAP_FLAG_NONE,
						&d3d12XResourceDesc,
						D3D12_RESOURCE_STATE_DEPTH_WRITE,
						&depthOptimizedClearValue,
						IID_PPV_ARGS(&mD3D12ResourceDepthStencil)
					)) )
					{
						d3d12Device.CreateDepthStencilView(mD3D12ResourceDepthStencil, &depthStencilDesc, mD3D12DescriptorHeapDepthStencilView->GetCPUDescriptorHandleForHeapStart());
					}
					else
					{
						RHI_LOG(CRITICAL, "Failed to create the Direct3D 12 depth stencil view (DSV) resource")
					}
				}
				else
				{
					RHI_LOG(CRITICAL, "Failed to describe and create a Direct3D 12 depth stencil view (DSV) descriptor heap")
				}
			}
		}

		/**
		*  @brief
		*    Destroy the Direct3D 12 views
		*/
		void destroyDirect3D12Views()
		{
			// Wait for the GPU to be done with all resources
			waitForPreviousFrame();

			// Release Direct3D 12 resources
			for ( int frame = 0; frame < Direct3D12Rhi::NUMBER_OF_FRAMES; ++frame )
			{
				if ( nullptr != mD3D12ResourceRenderTargets[frame] )
				{
					mD3D12ResourceRenderTargets[frame]->Release();
					mD3D12ResourceRenderTargets[frame] = nullptr;
				}
			}
			if ( nullptr != mD3D12ResourceDepthStencil )
			{
				mD3D12ResourceDepthStencil->Release();
				mD3D12ResourceDepthStencil = nullptr;
			}

			// Release Direct3D 12 descriptor heap
			if ( nullptr != mD3D12DescriptorHeapRenderTargetView )
			{
				mD3D12DescriptorHeapRenderTargetView->Release();
				mD3D12DescriptorHeapRenderTargetView = nullptr;
			}
			if ( nullptr != mD3D12DescriptorHeapDepthStencilView )
			{
				mD3D12DescriptorHeapDepthStencilView->Release();
				mD3D12DescriptorHeapDepthStencilView = nullptr;
			}
		}

		/**
		*  @brief
		*    Wait for the GPU to be done with all resources
		*/
		void waitForPreviousFrame()
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());
			SE_ASSERT(nullptr != mDxgiSwapChain3, "Invalid Direct3D 12 DXGI swap chain 3")

				// TODO(co) This is the most simple but least effective approach and only meant for the Direct3D 12 RHI implementation kickoff.

				// Signal and increment the fence value
				const UINT64 fence = mFenceValue;
			if ( SUCCEEDED(direct3D12Rhi.getD3D12CommandQueue()->Signal(mD3D12Fence, fence)) )
			{
				++mFenceValue;

				// Wait until the previous frame is finished
				if ( mD3D12Fence->GetCompletedValue() < fence )
				{
					if ( SUCCEEDED(mD3D12Fence->SetEventOnCompletion(fence, mFenceEvent)) )
					{
						::WaitForSingleObject(mFenceEvent, INFINITE);
					}
					else
					{
						RHI_LOG(CRITICAL, "Failed to set Direct3D 12 event on completion")
					}
				}

				mFrameIndex = mDxgiSwapChain3->GetCurrentBackBufferIndex();
			}
		}


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		IDXGISwapChain3*	  mDxgiSwapChain3;												// The DXGI swap chain 3 instance, null pointer on error
		ID3D12DescriptorHeap* mD3D12DescriptorHeapRenderTargetView;							// The Direct3D 12 render target view descriptor heap instance, null pointer on error
		ID3D12DescriptorHeap* mD3D12DescriptorHeapDepthStencilView;							// The Direct3D 12 depth stencil view descriptor heap instance, null pointer on error
		UINT				  mRenderTargetViewDescriptorSize;								// Render target view descriptor size
		ID3D12Resource*		  mD3D12ResourceRenderTargets[Direct3D12Rhi::NUMBER_OF_FRAMES];	// The Direct3D 12 render target instances, null pointer on error
		ID3D12Resource*		  mD3D12ResourceDepthStencil;									// The Direct3D 12 depth stencil instance, null pointer on error

		// Synchronization objects
		uint32_t	 mSynchronizationInterval;
		UINT		 mFrameIndex;
		HANDLE		 mFenceEvent;
		ID3D12Fence* mD3D12Fence;
		UINT64		 mFenceValue;


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/RenderTarget/Framebuffer.h              ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 framebuffer class
	*
	*  @todo
	*    - TODO(co) "D3D12GraphicsCommandList::OMSetRenderTargets()" supports using a single Direct3D 12 render target view descriptor heap instance with multiple targets in it, use it
	*/
	class Framebuffer final : public Rhi::IFramebuffer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] renderPass
		*    Render pass to use, the swap chain keeps a reference to the render pass
		*  @param[in] colorFramebufferAttachments
		*    The color render target textures, can be a null pointer or can contain null pointers, if not a null pointer there must be at
		*    least "Rhi::IRenderPass::getNumberOfColorAttachments()" textures in the provided C-array of pointers
		*  @param[in] depthStencilFramebufferAttachment
		*    The depth stencil render target texture, can be a null pointer
		*
		*  @note
		*    - The framebuffer keeps a reference to the provided texture instances
		*/
		Framebuffer(Rhi::IRenderPass& renderPass, const Rhi::FramebufferAttachment* colorFramebufferAttachments, const Rhi::FramebufferAttachment* depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IFramebuffer(renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mNumberOfColorTextures(static_cast<RenderPass&>(renderPass).getNumberOfColorAttachments()),
			mColorTextures(nullptr),	// Set below
			mDepthStencilTexture(nullptr),
			mWidth(UINT_MAX),
			mHeight(UINT_MAX),
			mD3D12DescriptorHeapRenderTargetViews(nullptr),
			mD3D12DescriptorHeapDepthStencilView(nullptr)
		{
			// Get the Direct3D 12 device instance
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(renderPass.getRhi());
			ID3D12Device& d3d12Device = direct3D12Rhi.getD3D12Device();

			// Add a reference to the used color textures
			if ( mNumberOfColorTextures > 0 )
			{
				mColorTextures = RHI_MALLOC_TYPED(Rhi::ITexture*, mNumberOfColorTextures);
				mD3D12DescriptorHeapRenderTargetViews = RHI_MALLOC_TYPED(ID3D12DescriptorHeap*, mNumberOfColorTextures);

				// Loop through all color textures
				ID3D12DescriptorHeap** d3d12DescriptorHeapRenderTargetView = mD3D12DescriptorHeapRenderTargetViews;
				Rhi::ITexture** colorTexturesEnd = mColorTextures + mNumberOfColorTextures;
				for ( Rhi::ITexture** colorTexture = mColorTextures; colorTexture < colorTexturesEnd; ++colorTexture, ++colorFramebufferAttachments, ++d3d12DescriptorHeapRenderTargetView )
				{
					// Sanity check
					SE_ASSERT(nullptr != colorFramebufferAttachments->texture, "Invalid Direct3D 12 color framebuffer attachment texture")

						// TODO(co) Add security check: Is the given resource one of the currently used RHI?
						*colorTexture = colorFramebufferAttachments->texture;
					(*colorTexture)->AddReference();

					// Evaluate the color texture type
					switch ( (*colorTexture)->getResourceType() )
					{
					case Rhi::ResourceType::TEXTURE_2D:
						{
							const Texture2D* texture2D = static_cast<Texture2D*>(*colorTexture);

							// Sanity checks
							SE_ASSERT(colorFramebufferAttachments->mipmapIndex < Rhi::ITexture2D::getNumberOfMipmaps(texture2D->getWidth(), texture2D->getHeight()), "Invalid Direct3D 12 color framebuffer attachment mipmap index")
								SE_ASSERT(0 == colorFramebufferAttachments->layerIndex, "Invalid Direct3D 12 color framebuffer attachment layer index")

								// Update the framebuffer width and height if required
								::detail::updateWidthHeight(colorFramebufferAttachments->mipmapIndex, texture2D->getWidth(), texture2D->getHeight(), mWidth, mHeight);

							// Get the Direct3D 12 resource
							ID3D12Resource* d3d12Resource = texture2D->getD3D12Resource();

							// Create the Direct3D 12 render target view instance
							D3D12_DESCRIPTOR_HEAP_DESC d3d12DescriptorHeapDesc = {};
							d3d12DescriptorHeapDesc.NumDescriptors = 1;
							d3d12DescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
							if ( SUCCEEDED(d3d12Device.CreateDescriptorHeap(&d3d12DescriptorHeapDesc, IID_PPV_ARGS(d3d12DescriptorHeapRenderTargetView))) )
							{
								D3D12_RENDER_TARGET_VIEW_DESC d3d12RenderTargetViewDesc = {};
								d3d12RenderTargetViewDesc.Format = static_cast<DXGI_FORMAT>(texture2D->getDxgiFormat());
								d3d12RenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
								d3d12RenderTargetViewDesc.Texture2D.MipSlice = colorFramebufferAttachments->mipmapIndex;
								d3d12Device.CreateRenderTargetView(d3d12Resource, &d3d12RenderTargetViewDesc, (*d3d12DescriptorHeapRenderTargetView)->GetCPUDescriptorHandleForHeapStart());
							}
							break;
						}

					case Rhi::ResourceType::TEXTURE_2D_ARRAY:
						{
							// Update the framebuffer width and height if required
							const Texture2DArray* texture2DArray = static_cast<Texture2DArray*>(*colorTexture);
							::detail::updateWidthHeight(colorFramebufferAttachments->mipmapIndex, texture2DArray->getWidth(), texture2DArray->getHeight(), mWidth, mHeight);

							// Get the Direct3D 12 resource
							ID3D12Resource* d3d12Resource = texture2DArray->getD3D12Resource();

							// Create the Direct3D 12 render target view instance
							D3D12_DESCRIPTOR_HEAP_DESC d3d12DescriptorHeapDesc = {};
							d3d12DescriptorHeapDesc.NumDescriptors = 1;
							d3d12DescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
							if ( SUCCEEDED(d3d12Device.CreateDescriptorHeap(&d3d12DescriptorHeapDesc, IID_PPV_ARGS(d3d12DescriptorHeapRenderTargetView))) )
							{
								D3D12_RENDER_TARGET_VIEW_DESC d3d12RenderTargetViewDesc = {};
								d3d12RenderTargetViewDesc.Format = static_cast<DXGI_FORMAT>(texture2DArray->getDxgiFormat());
								d3d12RenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
								d3d12RenderTargetViewDesc.Texture2DArray.MipSlice = colorFramebufferAttachments->mipmapIndex;
								d3d12RenderTargetViewDesc.Texture2DArray.FirstArraySlice = colorFramebufferAttachments->layerIndex;
								d3d12RenderTargetViewDesc.Texture2DArray.ArraySize = 1;
								d3d12RenderTargetViewDesc.Texture2DArray.PlaneSlice = 0;
								d3d12Device.CreateRenderTargetView(d3d12Resource, &d3d12RenderTargetViewDesc, (*d3d12DescriptorHeapRenderTargetView)->GetCPUDescriptorHandleForHeapStart());
							}
							break;
						}

					case Rhi::ResourceType::ROOT_SIGNATURE:
					case Rhi::ResourceType::RESOURCE_GROUP:
					case Rhi::ResourceType::GRAPHICS_PROGRAM:
					case Rhi::ResourceType::VERTEX_ARRAY:
					case Rhi::ResourceType::RENDER_PASS:
					case Rhi::ResourceType::QUERY_POOL:
					case Rhi::ResourceType::SWAP_CHAIN:
					case Rhi::ResourceType::FRAMEBUFFER:
					case Rhi::ResourceType::VERTEX_BUFFER:
					case Rhi::ResourceType::INDEX_BUFFER:
					case Rhi::ResourceType::TEXTURE_BUFFER:
					case Rhi::ResourceType::STRUCTURED_BUFFER:
					case Rhi::ResourceType::INDIRECT_BUFFER:
					case Rhi::ResourceType::UNIFORM_BUFFER:
					case Rhi::ResourceType::TEXTURE_1D:
					case Rhi::ResourceType::TEXTURE_1D_ARRAY:
					case Rhi::ResourceType::TEXTURE_3D:
					case Rhi::ResourceType::TEXTURE_CUBE:
					case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
					case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
					case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
					case Rhi::ResourceType::SAMPLER_STATE:
					case Rhi::ResourceType::VERTEX_SHADER:
					case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
					case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
					case Rhi::ResourceType::GEOMETRY_SHADER:
					case Rhi::ResourceType::FRAGMENT_SHADER:
					case Rhi::ResourceType::TASK_SHADER:
					case Rhi::ResourceType::MESH_SHADER:
					case Rhi::ResourceType::COMPUTE_SHADER:
					default:
						RHI_LOG(CRITICAL, "The type of the given color texture at index %u is not supported by the Direct3D 12 RHI implementation", colorTexture - mColorTextures)
							*d3d12DescriptorHeapRenderTargetView = nullptr;
						break;
					}
				}
			}

			// Add a reference to the used depth stencil texture
			if ( nullptr != depthStencilFramebufferAttachment )
			{
				mDepthStencilTexture = depthStencilFramebufferAttachment->texture;
				SE_ASSERT(nullptr != mDepthStencilTexture, "Invalid Direct3D 12 depth stencil framebuffer attachment texture")
					mDepthStencilTexture->AddReference();

				// Evaluate the depth stencil texture type
				switch ( mDepthStencilTexture->getResourceType() )
				{
				case Rhi::ResourceType::TEXTURE_2D:
					{
						const Texture2D* texture2D = static_cast<Texture2D*>(mDepthStencilTexture);

						// Sanity checks
						SE_ASSERT(depthStencilFramebufferAttachment->mipmapIndex < Rhi::ITexture2D::getNumberOfMipmaps(texture2D->getWidth(), texture2D->getHeight()), "Invalid Direct3D 12 depth stencil framebuffer attachment mipmap index")
							SE_ASSERT(0 == depthStencilFramebufferAttachment->layerIndex, "Invalid Direct3D 12 depth stencil framebuffer attachment layer index")

							// Update the framebuffer width and height if required
							::detail::updateWidthHeight(depthStencilFramebufferAttachment->mipmapIndex, texture2D->getWidth(), texture2D->getHeight(), mWidth, mHeight);

						// Get the Direct3D 12 resource
						ID3D12Resource* d3d12Resource = texture2D->getD3D12Resource();

						// Create the Direct3D 12 render target view instance
						D3D12_DESCRIPTOR_HEAP_DESC d3d12DescriptorHeapDesc = {};
						d3d12DescriptorHeapDesc.NumDescriptors = 1;
						d3d12DescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
						if ( SUCCEEDED(d3d12Device.CreateDescriptorHeap(&d3d12DescriptorHeapDesc, IID_PPV_ARGS(&mD3D12DescriptorHeapDepthStencilView))) )
						{
							D3D12_RENDER_TARGET_VIEW_DESC d3d12RenderTargetViewDesc = {};
							d3d12RenderTargetViewDesc.Format = static_cast<DXGI_FORMAT>(texture2D->getDxgiFormat());
							d3d12RenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
							d3d12RenderTargetViewDesc.Texture2D.MipSlice = depthStencilFramebufferAttachment->mipmapIndex;
							d3d12Device.CreateRenderTargetView(d3d12Resource, &d3d12RenderTargetViewDesc, mD3D12DescriptorHeapDepthStencilView->GetCPUDescriptorHandleForHeapStart());
						}
						break;
					}

				case Rhi::ResourceType::TEXTURE_2D_ARRAY:
					{
						// Update the framebuffer width and height if required
						const Texture2DArray* texture2DArray = static_cast<Texture2DArray*>(mDepthStencilTexture);
						::detail::updateWidthHeight(depthStencilFramebufferAttachment->mipmapIndex, texture2DArray->getWidth(), texture2DArray->getHeight(), mWidth, mHeight);

						// Get the Direct3D 12 resource
						ID3D12Resource* d3d12Resource = texture2DArray->getD3D12Resource();

						// Create the Direct3D 12 render target view instance
						D3D12_DESCRIPTOR_HEAP_DESC d3d12DescriptorHeapDesc = {};
						d3d12DescriptorHeapDesc.NumDescriptors = 1;
						d3d12DescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
						if ( SUCCEEDED(d3d12Device.CreateDescriptorHeap(&d3d12DescriptorHeapDesc, IID_PPV_ARGS(&mD3D12DescriptorHeapDepthStencilView))) )
						{
							D3D12_RENDER_TARGET_VIEW_DESC d3d12RenderTargetViewDesc = {};
							d3d12RenderTargetViewDesc.Format = static_cast<DXGI_FORMAT>(texture2DArray->getDxgiFormat());
							d3d12RenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
							d3d12RenderTargetViewDesc.Texture2DArray.MipSlice = depthStencilFramebufferAttachment->mipmapIndex;
							d3d12RenderTargetViewDesc.Texture2DArray.FirstArraySlice = depthStencilFramebufferAttachment->layerIndex;
							d3d12RenderTargetViewDesc.Texture2DArray.ArraySize = 1;
							d3d12RenderTargetViewDesc.Texture2DArray.PlaneSlice = 0;
							d3d12Device.CreateRenderTargetView(d3d12Resource, &d3d12RenderTargetViewDesc, mD3D12DescriptorHeapDepthStencilView->GetCPUDescriptorHandleForHeapStart());
						}
						break;
					}

				case Rhi::ResourceType::ROOT_SIGNATURE:
				case Rhi::ResourceType::RESOURCE_GROUP:
				case Rhi::ResourceType::GRAPHICS_PROGRAM:
				case Rhi::ResourceType::VERTEX_ARRAY:
				case Rhi::ResourceType::RENDER_PASS:
				case Rhi::ResourceType::QUERY_POOL:
				case Rhi::ResourceType::SWAP_CHAIN:
				case Rhi::ResourceType::FRAMEBUFFER:
				case Rhi::ResourceType::VERTEX_BUFFER:
				case Rhi::ResourceType::INDEX_BUFFER:
				case Rhi::ResourceType::TEXTURE_BUFFER:
				case Rhi::ResourceType::STRUCTURED_BUFFER:
				case Rhi::ResourceType::INDIRECT_BUFFER:
				case Rhi::ResourceType::UNIFORM_BUFFER:
				case Rhi::ResourceType::TEXTURE_1D:
				case Rhi::ResourceType::TEXTURE_1D_ARRAY:
				case Rhi::ResourceType::TEXTURE_3D:
				case Rhi::ResourceType::TEXTURE_CUBE:
				case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
				case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
				case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
				case Rhi::ResourceType::SAMPLER_STATE:
				case Rhi::ResourceType::VERTEX_SHADER:
				case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
				case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
				case Rhi::ResourceType::GEOMETRY_SHADER:
				case Rhi::ResourceType::FRAGMENT_SHADER:
				case Rhi::ResourceType::TASK_SHADER:
				case Rhi::ResourceType::MESH_SHADER:
				case Rhi::ResourceType::COMPUTE_SHADER:
				default:
					RHI_LOG(CRITICAL, "The type of the given depth stencil texture is not supported by the Direct3D 12 RHI implementation")
						break;
				}
			}

			// Validate the framebuffer width and height
			if ( 0 == mWidth || UINT_MAX == mWidth )
			{
				SE_ASSERT(false, "Invalid Direct3D 12 framebuffer width")
					mWidth = 1;
			}
			if ( 0 == mHeight || UINT_MAX == mHeight )
			{
				SE_ASSERT(false, "Invalid Direct3D 12 framebuffer height")
					mHeight = 1;
			}

			// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
			RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "FBO", 6)	// 6 = "FBO: " including terminating zero
				const size_t detailedDebugNameLength = strlen(detailedDebugName);

			{ // Assign a debug name to the Direct3D 12 render target view, do also add the index to the name
				const size_t adjustedDetailedDebugNameLength = detailedDebugNameLength + 5;	// Direct3D 12 supports 8 render targets ("D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT", so: One digit + one [ + one ] + one space + terminating zero = 5 characters)
				char* nameWithIndex = RHI_MALLOC_TYPED(char, detailedDebugNameLength);
				ID3D12DescriptorHeap** d3d12DescriptorHeapRenderTargetViewsEnd = mD3D12DescriptorHeapRenderTargetViews + mNumberOfColorTextures;
				for ( ID3D12DescriptorHeap** d3d12DescriptorHeapRenderTargetView = mD3D12DescriptorHeapRenderTargetViews; d3d12DescriptorHeapRenderTargetView < d3d12DescriptorHeapRenderTargetViewsEnd; ++d3d12DescriptorHeapRenderTargetView )
				{
					sprintf_s(nameWithIndex, detailedDebugNameLength, "%s [%u]", detailedDebugName, static_cast<uint32_t>(d3d12DescriptorHeapRenderTargetView - mD3D12DescriptorHeapRenderTargetViews));
					FAILED_DEBUG_BREAK((*d3d12DescriptorHeapRenderTargetView)->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(adjustedDetailedDebugNameLength), nameWithIndex))
				}
				RHI_FREE(nameWithIndex);
			}

			// Assign a debug name to the Direct3D 12 depth stencil view
			if ( nullptr != mD3D12DescriptorHeapDepthStencilView )
			{
				FAILED_DEBUG_BREAK(mD3D12DescriptorHeapDepthStencilView->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(detailedDebugNameLength), detailedDebugName))
			}
#endif
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Framebuffer() override
		{
			// Release the reference to the used color textures
			if ( nullptr != mD3D12DescriptorHeapRenderTargetViews )
			{
				// Release references
				ID3D12DescriptorHeap** d3d12DescriptorHeapRenderTargetViewsEnd = mD3D12DescriptorHeapRenderTargetViews + mNumberOfColorTextures;
				for ( ID3D12DescriptorHeap** d3d12DescriptorHeapRenderTargetView = mD3D12DescriptorHeapRenderTargetViews; d3d12DescriptorHeapRenderTargetView < d3d12DescriptorHeapRenderTargetViewsEnd; ++d3d12DescriptorHeapRenderTargetView )
				{
					(*d3d12DescriptorHeapRenderTargetView)->Release();
				}

				// Cleanup
				RHI_FREE(mD3D12DescriptorHeapRenderTargetViews);
			}
			if ( nullptr != mColorTextures )
			{
				// Release references
				Rhi::ITexture** colorTexturesEnd = mColorTextures + mNumberOfColorTextures;
				for ( Rhi::ITexture** colorTexture = mColorTextures; colorTexture < colorTexturesEnd; ++colorTexture )
				{
					(*colorTexture)->ReleaseReference();
				}

				// Cleanup
				RHI_FREE(mColorTextures);
			}

			// Release the reference to the used depth stencil texture
			if ( nullptr != mD3D12DescriptorHeapDepthStencilView )
			{
				// Release reference
				mD3D12DescriptorHeapDepthStencilView->Release();
			}
			if ( nullptr != mDepthStencilTexture )
			{
				// Release reference
				mDepthStencilTexture->ReleaseReference();
			}
		}

		/**
		*  @brief
		*    Return the number of color textures
		*
		*  @return
		*    The number of color textures
		*/
		[[nodiscard]] inline uint32_t getNumberOfColorTextures() const
		{
			return mNumberOfColorTextures;
		}

		/**
		*  @brief
		*    Return the color textures
		*
		*  @return
		*    The color textures, null pointer on error, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline Rhi::ITexture** getColorTextures() const
		{
			return mColorTextures;
		}

		/**
		*  @brief
		*    Return the depth stencil texture
		*
		*  @return
		*    The depth stencil texture, null pointer on error, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline Rhi::ITexture* getDepthStencilTexture() const
		{
			return mDepthStencilTexture;
		}

		/**
		*  @brief
		*    Return the Direct3D 12 render target view descriptor heap instance
		*
		*  @return
		*    The Direct3D 12 render target view descriptor heap instance, null pointer on error, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3D12DescriptorHeap** getD3D12DescriptorHeapRenderTargetViews() const
		{
			return mD3D12DescriptorHeapRenderTargetViews;
		}

		/**
		*  @brief
		*    Return the Direct3D 12 depth stencil view descriptor heap instance
		*
		*  @return
		*    The Direct3D 12 depth stencil view descriptor heap instance, null pointer on error, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3D12DescriptorHeap* getD3D12DescriptorHeapDepthStencilView() const
		{
			return mD3D12DescriptorHeapDepthStencilView;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IRenderTarget methods             ]
		//[-------------------------------------------------------]
	public:
		inline virtual void getWidthAndHeight(uint32_t& width, uint32_t& height) const override
		{
			// No fancy implementation in here, just copy over the internal information
			width = mWidth;
			height = mHeight;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(Framebuffer, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Framebuffer(const Framebuffer&) = delete;
		Framebuffer& operator =(const Framebuffer&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		// Generic part
		uint32_t		mNumberOfColorTextures;	// Number of color render target textures
		Rhi::ITexture** mColorTextures;			// The color render target textures (we keep a reference to it), can be a null pointer or can contain null pointers, if not a null pointer there must be at least "m_nNumberOfColorTextures" textures in the provided C-array of pointers
		Rhi::ITexture*  mDepthStencilTexture;	// The depth stencil render target texture (we keep a reference to it), can be a null pointer
		uint32_t		mWidth;					// The framebuffer width
		uint32_t		mHeight;				// The framebuffer height
		// Direct3D 12 part
		ID3D12DescriptorHeap** mD3D12DescriptorHeapRenderTargetViews;	// The Direct3D 12 render target view descriptor heap instance, null pointer on error
		ID3D12DescriptorHeap*  mD3D12DescriptorHeapDepthStencilView;	// The Direct3D 12 depth stencil view descriptor heap instance, null pointer on error


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Shader/VertexShaderHlsl.h               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    HLSL vertex shader class
	*/
	class VertexShaderHlsl final : public Rhi::IVertexShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a vertex shader from shader bytecode
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*/
		VertexShaderHlsl(Direct3D12Rhi& direct3D12Rhi, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IVertexShader(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mD3DBlobVertexShader(nullptr)
		{
			// Backup the vertex shader bytecode
			FAILED_DEBUG_BREAK(D3DCreateBlob(shaderBytecode.getNumberOfBytes(), &mD3DBlobVertexShader))
				memcpy(mD3DBlobVertexShader->GetBufferPointer(), shaderBytecode.getBytecode(), shaderBytecode.getNumberOfBytes());
		}

		/**
		*  @brief
		*    Constructor for creating a vertex shader from shader source code
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		VertexShaderHlsl(Direct3D12Rhi& direct3D12Rhi, const char* sourceCode, Rhi::IShaderLanguage::OptimizationLevel optimizationLevel, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IVertexShader(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mD3DBlobVertexShader(nullptr)
		{
			// Create the Direct3D 12 binary large object for the vertex shader
			mD3DBlobVertexShader = loadShaderFromSourcecode("vs_5_0", sourceCode, nullptr, optimizationLevel);

			// Return shader bytecode, if requested do to so
			if ( nullptr != shaderBytecode )
			{
				shaderBytecode->setBytecodeCopy(static_cast<uint32_t>(mD3DBlobVertexShader->GetBufferSize()), static_cast<uint8_t*>(mD3DBlobVertexShader->GetBufferPointer()));
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~VertexShaderHlsl() override
		{
			// Release the Direct3D 12 shader binary large object
			if ( nullptr != mD3DBlobVertexShader )
			{
				mD3DBlobVertexShader->Release();
			}
		}

		/**
		*  @brief
		*    Return the Direct3D 12 vertex shader blob
		*
		*  @return
		*    Direct3D 12 vertex shader blob, can be a null pointer on error, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3DBlob* getD3DBlobVertexShader() const
		{
			return mD3DBlobVertexShader;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::HLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(VertexShaderHlsl, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit VertexShaderHlsl(const VertexShaderHlsl&) = delete;
		VertexShaderHlsl& operator =(const VertexShaderHlsl&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		ID3DBlob* mD3DBlobVertexShader;	// Direct3D 12 vertex shader blob, can be a null pointer


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Shader/TessellationControlShaderHlsl.h  ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    HLSL tessellation control shader ("hull shader" in Direct3D terminology) class
	*/
	class TessellationControlShaderHlsl final : public Rhi::ITessellationControlShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a tessellation control shader from shader bytecode
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*/
		TessellationControlShaderHlsl(Direct3D12Rhi& direct3D12Rhi, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITessellationControlShader(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mD3DBlobHullShader(nullptr)
		{
			// Backup the hull shader bytecode
			FAILED_DEBUG_BREAK(D3DCreateBlob(shaderBytecode.getNumberOfBytes(), &mD3DBlobHullShader))
				memcpy(mD3DBlobHullShader->GetBufferPointer(), shaderBytecode.getBytecode(), shaderBytecode.getNumberOfBytes());
		}

		/**
		*  @brief
		*    Constructor for creating a tessellation control shader from shader source code
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		TessellationControlShaderHlsl(Direct3D12Rhi& direct3D12Rhi, const char* sourceCode, Rhi::IShaderLanguage::OptimizationLevel optimizationLevel, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITessellationControlShader(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mD3DBlobHullShader(nullptr)
		{
			// Create the Direct3D 12 binary large object for the hull shader
			mD3DBlobHullShader = loadShaderFromSourcecode("hs_5_0", sourceCode, nullptr, optimizationLevel);

			// Return shader bytecode, if requested do to so
			if ( nullptr != shaderBytecode )
			{
				shaderBytecode->setBytecodeCopy(static_cast<uint32_t>(mD3DBlobHullShader->GetBufferSize()), static_cast<uint8_t*>(mD3DBlobHullShader->GetBufferPointer()));
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TessellationControlShaderHlsl() override
		{
			// Release the Direct3D 12 shader binary large object
			if ( nullptr != mD3DBlobHullShader )
			{
				mD3DBlobHullShader->Release();
			}
		}

		/**
		*  @brief
		*    Return the Direct3D 12 hull shader blob
		*
		*  @return
		*    Direct3D 12 hull shader blob, can be a null pointer on error, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3DBlob* getD3DBlobHullShader() const
		{
			return mD3DBlobHullShader;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::HLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(TessellationControlShaderHlsl, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TessellationControlShaderHlsl(const TessellationControlShaderHlsl&) = delete;
		TessellationControlShaderHlsl& operator =(const TessellationControlShaderHlsl&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		ID3DBlob* mD3DBlobHullShader;	// Direct3D 12 hull shader blob, can be a null pointer


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Shader/TessellationEvaluationShaderHlsl.h ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    HLSL tessellation evaluation shader ("domain shader" in Direct3D terminology) class
	*/
	class TessellationEvaluationShaderHlsl final : public Rhi::ITessellationEvaluationShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a tessellation evaluation shader from shader bytecode
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*/
		TessellationEvaluationShaderHlsl(Direct3D12Rhi& direct3D12Rhi, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITessellationEvaluationShader(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mD3DBlobDomainShader(nullptr)
		{
			// Backup the domain shader bytecode
			FAILED_DEBUG_BREAK(D3DCreateBlob(shaderBytecode.getNumberOfBytes(), &mD3DBlobDomainShader))
				memcpy(mD3DBlobDomainShader->GetBufferPointer(), shaderBytecode.getBytecode(), shaderBytecode.getNumberOfBytes());
		}

		/**
		*  @brief
		*    Constructor for creating a tessellation evaluation shader from shader source code
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		TessellationEvaluationShaderHlsl(Direct3D12Rhi& direct3D12Rhi, const char* sourceCode, Rhi::IShaderLanguage::OptimizationLevel optimizationLevel, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITessellationEvaluationShader(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mD3DBlobDomainShader(nullptr)
		{
			// Create the Direct3D 12 binary large object for the domain shader
			mD3DBlobDomainShader = loadShaderFromSourcecode("ds_5_0", sourceCode, nullptr, optimizationLevel);

			// Return shader bytecode, if requested do to so
			if ( nullptr != shaderBytecode )
			{
				shaderBytecode->setBytecodeCopy(static_cast<uint32_t>(mD3DBlobDomainShader->GetBufferSize()), static_cast<uint8_t*>(mD3DBlobDomainShader->GetBufferPointer()));
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TessellationEvaluationShaderHlsl() override
		{
			// Release the Direct3D 12 shader binary large object
			if ( nullptr != mD3DBlobDomainShader )
			{
				mD3DBlobDomainShader->Release();
			}
		}

		/**
		*  @brief
		*    Return the Direct3D 12 domain shader blob
		*
		*  @return
		*    Direct3D 12 domain shader blob, can be a null pointer on error, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3DBlob* getD3DBlobDomainShader() const
		{
			return mD3DBlobDomainShader;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::HLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(TessellationEvaluationShaderHlsl, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TessellationEvaluationShaderHlsl(const TessellationEvaluationShaderHlsl&) = delete;
		TessellationEvaluationShaderHlsl& operator =(const TessellationEvaluationShaderHlsl&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		ID3DBlob* mD3DBlobDomainShader;	// Direct3D 12 domain shader blob, can be a null pointer


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Shader/GeometryShaderHlsl.h             ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    HLSL geometry shader class
	*/
	class GeometryShaderHlsl final : public Rhi::IGeometryShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a geometry shader from shader bytecode
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*/
		GeometryShaderHlsl(Direct3D12Rhi& direct3D12Rhi, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IGeometryShader(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mD3DBlobGeometryShader(nullptr)
		{
			// Backup the geometry shader bytecode
			FAILED_DEBUG_BREAK(D3DCreateBlob(shaderBytecode.getNumberOfBytes(), &mD3DBlobGeometryShader))
				memcpy(mD3DBlobGeometryShader->GetBufferPointer(), shaderBytecode.getBytecode(), shaderBytecode.getNumberOfBytes());
		}

		/**
		*  @brief
		*    Constructor for creating a geometry shader from shader source code
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		GeometryShaderHlsl(Direct3D12Rhi& direct3D12Rhi, const char* sourceCode, Rhi::IShaderLanguage::OptimizationLevel optimizationLevel, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IGeometryShader(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mD3DBlobGeometryShader(nullptr)
		{
			// Create the Direct3D 12 binary large object for the geometry shader
			mD3DBlobGeometryShader = loadShaderFromSourcecode("gs_5_0", sourceCode, nullptr, optimizationLevel);

			// Return shader bytecode, if requested do to so
			if ( nullptr != shaderBytecode )
			{
				shaderBytecode->setBytecodeCopy(static_cast<uint32_t>(mD3DBlobGeometryShader->GetBufferSize()), static_cast<uint8_t*>(mD3DBlobGeometryShader->GetBufferPointer()));
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GeometryShaderHlsl() override
		{
			// Release the Direct3D 12 shader binary large object
			if ( nullptr != mD3DBlobGeometryShader )
			{
				mD3DBlobGeometryShader->Release();
			}
		}

		/**
		*  @brief
		*    Return the Direct3D 12 geometry shader blob
		*
		*  @return
		*    Direct3D 12 geometry shader blob, can be a null pointer on error, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3DBlob* getD3DBlobGeometryShader() const
		{
			return mD3DBlobGeometryShader;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::HLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(GeometryShaderHlsl, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit GeometryShaderHlsl(const GeometryShaderHlsl&) = delete;
		GeometryShaderHlsl& operator =(const GeometryShaderHlsl&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		ID3DBlob* mD3DBlobGeometryShader;	// Direct3D 12 geometry shader blob, can be a null pointer


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Shader/FragmentShaderHlsl.h             ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    HLSL fragment shader class (FS, "pixel shader" in Direct3D terminology)
	*/
	class FragmentShaderHlsl final : public Rhi::IFragmentShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a fragment shader from shader bytecode
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*/
		FragmentShaderHlsl(Direct3D12Rhi& direct3D12Rhi, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IFragmentShader(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mD3DBlobFragmentShader(nullptr)
		{
			// Backup the fragment shader bytecode
			FAILED_DEBUG_BREAK(D3DCreateBlob(shaderBytecode.getNumberOfBytes(), &mD3DBlobFragmentShader))
				memcpy(mD3DBlobFragmentShader->GetBufferPointer(), shaderBytecode.getBytecode(), shaderBytecode.getNumberOfBytes());
		}

		/**
		*  @brief
		*    Constructor for creating a fragment shader from shader source code
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		FragmentShaderHlsl(Direct3D12Rhi& direct3D12Rhi, const char* sourceCode, Rhi::IShaderLanguage::OptimizationLevel optimizationLevel, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IFragmentShader(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mD3DBlobFragmentShader(nullptr)
		{
			// Create the Direct3D 12 binary large object for the fragment shader
			mD3DBlobFragmentShader = loadShaderFromSourcecode("ps_5_0", sourceCode, nullptr, optimizationLevel);

			// Return shader bytecode, if requested do to so
			if ( nullptr != shaderBytecode )
			{
				shaderBytecode->setBytecodeCopy(static_cast<uint32_t>(mD3DBlobFragmentShader->GetBufferSize()), static_cast<uint8_t*>(mD3DBlobFragmentShader->GetBufferPointer()));
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FragmentShaderHlsl() override
		{
			// Release the Direct3D 12 shader binary large object
			if ( nullptr != mD3DBlobFragmentShader )
			{
				mD3DBlobFragmentShader->Release();
			}
		}

		/**
		*  @brief
		*    Return the Direct3D 12 fragment shader blob
		*
		*  @return
		*    Direct3D 12 fragment shader blob, can be a null pointer on error, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3DBlob* getD3DBlobFragmentShader() const
		{
			return mD3DBlobFragmentShader;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::HLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(FragmentShaderHlsl, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit FragmentShaderHlsl(const FragmentShaderHlsl&) = delete;
		FragmentShaderHlsl& operator =(const FragmentShaderHlsl&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		ID3DBlob* mD3DBlobFragmentShader;	// Direct3D 12 mesh shader blob, can be a null pointer


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Shader/TaskShaderHlsl.h                 ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    HLSL task shader class (TS, "amplification shader" in Direct3D terminology)
	*/
	class TaskShaderHlsl final : public Rhi::ITaskShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a task shader from shader bytecode
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*/
		TaskShaderHlsl(Direct3D12Rhi& direct3D12Rhi, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITaskShader(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mD3DBlobTaskShader(nullptr)
		{
			// Backup the task shader bytecode
			FAILED_DEBUG_BREAK(D3DCreateBlob(shaderBytecode.getNumberOfBytes(), &mD3DBlobTaskShader))
				memcpy(mD3DBlobTaskShader->GetBufferPointer(), shaderBytecode.getBytecode(), shaderBytecode.getNumberOfBytes());
		}

		/**
		*  @brief
		*    Constructor for creating a task shader from shader source code
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		TaskShaderHlsl(Direct3D12Rhi& direct3D12Rhi, const char* sourceCode, Rhi::IShaderLanguage::OptimizationLevel optimizationLevel, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITaskShader(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mD3DBlobTaskShader(nullptr)
		{
			// Create the Direct3D 12 binary large object for the task shader
			mD3DBlobTaskShader = loadShaderFromSourcecode("ps_5_0", sourceCode, nullptr, optimizationLevel);

			// Return shader bytecode, if requested do to so
			if ( nullptr != shaderBytecode )
			{
				shaderBytecode->setBytecodeCopy(static_cast<uint32_t>(mD3DBlobTaskShader->GetBufferSize()), static_cast<uint8_t*>(mD3DBlobTaskShader->GetBufferPointer()));
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TaskShaderHlsl() override
		{
			// Release the Direct3D 12 shader binary large object
			if ( nullptr != mD3DBlobTaskShader )
			{
				mD3DBlobTaskShader->Release();
			}
		}

		/**
		*  @brief
		*    Return the Direct3D 12 task shader blob
		*
		*  @return
		*    Direct3D 12 task shader blob, can be a null pointer on error, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3DBlob* getD3DBlobTaskShader() const
		{
			return mD3DBlobTaskShader;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::HLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(TaskShaderHlsl, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit TaskShaderHlsl(const TaskShaderHlsl&) = delete;
		TaskShaderHlsl& operator =(const TaskShaderHlsl&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		ID3DBlob* mD3DBlobTaskShader;	// Direct3D 12 task shader blob, can be a null pointer


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Shader/MeshShaderHlsl.h                 ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    HLSL mesh shader class (MS)
	*/
	class MeshShaderHlsl final : public Rhi::IMeshShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a mesh shader from shader bytecode
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*/
		MeshShaderHlsl(Direct3D12Rhi& direct3D12Rhi, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IMeshShader(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mD3DBlobMeshShader(nullptr)
		{
			// Backup the mesh shader bytecode
			FAILED_DEBUG_BREAK(D3DCreateBlob(shaderBytecode.getNumberOfBytes(), &mD3DBlobMeshShader))
				memcpy(mD3DBlobMeshShader->GetBufferPointer(), shaderBytecode.getBytecode(), shaderBytecode.getNumberOfBytes());
		}

		/**
		*  @brief
		*    Constructor for creating a mesh shader from shader source code
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		MeshShaderHlsl(Direct3D12Rhi& direct3D12Rhi, const char* sourceCode, Rhi::IShaderLanguage::OptimizationLevel optimizationLevel, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IMeshShader(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mD3DBlobMeshShader(nullptr)
		{
			// Create the Direct3D 12 binary large object for the mesh shader
			mD3DBlobMeshShader = loadShaderFromSourcecode("ps_5_0", sourceCode, nullptr, optimizationLevel);

			// Return shader bytecode, if requested do to so
			if ( nullptr != shaderBytecode )
			{
				shaderBytecode->setBytecodeCopy(static_cast<uint32_t>(mD3DBlobMeshShader->GetBufferSize()), static_cast<uint8_t*>(mD3DBlobMeshShader->GetBufferPointer()));
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MeshShaderHlsl() override
		{
			// Release the Direct3D 12 shader binary large object
			if ( nullptr != mD3DBlobMeshShader )
			{
				mD3DBlobMeshShader->Release();
			}
		}

		/**
		*  @brief
		*    Return the Direct3D 12 mesh shader blob
		*
		*  @return
		*    Direct3D 12 mesh shader blob, can be a null pointer on error, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3DBlob* getD3DBlobMeshShader() const
		{
			return mD3DBlobMeshShader;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::HLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(MeshShaderHlsl, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit MeshShaderHlsl(const MeshShaderHlsl&) = delete;
		MeshShaderHlsl& operator =(const MeshShaderHlsl&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		ID3DBlob* mD3DBlobMeshShader;	// Direct3D 12 mesh shader blob, can be a null pointer


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Shader/ComputeShaderHlsl.h              ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    HLSL compute shader class (CS)
	*/
	class ComputeShaderHlsl final : public Rhi::IComputeShader
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for creating a compute shader from shader bytecode
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*/
		ComputeShaderHlsl(Direct3D12Rhi& direct3D12Rhi, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IComputeShader(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mD3DBlobComputeShader(nullptr)
		{
			// Backup the compute shader bytecode
			FAILED_DEBUG_BREAK(D3DCreateBlob(shaderBytecode.getNumberOfBytes(), &mD3DBlobComputeShader))
				memcpy(mD3DBlobComputeShader->GetBufferPointer(), shaderBytecode.getBytecode(), shaderBytecode.getNumberOfBytes());
		}

		/**
		*  @brief
		*    Constructor for creating a compute shader from shader source code
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] sourceCode
		*    Shader ASCII source code, must be valid
		*/
		ComputeShaderHlsl(Direct3D12Rhi& direct3D12Rhi, const char* sourceCode, Rhi::IShaderLanguage::OptimizationLevel optimizationLevel, Rhi::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IComputeShader(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mD3DBlobComputeShader(nullptr)
		{
			// Create the Direct3D 12 binary large object for the compute shader
			mD3DBlobComputeShader = loadShaderFromSourcecode("cs_5_0", sourceCode, nullptr, optimizationLevel);

			// Return shader bytecode, if requested do to so
			if ( nullptr != shaderBytecode )
			{
				shaderBytecode->setBytecodeCopy(static_cast<uint32_t>(mD3DBlobComputeShader->GetBufferSize()), static_cast<uint8_t*>(mD3DBlobComputeShader->GetBufferPointer()));
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ComputeShaderHlsl() override
		{
			// Release the Direct3D 12 shader binary large object
			if ( nullptr != mD3DBlobComputeShader )
			{
				mD3DBlobComputeShader->Release();
			}
		}

		/**
		*  @brief
		*    Return the Direct3D 12 compute shader blob
		*
		*  @return
		*    Direct3D 12 compute shader blob, can be a null pointer on error, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3DBlob* getD3DBlobComputeShader() const
		{
			return mD3DBlobComputeShader;
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShader methods                   ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::HLSL_NAME;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(ComputeShaderHlsl, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit ComputeShaderHlsl(const ComputeShaderHlsl&) = delete;
		ComputeShaderHlsl& operator =(const ComputeShaderHlsl&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		ID3DBlob* mD3DBlobComputeShader;	// Direct3D 12 compute shader blob, can be a null pointer


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Shader/GraphicsProgramHlsl.h            ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    HLSL graphics program class
	*/
	class GraphicsProgramHlsl final : public Rhi::IGraphicsProgram
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for traditional graphics program
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] vertexShaderHlsl
		*    Vertex shader the graphics program is using, can be a null pointer
		*  @param[in] tessellationControlShaderHlsl
		*    Tessellation control shader the graphics program is using, can be a null pointer
		*  @param[in] tessellationEvaluationShaderHlsl
		*    Tessellation evaluation shader the graphics program is using, can be a null pointer
		*  @param[in] geometryShaderHlsl
		*    Geometry shader the graphics program is using, can be a null pointer
		*  @param[in] fragmentShaderHlsl
		*    Fragment shader the graphics program is using, can be a null pointer
		*
		*  @note
		*    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
		*/
		GraphicsProgramHlsl(Direct3D12Rhi& direct3D12Rhi, VertexShaderHlsl* vertexShaderHlsl, TessellationControlShaderHlsl* tessellationControlShaderHlsl, TessellationEvaluationShaderHlsl* tessellationEvaluationShaderHlsl, GeometryShaderHlsl* geometryShaderHlsl, FragmentShaderHlsl* fragmentShaderHlsl RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IGraphicsProgram(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVertexShaderHlsl(vertexShaderHlsl),
			mTessellationControlShaderHlsl(tessellationControlShaderHlsl),
			mTessellationEvaluationShaderHlsl(tessellationEvaluationShaderHlsl),
			mGeometryShaderHlsl(geometryShaderHlsl),
			mFragmentShaderHlsl(fragmentShaderHlsl)
		{
			// Add references to the provided shaders
			if ( nullptr != mVertexShaderHlsl )
			{
				mVertexShaderHlsl->AddReference();
			}
			if ( nullptr != mTessellationControlShaderHlsl )
			{
				mTessellationControlShaderHlsl->AddReference();
			}
			if ( nullptr != mTessellationEvaluationShaderHlsl )
			{
				mTessellationEvaluationShaderHlsl->AddReference();
			}
			if ( nullptr != mGeometryShaderHlsl )
			{
				mGeometryShaderHlsl->AddReference();
			}
			if ( nullptr != mFragmentShaderHlsl )
			{
				mFragmentShaderHlsl->AddReference();
			}
		}

		/**
		*  @brief
		*    Constructor for task and mesh shader based graphics program
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] taskShaderHlsl
		*    Task shader the graphics program is using, can be a null pointer
		*  @param[in] meshShaderHlsl
		*    Mesh shader the graphics program is using
		*  @param[in] fragmentShaderHlsl
		*    Fragment shader the graphics program is using, can be a null pointer
		*
		*  @note
		*    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
		*/
		GraphicsProgramHlsl(Direct3D12Rhi& direct3D12Rhi, TaskShaderHlsl* taskShaderHlsl, MeshShaderHlsl& meshShaderHlsl, FragmentShaderHlsl* fragmentShaderHlsl RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IGraphicsProgram(direct3D12Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mVertexShaderHlsl(nullptr),
			mTessellationControlShaderHlsl(nullptr),
			mTessellationEvaluationShaderHlsl(nullptr),
			mGeometryShaderHlsl(nullptr),
			mFragmentShaderHlsl(fragmentShaderHlsl),
			mTaskShaderHlsl(taskShaderHlsl),
			mMeshShaderHlsl(&meshShaderHlsl)
		{
			// Add references to the provided shaders
			if ( nullptr != mFragmentShaderHlsl )
			{
				mFragmentShaderHlsl->AddReference();
			}
			if ( nullptr != mTaskShaderHlsl )
			{
				mTaskShaderHlsl->AddReference();
			}
			mMeshShaderHlsl->AddReference();
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GraphicsProgramHlsl() override
		{
			// Release the shader references
			if ( nullptr != mVertexShaderHlsl )
			{
				mVertexShaderHlsl->ReleaseReference();
			}
			if ( nullptr != mTessellationControlShaderHlsl )
			{
				mTessellationControlShaderHlsl->ReleaseReference();
			}
			if ( nullptr != mTessellationEvaluationShaderHlsl )
			{
				mTessellationEvaluationShaderHlsl->ReleaseReference();
			}
			if ( nullptr != mGeometryShaderHlsl )
			{
				mGeometryShaderHlsl->ReleaseReference();
			}
			if ( nullptr != mFragmentShaderHlsl )
			{
				mFragmentShaderHlsl->ReleaseReference();
			}
			if ( nullptr != mTaskShaderHlsl )
			{
				mTaskShaderHlsl->ReleaseReference();
			}
			if ( nullptr != mMeshShaderHlsl )
			{
				mMeshShaderHlsl->ReleaseReference();
			}
		}


		//[-------------------------------------------------------]
		//[ Traditional graphics program                          ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the HLSL vertex shader the graphics program is using
		*
		*  @return
		*    The HLSL vertex shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline VertexShaderHlsl* getVertexShaderHlsl() const
		{
			return mVertexShaderHlsl;
		}

		/**
		*  @brief
		*    Return the HLSL tessellation control shader the graphics program is using
		*
		*  @return
		*    The HLSL tessellation control shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline TessellationControlShaderHlsl* getTessellationControlShaderHlsl() const
		{
			return mTessellationControlShaderHlsl;
		}

		/**
		*  @brief
		*    Return the HLSL tessellation evaluation shader the graphics program is using
		*
		*  @return
		*    The HLSL tessellation evaluation shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline TessellationEvaluationShaderHlsl* getTessellationEvaluationShaderHlsl() const
		{
			return mTessellationEvaluationShaderHlsl;
		}

		/**
		*  @brief
		*    Return the HLSL geometry shader the graphics program is using
		*
		*  @return
		*    The HLSL geometry shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline GeometryShaderHlsl* getGeometryShaderHlsl() const
		{
			return mGeometryShaderHlsl;
		}

		//[-------------------------------------------------------]
		//[ Both graphics programs                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the HLSL fragment shader the graphics program is using
		*
		*  @return
		*    The HLSL fragment shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline FragmentShaderHlsl* getFragmentShaderHlsl() const
		{
			return mFragmentShaderHlsl;
		}

		//[-------------------------------------------------------]
		//[ Task and mesh shader based graphics program           ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the HLSL task shader the graphics program is using
		*
		*  @return
		*    The HLSL task shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline TaskShaderHlsl* getTaskShaderHlsl() const
		{
			return mTaskShaderHlsl;
		}

		/**
		*  @brief
		*    Return the HLSL mesh shader the graphics program is using
		*
		*  @return
		*    The HLSL mesh shader the graphics program is using, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline MeshShaderHlsl* getMeshShaderHlsl() const
		{
			return mMeshShaderHlsl;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(GraphicsProgramHlsl, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit GraphicsProgramHlsl(const GraphicsProgramHlsl&) = delete;
		GraphicsProgramHlsl& operator =(const GraphicsProgramHlsl&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		// Traditional graphics program
		VertexShaderHlsl*				  mVertexShaderHlsl = nullptr;					// Vertex shader the graphics program is using (we keep a reference to it), can be a null pointer
		TessellationControlShaderHlsl*	  mTessellationControlShaderHlsl = nullptr;		// Tessellation control shader the graphics program is using (we keep a reference to it), can be a null pointer
		TessellationEvaluationShaderHlsl* mTessellationEvaluationShaderHlsl = nullptr;	// Tessellation evaluation shader the graphics program is using (we keep a reference to it), can be a null pointer
		GeometryShaderHlsl*				  mGeometryShaderHlsl = nullptr;					// Geometry shader the graphics program is using (we keep a reference to it), can be a null pointer
		// Both graphics programs
		FragmentShaderHlsl* mFragmentShaderHlsl = nullptr;	// Fragment shader the graphics program is using (we keep a reference to it), can be a null pointer
		// Task and mesh shader based graphics program
		TaskShaderHlsl* mTaskShaderHlsl = nullptr;	// Task shader the graphics program is using (we keep a reference to it), can be a null pointer
		MeshShaderHlsl* mMeshShaderHlsl = nullptr;	// Mesh shader the graphics program is using (we keep a reference to it), can be a null pointer


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/Shader/ShaderLanguageHlsl.h             ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    HLSL shader language class
	*/
	class ShaderLanguageHlsl final : public Rhi::IShaderLanguage
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*/
		inline explicit ShaderLanguageHlsl(Direct3D12Rhi& direct3D12Rhi) :
			IShaderLanguage(direct3D12Rhi)
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ShaderLanguageHlsl() override
		{
		}


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IShaderLanguage methods           ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual const char* getShaderLanguageName() const override
		{
			return ::detail::HLSL_NAME;
		}

		[[nodiscard]] inline virtual Rhi::IVertexShader* createVertexShaderFromBytecode([[maybe_unused]] const Rhi::VertexAttributes& vertexAttributes, const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());

			// Sanity check
			SE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Direct3D 12 vertex shader bytecode is invalid")

				// There's no need to check for "Rhi::Capabilities::vertexShader", we know there's vertex shader support
				return RHI_NEW(VertexShaderHlsl)(direct3D12Rhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IVertexShader* createVertexShaderFromSourceCode([[maybe_unused]] const Rhi::VertexAttributes& vertexAttributes, const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// There's no need to check for "Rhi::Capabilities::vertexShader", we know there's vertex shader support
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());
			return RHI_NEW(VertexShaderHlsl)(direct3D12Rhi, shaderSourceCode.sourceCode, getOptimizationLevel(), shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::ITessellationControlShader* createTessellationControlShaderFromBytecode(const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// "hull shader" in Direct3D terminology
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());

			// Sanity check
			SE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Direct3D 12 tessellation control shader bytecode is invalid")

				// There's no need to check for "Rhi::Capabilities::maximumNumberOfPatchVertices", we know there's tessellation control shader support
				return RHI_NEW(TessellationControlShaderHlsl)(direct3D12Rhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::ITessellationControlShader* createTessellationControlShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// "hull shader" in Direct3D terminology

			// There's no need to check for "Rhi::Capabilities::maximumNumberOfPatchVertices", we know there's tessellation control shader support
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());
			return RHI_NEW(TessellationControlShaderHlsl)(direct3D12Rhi, shaderSourceCode.sourceCode, getOptimizationLevel(), shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::ITessellationEvaluationShader* createTessellationEvaluationShaderFromBytecode(const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// "domain shader" in Direct3D terminology
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());

			// Sanity check
			SE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Direct3D 12 tessellation evaluation shader bytecode is invalid")

				// There's no need to check for "Rhi::Capabilities::maximumNumberOfPatchVertices", we know there's tessellation evaluation shader support
				return RHI_NEW(TessellationEvaluationShaderHlsl)(direct3D12Rhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::ITessellationEvaluationShader* createTessellationEvaluationShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// "domain shader" in Direct3D terminology

			// There's no need to check for "Rhi::Capabilities::maximumNumberOfPatchVertices", we know there's tessellation evaluation shader support
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());
			return RHI_NEW(TessellationEvaluationShaderHlsl)(direct3D12Rhi, shaderSourceCode.sourceCode, getOptimizationLevel(), shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IGeometryShader* createGeometryShaderFromBytecode(const Rhi::ShaderBytecode& shaderBytecode, [[maybe_unused]] Rhi::GsInputPrimitiveTopology gsInputPrimitiveTopology, [[maybe_unused]] Rhi::GsOutputPrimitiveTopology gsOutputPrimitiveTopology, [[maybe_unused]] uint32_t numberOfOutputVertices RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());

			// Sanity check
			SE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Direct3D 12 geometry shader bytecode is invalid")

				// There's no need to check for "Rhi::Capabilities::maximumNumberOfGsOutputVertices", we know there's geometry shader support
				// Ignore "gsInputPrimitiveTopology", it's directly set within HLSL
				// Ignore "gsOutputPrimitiveTopology", it's directly set within HLSL
				// Ignore "numberOfOutputVertices", it's directly set within HLSL
				return RHI_NEW(GeometryShaderHlsl)(direct3D12Rhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IGeometryShader* createGeometryShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] Rhi::GsInputPrimitiveTopology gsInputPrimitiveTopology, [[maybe_unused]] Rhi::GsOutputPrimitiveTopology gsOutputPrimitiveTopology, [[maybe_unused]] uint32_t numberOfOutputVertices, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// There's no need to check for "Rhi::Capabilities::maximumNumberOfGsOutputVertices", we know there's geometry shader support
			// Ignore "gsInputPrimitiveTopology", it's directly set within HLSL
			// Ignore "gsOutputPrimitiveTopology", it's directly set within HLSL
			// Ignore "numberOfOutputVertices", it's directly set within HLSL
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());
			return RHI_NEW(GeometryShaderHlsl)(direct3D12Rhi, shaderSourceCode.sourceCode, getOptimizationLevel(), shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IFragmentShader* createFragmentShaderFromBytecode(const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());

			// Sanity check
			SE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Direct3D 12 fragment shader bytecode is invalid")

				// There's no need to check for "Rhi::Capabilities::fragmentShader", we know there's fragment shader support
				return RHI_NEW(FragmentShaderHlsl)(direct3D12Rhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IFragmentShader* createFragmentShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// There's no need to check for "Rhi::Capabilities::fragmentShader", we know there's fragment shader support
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());
			return RHI_NEW(FragmentShaderHlsl)(direct3D12Rhi, shaderSourceCode.sourceCode, getOptimizationLevel(), shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::ITaskShader* createTaskShaderFromBytecode(const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());

			// Sanity checks
			SE_ASSERT(direct3D12Rhi.getCapabilities().meshShader, "Direct3D 12 task shader support is unavailable, DirectX 12 Ultimate needed")
				SE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Direct3D 12 task shader bytecode is invalid")

				// Create the task shader
				return RHI_NEW(TaskShaderHlsl)(direct3D12Rhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::ITaskShader* createTaskShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());

			// Sanity check
			SE_ASSERT(direct3D12Rhi.getCapabilities().meshShader, "Direct3D 12 task shader support is unavailable, DirectX 12 Ultimate needed")

				// Create the task shader
				return RHI_NEW(TaskShaderHlsl)(direct3D12Rhi, shaderSourceCode.sourceCode, getOptimizationLevel(), shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IMeshShader* createMeshShaderFromBytecode(const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());

			// Sanity checks
			SE_ASSERT(direct3D12Rhi.getCapabilities().meshShader, "Direct3D 12 mesh shader support is unavailable, DirectX 12 Ultimate needed")
				SE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Direct3D 12 mesh shader bytecode is invalid")

				// Create the task shader
				return RHI_NEW(MeshShaderHlsl)(direct3D12Rhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IMeshShader* createMeshShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());

			// Sanity checks
			SE_ASSERT(direct3D12Rhi.getCapabilities().meshShader, "Direct3D 12 mesh shader support is unavailable, DirectX 12 Ultimate needed")

				// Create the task shader
				return RHI_NEW(MeshShaderHlsl)(direct3D12Rhi, shaderSourceCode.sourceCode, getOptimizationLevel(), shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IComputeShader* createComputeShaderFromBytecode(const Rhi::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());

			// Sanity check
			SE_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "Direct3D 12 compute shader bytecode is invalid")

				// There's no need to check for "Rhi::Capabilities::computeShader", we know there's compute shader support
				return RHI_NEW(ComputeShaderHlsl)(direct3D12Rhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] inline virtual Rhi::IComputeShader* createComputeShaderFromSourceCode(const Rhi::ShaderSourceCode& shaderSourceCode, Rhi::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			// There's no need to check for "Rhi::Capabilities::computeShader", we know there's compute shader support
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());
			return RHI_NEW(ComputeShaderHlsl)(direct3D12Rhi, shaderSourceCode.sourceCode, getOptimizationLevel(), shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] virtual Rhi::IGraphicsProgram* createGraphicsProgram([[maybe_unused]] const Rhi::IRootSignature& rootSignature, [[maybe_unused]] const Rhi::VertexAttributes& vertexAttributes, Rhi::IVertexShader* vertexShader, Rhi::ITessellationControlShader* tessellationControlShader, Rhi::ITessellationEvaluationShader* tessellationEvaluationShader, Rhi::IGeometryShader* geometryShader, Rhi::IFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());

			// Sanity checks
			// -> A shader can be a null pointer, but if it's not the shader and graphics program language must match
			// -> Optimization: Comparing the shader language name by directly comparing the pointer address of
			//    the name is safe because we know that we always reference to one and the same name address
			// TODO(co) Add security check: Is the given resource one of the currently used RHI?
			SE_ASSERT(nullptr == vertexShader || vertexShader->getShaderLanguageName() == ::detail::HLSL_NAME, "Direct3D 12 vertex shader language mismatch")
				SE_ASSERT(nullptr == tessellationControlShader || tessellationControlShader->getShaderLanguageName() == ::detail::HLSL_NAME, "Direct3D 12 tessellation control shader language mismatch")
				SE_ASSERT(nullptr == tessellationEvaluationShader || tessellationEvaluationShader->getShaderLanguageName() == ::detail::HLSL_NAME, "Direct3D 12 tessellation evaluation shader language mismatch")
				SE_ASSERT(nullptr == geometryShader || geometryShader->getShaderLanguageName() == ::detail::HLSL_NAME, "Direct3D 12 geometry shader language mismatch")
				SE_ASSERT(nullptr == fragmentShader || fragmentShader->getShaderLanguageName() == ::detail::HLSL_NAME, "Direct3D 12 fragment shader language mismatch")

				// Create the graphics program
				return RHI_NEW(GraphicsProgramHlsl)(direct3D12Rhi, static_cast<VertexShaderHlsl*>(vertexShader), static_cast<TessellationControlShaderHlsl*>(tessellationControlShader), static_cast<TessellationEvaluationShaderHlsl*>(tessellationEvaluationShader), static_cast<GeometryShaderHlsl*>(geometryShader), static_cast<FragmentShaderHlsl*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}

		[[nodiscard]] virtual Rhi::IGraphicsProgram* createGraphicsProgram([[maybe_unused]] const Rhi::IRootSignature& rootSignature, Rhi::ITaskShader* taskShader, Rhi::IMeshShader& meshShader, Rhi::IFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER)
		{
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());

			// Sanity checks
			// -> A shader can be a null pointer, but if it's not the shader and graphics program language must match
			// -> Optimization: Comparing the shader language name by directly comparing the pointer address of
			//    the name is safe because we know that we always reference to one and the same name address
			// TODO(co) Add security check: Is the given resource one of the currently used RHI?
			SE_ASSERT(nullptr == taskShader || taskShader->getShaderLanguageName() == ::detail::HLSL_NAME, "Direct3D 12 task shader language mismatch")
				SE_ASSERT(meshShader.getShaderLanguageName() == ::detail::HLSL_NAME, "Direct3D 12 mesh shader language mismatch")
				SE_ASSERT(nullptr == fragmentShader || fragmentShader->getShaderLanguageName() == ::detail::HLSL_NAME, "Direct3D 12 fragment shader language mismatch")

				// Create the graphics program
				return RHI_NEW(GraphicsProgramHlsl)(direct3D12Rhi, static_cast<TaskShaderHlsl*>(taskShader), static_cast<MeshShaderHlsl&>(meshShader), static_cast<FragmentShaderHlsl*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(ShaderLanguageHlsl, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit ShaderLanguageHlsl(const ShaderLanguageHlsl&) = delete;
		ShaderLanguageHlsl& operator =(const ShaderLanguageHlsl&) = delete;


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/State/GraphicsPipelineState.h           ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 graphics pipeline state class
	*/
	class GraphicsPipelineState final : public Rhi::IGraphicsPipelineState
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] graphicsPipelineState
		*    Graphics pipeline state to use
		*  @param[in] id
		*    The unique compact graphics pipeline state ID
		*/
		GraphicsPipelineState(Direct3D12Rhi& direct3D12Rhi, const Rhi::GraphicsPipelineState& graphicsPipelineState, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IGraphicsPipelineState(direct3D12Rhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mD3D12PrimitiveTopology(static_cast<D3D12_PRIMITIVE_TOPOLOGY>(graphicsPipelineState.primitiveTopology)),
			mD3D12GraphicsPipelineState(nullptr),
			mRootSignature(graphicsPipelineState.rootSignature),
			mGraphicsProgram(graphicsPipelineState.graphicsProgram),
			mRenderPass(graphicsPipelineState.renderPass)
		{
			// Add a reference to the referenced RHI resources
			mRootSignature->AddReference();
			mGraphicsProgram->AddReference();
			mRenderPass->AddReference();

			// Define the vertex input layout
			// -> No dynamic allocations/deallocations in here, a fixed maximum number of supported attributes must be sufficient
			static constexpr uint32_t MAXIMUM_NUMBER_OF_ATTRIBUTES = 16;	// 16 elements per vertex are already pretty many
			uint32_t numberOfVertexAttributes = graphicsPipelineState.vertexAttributes.numberOfAttributes;
			if ( numberOfVertexAttributes > MAXIMUM_NUMBER_OF_ATTRIBUTES )
			{
				RHI_LOG(CRITICAL, "Too many vertex attributes (%u) provided. The limit of the Direct3D 12 RHI implementation is %u.", numberOfVertexAttributes, MAXIMUM_NUMBER_OF_ATTRIBUTES)
					numberOfVertexAttributes = MAXIMUM_NUMBER_OF_ATTRIBUTES;
			}
			D3D12_INPUT_ELEMENT_DESC d3d12InputElementDescs[MAXIMUM_NUMBER_OF_ATTRIBUTES];
			for ( uint32_t vertexAttribute = 0; vertexAttribute < numberOfVertexAttributes; ++vertexAttribute )
			{
				const Rhi::VertexAttribute& currentVertexAttribute = graphicsPipelineState.vertexAttributes.attributes[vertexAttribute];
				D3D12_INPUT_ELEMENT_DESC& d3d12InputElementDesc = d3d12InputElementDescs[vertexAttribute];

				d3d12InputElementDesc.SemanticName = currentVertexAttribute.semanticName;
				d3d12InputElementDesc.SemanticIndex = currentVertexAttribute.semanticIndex;
				d3d12InputElementDesc.Format = Mapping::getDirect3D12Format(currentVertexAttribute.vertexAttributeFormat);
				d3d12InputElementDesc.InputSlot = currentVertexAttribute.inputSlot;
				d3d12InputElementDesc.AlignedByteOffset = currentVertexAttribute.alignedByteOffset;

				// Per-instance instead of per-vertex?
				if ( currentVertexAttribute.instancesPerElement > 0 )
				{
					d3d12InputElementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;	// Input classification (D3D12_INPUT_CLASSIFICATION)
					d3d12InputElementDesc.InstanceDataStepRate = currentVertexAttribute.instancesPerElement;	// Instance data step rate (UINT)
				}
				else
				{
					d3d12InputElementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;	// Input classification (D3D12_INPUT_CLASSIFICATION)
					d3d12InputElementDesc.InstanceDataStepRate = 0;												// Instance data step rate (UINT)
				}
			}

			// Describe and create the graphics pipeline state object (PSO)
			D3D12_GRAPHICS_PIPELINE_STATE_DESC d3d12GraphicsPipelineState = {};
			d3d12GraphicsPipelineState.InputLayout = { d3d12InputElementDescs, numberOfVertexAttributes };
			d3d12GraphicsPipelineState.pRootSignature = static_cast<RootSignature*>(mRootSignature)->getD3D12RootSignature();
			{ // Set shaders
				GraphicsProgramHlsl* graphicsProgramHlsl = static_cast<GraphicsProgramHlsl*>(mGraphicsProgram);
				const MeshShaderHlsl* meshShaderHlsl = graphicsProgramHlsl->getMeshShaderHlsl();
				if ( nullptr != meshShaderHlsl )
				{
					// Task and mesh shader based graphics program

					{ // Task shader
						const TaskShaderHlsl* taskShaderHlsl = graphicsProgramHlsl->getTaskShaderHlsl();
						if ( nullptr != taskShaderHlsl )
						{
							// TODO(co) "DirectX 12 Ultimate" needed
							// ID3DBlob* d3dBlobTaskShader = taskShaderHlsl->getD3DBlobTaskShader();
							// d3d12GraphicsPipelineState.AS = { reinterpret_cast<UINT8*>(d3dBlobTaskShader->GetBufferPointer()), d3dBlobTaskShader->GetBufferSize() };
						}
					}
					{ // Mesh shader
						// TODO(co) "DirectX 12 Ultimate" needed
						// ID3DBlob* d3dBlobMeshShader = meshShaderHlsl->getD3DBlobMeshShader();
						// d3d12GraphicsPipelineState.MS = { reinterpret_cast<UINT8*>(d3dBlobMeshShader->GetBufferPointer()), d3dBlobMeshShader->GetBufferSize() };
					}
					{ // Fragment shader (FS, "pixel shader" in Direct3D terminology)
						const FragmentShaderHlsl* fragmentShaderHlsl = graphicsProgramHlsl->getFragmentShaderHlsl();
						if ( nullptr != fragmentShaderHlsl )
						{
							ID3DBlob* d3dBlobFragmentShader = graphicsProgramHlsl->getFragmentShaderHlsl()->getD3DBlobFragmentShader();
							d3d12GraphicsPipelineState.PS = { reinterpret_cast<UINT8*>(d3dBlobFragmentShader->GetBufferPointer()), d3dBlobFragmentShader->GetBufferSize() };
						}
					}
				}
				else
				{
					// Traditional graphics program

					{ // Vertex shader
						const VertexShaderHlsl* vertexShaderHlsl = graphicsProgramHlsl->getVertexShaderHlsl();
						if ( nullptr != vertexShaderHlsl )
						{
							ID3DBlob* d3dBlobVertexShader = vertexShaderHlsl->getD3DBlobVertexShader();
							d3d12GraphicsPipelineState.VS = { reinterpret_cast<UINT8*>(d3dBlobVertexShader->GetBufferPointer()), d3dBlobVertexShader->GetBufferSize() };
						}
					}
					{ // Tessellation control shader (TCS, "hull shader" in Direct3D terminology)
						const TessellationControlShaderHlsl* tessellationControlShaderHlsl = graphicsProgramHlsl->getTessellationControlShaderHlsl();
						if ( nullptr != tessellationControlShaderHlsl )
						{
							ID3DBlob* d3dBlobHullShader = tessellationControlShaderHlsl->getD3DBlobHullShader();
							d3d12GraphicsPipelineState.HS = { reinterpret_cast<UINT8*>(d3dBlobHullShader->GetBufferPointer()), d3dBlobHullShader->GetBufferSize() };
						}
					}
					{ // Tessellation evaluation shader (TES, "domain shader" in Direct3D terminology)
						const TessellationEvaluationShaderHlsl* tessellationEvaluationShaderHlsl = graphicsProgramHlsl->getTessellationEvaluationShaderHlsl();
						if ( nullptr != tessellationEvaluationShaderHlsl )
						{
							ID3DBlob* d3dBlobDomainShader = tessellationEvaluationShaderHlsl->getD3DBlobDomainShader();
							d3d12GraphicsPipelineState.DS = { reinterpret_cast<UINT8*>(d3dBlobDomainShader->GetBufferPointer()), d3dBlobDomainShader->GetBufferSize() };
						}
					}
					{ // Geometry shader
						const GeometryShaderHlsl* geometryShaderHlsl = graphicsProgramHlsl->getGeometryShaderHlsl();
						if ( nullptr != geometryShaderHlsl )
						{
							ID3DBlob* d3dBlobGeometryShader = geometryShaderHlsl->getD3DBlobGeometryShader();
							d3d12GraphicsPipelineState.GS = { reinterpret_cast<UINT8*>(d3dBlobGeometryShader->GetBufferPointer()), d3dBlobGeometryShader->GetBufferSize() };
						}
					}
					{ // Fragment shader (FS, "pixel shader" in Direct3D terminology)
						const FragmentShaderHlsl* fragmentShaderHlsl = graphicsProgramHlsl->getFragmentShaderHlsl();
						if ( nullptr != fragmentShaderHlsl )
						{
							ID3DBlob* d3dBlobFragmentShader = graphicsProgramHlsl->getFragmentShaderHlsl()->getD3DBlobFragmentShader();
							d3d12GraphicsPipelineState.PS = { reinterpret_cast<UINT8*>(d3dBlobFragmentShader->GetBufferPointer()), d3dBlobFragmentShader->GetBufferSize() };
						}
					}
				}
			}
			d3d12GraphicsPipelineState.PrimitiveTopologyType = static_cast<D3D12_PRIMITIVE_TOPOLOGY_TYPE>(graphicsPipelineState.primitiveTopologyType);
			memcpy(&d3d12GraphicsPipelineState.RasterizerState, &graphicsPipelineState.rasterizerState, sizeof(D3D12_RASTERIZER_DESC));
			memcpy(&d3d12GraphicsPipelineState.DepthStencilState, &graphicsPipelineState.depthStencilState, sizeof(D3D12_DEPTH_STENCIL_DESC));
			{
				// TODO(co) "D3D12_RENDER_TARGET_BLEND_DESC" and "D3D11_RENDER_TARGET_BLEND_DESC" are different, we probably want to switch to "D3D12_RENDER_TARGET_BLEND_DESC"
				/*
					typedef struct D3D12_RENDER_TARGET_BLEND_DESC
					{
						BOOL BlendEnable;
						BOOL LogicOpEnable;
						D3D12_BLEND SrcBlend;
						D3D12_BLEND DestBlend;
						D3D12_BLEND_OP BlendOp;
						D3D12_BLEND SrcBlendAlpha;
						D3D12_BLEND DestBlendAlpha;
						D3D12_BLEND_OP BlendOpAlpha;
						D3D12_LOGIC_OP LogicOp;
						UINT8 RenderTargetWriteMask;
					} D3D12_RENDER_TARGET_BLEND_DESC;

					typedef struct D3D11_RENDER_TARGET_BLEND_DESC
					{
						BOOL			BlendEnable;
						D3D11_BLEND		SrcBlend;
						D3D11_BLEND		DestBlend;
						D3D11_BLEND_OP	BlendOp;
						D3D11_BLEND		SrcBlendAlpha;
						D3D11_BLEND		DestBlendAlpha;
						D3D11_BLEND_OP	BlendOpAlpha;
						UINT8			RenderTargetWriteMask;
					} D3D11_RENDER_TARGET_BLEND_DESC;
				*/
				D3D12_BLEND_DESC& d3d12BlendDesc = d3d12GraphicsPipelineState.BlendState;
				const Rhi::BlendState& blendState = graphicsPipelineState.blendState;
				d3d12BlendDesc.AlphaToCoverageEnable = blendState.alphaToCoverageEnable;
				d3d12BlendDesc.IndependentBlendEnable = blendState.independentBlendEnable;
				for ( uint32_t renderTargetIndex = 0; renderTargetIndex < 8; ++renderTargetIndex )
				{
					D3D12_RENDER_TARGET_BLEND_DESC& d3d12RenderTargetBlendDesc = d3d12BlendDesc.RenderTarget[renderTargetIndex];
					const Rhi::RenderTargetBlendDesc& renderTargetBlendDesc = blendState.renderTarget[renderTargetIndex];
					d3d12RenderTargetBlendDesc.BlendEnable = renderTargetBlendDesc.blendEnable;
					d3d12RenderTargetBlendDesc.LogicOpEnable = false;
					d3d12RenderTargetBlendDesc.SrcBlend = static_cast<D3D12_BLEND>(renderTargetBlendDesc.srcBlend);
					d3d12RenderTargetBlendDesc.DestBlend = static_cast<D3D12_BLEND>(renderTargetBlendDesc.destBlend);
					d3d12RenderTargetBlendDesc.BlendOp = static_cast<D3D12_BLEND_OP>(renderTargetBlendDesc.blendOp);
					d3d12RenderTargetBlendDesc.SrcBlendAlpha = static_cast<D3D12_BLEND>(renderTargetBlendDesc.srcBlendAlpha);
					d3d12RenderTargetBlendDesc.DestBlendAlpha = static_cast<D3D12_BLEND>(renderTargetBlendDesc.destBlendAlpha);
					d3d12RenderTargetBlendDesc.BlendOpAlpha = static_cast<D3D12_BLEND_OP>(renderTargetBlendDesc.blendOpAlpha);
					d3d12RenderTargetBlendDesc.LogicOp = D3D12_LOGIC_OP_CLEAR;
					d3d12RenderTargetBlendDesc.RenderTargetWriteMask = renderTargetBlendDesc.renderTargetWriteMask;
				}
			}
			d3d12GraphicsPipelineState.SampleMask = UINT_MAX;
			d3d12GraphicsPipelineState.NumRenderTargets = graphicsPipelineState.numberOfRenderTargets;
			for ( uint32_t i = 0; i < graphicsPipelineState.numberOfRenderTargets; ++i )
			{
				d3d12GraphicsPipelineState.RTVFormats[i] = Mapping::getDirect3D12Format(graphicsPipelineState.renderTargetViewFormats[i]);
			}
			d3d12GraphicsPipelineState.DSVFormat = Mapping::getDirect3D12Format(graphicsPipelineState.depthStencilViewFormat);
			d3d12GraphicsPipelineState.SampleDesc.Count = 1;

			if ( SUCCEEDED(direct3D12Rhi.getD3D12Device().CreateGraphicsPipelineState(&d3d12GraphicsPipelineState, IID_PPV_ARGS(&mD3D12GraphicsPipelineState))) )
			{
				// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Graphics PSO", 15)	// 15 = "Graphics PSO: " including terminating zero
					FAILED_DEBUG_BREAK(mD3D12GraphicsPipelineState->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
#endif
			}
			else
			{
				RHI_LOG(CRITICAL, "Failed to create the Direct3D 12 graphics pipeline state object")
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GraphicsPipelineState() override
		{
			// Release the Direct3D 12 graphics pipeline state
			if ( nullptr != mD3D12GraphicsPipelineState )
			{
				mD3D12GraphicsPipelineState->Release();
			}

			// Release referenced RHI resources
			mRootSignature->ReleaseReference();
			mGraphicsProgram->ReleaseReference();
			mRenderPass->ReleaseReference();

			// Free the unique compact graphics pipeline state ID
			static_cast<Direct3D12Rhi&>(getRhi()).GraphicsPipelineStateMakeId.DestroyID(getId());
		}

		/**
		*  @brief
		*    Return the Direct3D 12 primitive topology
		*
		*  @return
		*    The Direct3D 12 primitive topology
		*/
		[[nodiscard]] inline D3D12_PRIMITIVE_TOPOLOGY getD3D12PrimitiveTopology() const
		{
			return mD3D12PrimitiveTopology;
		}

		/**
		*  @brief
		*    Return the Direct3D 12 graphics pipeline state
		*
		*  @return
		*    The Direct3D 12 graphics pipeline state, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3D12PipelineState* getD3D12GraphicsPipelineState() const
		{
			return mD3D12GraphicsPipelineState;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(GraphicsPipelineState, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit GraphicsPipelineState(const GraphicsPipelineState&) = delete;
		GraphicsPipelineState& operator =(const GraphicsPipelineState&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		D3D12_PRIMITIVE_TOPOLOGY mD3D12PrimitiveTopology;
		ID3D12PipelineState*	 mD3D12GraphicsPipelineState;	// Direct3D 12 graphics pipeline state, can be a null pointer
		Rhi::IRootSignature*	 mRootSignature;
		Rhi::IGraphicsProgram*	 mGraphicsProgram;
		Rhi::IRenderPass*		 mRenderPass;


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/State/ComputePipelineState.h            ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 compute pipeline state class
	*/
	class ComputePipelineState final : public Rhi::IComputePipelineState
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] direct3D12Rhi
		*    Owner Direct3D 12 RHI instance
		*  @param[in] rootSignature
		*    Root signature shader to use
		*  @param[in] computeShader
		*    Compute shader to use
		*  @param[in] id
		*    The unique compact compute pipeline state ID
		*/
		ComputePipelineState(Direct3D12Rhi& direct3D12Rhi, Rhi::IRootSignature& rootSignature, Rhi::IComputeShader& computeShader, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
			IComputePipelineState(direct3D12Rhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mD3D12ComputePipelineState(nullptr),
			mRootSignature(rootSignature),
			mComputeShader(computeShader)
		{
			// Add a reference to the given root signature and compute shader
			rootSignature.AddReference();
			computeShader.AddReference();

			// Describe and create the compute pipeline state object (PSO)
			D3D12_COMPUTE_PIPELINE_STATE_DESC d3d12ComputePipelineState = {};
			d3d12ComputePipelineState.pRootSignature = static_cast<RootSignature&>(rootSignature).getD3D12RootSignature();
			{ // Set compute shader
				ID3DBlob* d3dBlobComputeShader = static_cast<ComputeShaderHlsl&>(computeShader).getD3DBlobComputeShader();
				d3d12ComputePipelineState.CS = { reinterpret_cast<UINT8*>(d3dBlobComputeShader->GetBufferPointer()), d3dBlobComputeShader->GetBufferSize() };
			}
			if ( SUCCEEDED(direct3D12Rhi.getD3D12Device().CreateComputePipelineState(&d3d12ComputePipelineState, IID_PPV_ARGS(&mD3D12ComputePipelineState))) )
			{
				// Assign a default name to the resource for debugging purposes
#if SE_DEBUG
				RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Compute PSO", 14)	// 14 = "Compute PSO: " including terminating zero
					FAILED_DEBUG_BREAK(mD3D12ComputePipelineState->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(detailedDebugName)), detailedDebugName))
#endif
			}
			else
			{
				RHI_LOG(CRITICAL, "Failed to create the Direct3D 12 compute pipeline state object")
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ComputePipelineState() override
		{
			// Release the Direct3D 12 compute pipeline state
			if ( nullptr != mD3D12ComputePipelineState )
			{
				mD3D12ComputePipelineState->Release();
			}

			// Release the root signature and compute shader reference
			mRootSignature.ReleaseReference();
			mComputeShader.ReleaseReference();

			// Free the unique compact compute pipeline state ID
			static_cast<Direct3D12Rhi&>(getRhi()).ComputePipelineStateMakeId.DestroyID(getId());
		}

		/**
		*  @brief
		*    Return the Direct3D 12 compute pipeline state
		*
		*  @return
		*    The Direct3D 12 compute pipeline state, can be a null pointer, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline ID3D12PipelineState* getD3D12ComputePipelineState() const
		{
			return mD3D12ComputePipelineState;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(ComputePipelineState, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit ComputePipelineState(const ComputePipelineState&) = delete;
		ComputePipelineState& operator =(const ComputePipelineState&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		ID3D12PipelineState* mD3D12ComputePipelineState;	// Direct3D 12 compute pipeline state, can be a null pointer
		Rhi::IRootSignature& mRootSignature;
		Rhi::IComputeShader& mComputeShader;


	};




	//[-------------------------------------------------------]
	//[ Direct3D12Rhi/ResourceGroup.h                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Direct3D 12 resource group class
	*/
	class ResourceGroup final : public Rhi::IResourceGroup
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rootSignature
		*    Root signature
		*  @param[in] d3d12DescriptorHeapType
		*    Direct3D 12 descriptor heap type
		*  @param[in] numberOfResources
		*    Number of resources, having no resources is invalid
		*  @param[in] resources
		*    At least "numberOfResources" resource pointers, must be valid, the resource group will keep a reference to the resources
		*  @param[in] samplerStates
		*    If not a null pointer at least "numberOfResources" sampler state pointers, must be valid if there's at least one texture resource, the resource group will keep a reference to the sampler states
		*/
		ResourceGroup(RootSignature& rootSignature, D3D12_DESCRIPTOR_HEAP_TYPE d3d12DescriptorHeapType, uint32_t numberOfResources, Rhi::IResource** resources, Rhi::ISamplerState** samplerStates RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResourceGroup(rootSignature.getRhi() RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mRootSignature(rootSignature),
			mD3D12DescriptorHeapType(d3d12DescriptorHeapType),
			mNumberOfResources(numberOfResources),
			mResources(RHI_MALLOC_TYPED(Rhi::IResource*, mNumberOfResources)),
			mSamplerStates(nullptr),
			mDescriptorHeapOffset(0),
			mDescriptorHeapSize(static_cast<uint16_t>(numberOfResources))
		{
			mRootSignature.AddReference();

			// Sanity check
			const Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());
			SE_ASSERT(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV == mD3D12DescriptorHeapType || D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER == mD3D12DescriptorHeapType, "Invalid Direct3D 12 descriptor heap type, must be \"D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV\" or \"D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER\"")

				// Process all resources and add our reference to the RHI resource
				if ( nullptr != samplerStates )
				{
					mSamplerStates = RHI_MALLOC_TYPED(Rhi::ISamplerState*, mNumberOfResources);
					for ( uint32_t resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex )
					{
						Rhi::ISamplerState* samplerState = mSamplerStates[resourceIndex] = samplerStates[resourceIndex];
						if ( nullptr != samplerState )
						{
							samplerState->AddReference();
						}
					}
				}
			ID3D12Device& d3d12Device = direct3D12Rhi.getD3D12Device();
			if ( D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV == d3d12DescriptorHeapType )
			{
				::detail::DescriptorHeap& descriptorHeap = direct3D12Rhi.getShaderResourceViewDescriptorHeap();
				mDescriptorHeapOffset = descriptorHeap.allocate(static_cast<uint16_t>(numberOfResources));
				const uint32_t descriptorSize = descriptorHeap.getDescriptorSize();
				D3D12_CPU_DESCRIPTOR_HANDLE d3d12CpuDescriptorHandle = descriptorHeap.getD3D12CpuDescriptorHandleForHeapStart();
				d3d12CpuDescriptorHandle.ptr += mDescriptorHeapOffset * descriptorSize;
				for ( uint32_t resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex, ++resources )
				{
					Rhi::IResource* resource = *resources;
					SE_ASSERT(nullptr != resource, "Invalid Direct3D 12 resource")
						mResources[resourceIndex] = resource;
					resource->AddReference();

					// Check the type of resource to set
					// TODO(co) Some additional resource type root signature security checks in debug build?
					const Rhi::ResourceType resourceType = resource->getResourceType();
					switch ( resourceType )
					{
					case Rhi::ResourceType::INDEX_BUFFER:
						{
							// TODO(co)
							SE_ASSERT(false, "TODO(co) Implement me")
								/*
								const IndexBuffer* indexBuffer = static_cast<IndexBuffer*>(resource);
								SE_ASSERT(nullptr != indexBuffer->getD3D12Resource(), "Invalid Direct3D 12 index buffer resource")
								const D3D12_CONSTANT_BUFFER_VIEW_DESC d3d12ConstantBufferViewDesc = { indexBuffer->getD3D12Resource()->GetGPUVirtualAddress(), indexBuffer->getNumberOfBytesOnGpu() };
								d3d12Device.CreateConstantBufferView(&d3d12ConstantBufferViewDesc, d3d12CpuDescriptorHandle);
								*/
								break;
						}

					case Rhi::ResourceType::VERTEX_BUFFER:
						{
							// TODO(co)
							SE_ASSERT(false, "TODO(co) Implement me")
								/*
								const VertexBuffer* vertexBuffer = static_cast<VertexBuffer*>(resource);
								SE_ASSERT(nullptr != vertexBuffer->getD3D12Resource(), "Invalid Direct3D 12 vertex buffer resource")
								const D3D12_CONSTANT_BUFFER_VIEW_DESC d3d12ConstantBufferViewDesc = { vertexBuffer->getD3D12Resource()->GetGPUVirtualAddress(), vertexBuffer->getNumberOfBytesOnGpu() };
								d3d12Device.CreateConstantBufferView(&d3d12ConstantBufferViewDesc, d3d12CpuDescriptorHandle);
								*/
								break;
						}

					case Rhi::ResourceType::TEXTURE_BUFFER:
						{
							const TextureBuffer* textureBuffer = static_cast<TextureBuffer*>(resource);
							SE_ASSERT(nullptr != textureBuffer->getD3D12Resource(), "Invalid Direct3D 12 texture buffer resource")
								const Rhi::TextureFormat::Enum textureFormat = textureBuffer->getTextureFormat();
							D3D12_SHADER_RESOURCE_VIEW_DESC d3d12ShaderResourceViewDesc = {};
							d3d12ShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
							d3d12ShaderResourceViewDesc.Format = Mapping::getDirect3D12Format(textureFormat);
							d3d12ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
							d3d12ShaderResourceViewDesc.Buffer.FirstElement = 0;
							d3d12ShaderResourceViewDesc.Buffer.NumElements = textureBuffer->getNumberOfBytes() / Rhi::TextureFormat::GetNumberOfBytesPerElement(textureFormat);
							d3d12Device.CreateShaderResourceView(textureBuffer->getD3D12Resource(), &d3d12ShaderResourceViewDesc, d3d12CpuDescriptorHandle);
							break;
						}

					case Rhi::ResourceType::STRUCTURED_BUFFER:
						{
							// TODO(co)
							SE_ASSERT(false, "TODO(co) Implement me")
								/*
								const StructuredBuffer* structuredBuffer = static_cast<StructuredBuffer*>(resource);
								SE_ASSERT(nullptr != structuredBuffer->getD3D12Resource(), "Invalid Direct3D 12 structured buffer resource")
								const D3D12_CONSTANT_BUFFER_VIEW_DESC d3d12ConstantBufferViewDesc = { structuredBuffer->getD3D12Resource()->GetGPUVirtualAddress(), structuredBuffer->getNumberOfBytesOnGpu() };
								d3d12Device.CreateConstantBufferView(&d3d12ConstantBufferViewDesc, d3d12CpuDescriptorHandle);
								*/
								break;
						}

					case Rhi::ResourceType::INDIRECT_BUFFER:
						{
							// TODO(co)
							SE_ASSERT(false, "TODO(co) Implement me")
								/*
								const IndirectBuffer* indirectBuffer = static_cast<IndirectBuffer*>(resource);
								SE_ASSERT(nullptr != indirectBuffer->getD3D12Resource(), "Invalid Direct3D 12 indirect buffer resource")
								const D3D12_CONSTANT_BUFFER_VIEW_DESC d3d12ConstantBufferViewDesc = { indirectBuffer->getD3D12Resource()->GetGPUVirtualAddress(), indirectBuffer->getNumberOfBytesOnGpu() };
								d3d12Device.CreateConstantBufferView(&d3d12ConstantBufferViewDesc, d3d12CpuDescriptorHandle);
								*/
								break;
						}

					case Rhi::ResourceType::UNIFORM_BUFFER:
						{
							const UniformBuffer* uniformBuffer = static_cast<UniformBuffer*>(resource);
							SE_ASSERT(nullptr != uniformBuffer->getD3D12Resource(), "Invalid Direct3D 12 uniform buffer resource")
								const D3D12_CONSTANT_BUFFER_VIEW_DESC d3d12ConstantBufferViewDesc = { uniformBuffer->getD3D12Resource()->GetGPUVirtualAddress(), uniformBuffer->getNumberOfBytesOnGpu() };
							d3d12Device.CreateConstantBufferView(&d3d12ConstantBufferViewDesc, d3d12CpuDescriptorHandle);
							break;
						}

					case Rhi::ResourceType::TEXTURE_1D:
					case Rhi::ResourceType::TEXTURE_1D_ARRAY:
					case Rhi::ResourceType::TEXTURE_2D:
					case Rhi::ResourceType::TEXTURE_2D_ARRAY:
					case Rhi::ResourceType::TEXTURE_3D:
					case Rhi::ResourceType::TEXTURE_CUBE:
					case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
						{
							// Evaluate the texture type and create the Direct3D 12 shader resource view
							ID3D12Resource* d3d12Resource = nullptr;
							D3D12_SHADER_RESOURCE_VIEW_DESC d3d12ShaderResourceViewDesc = {};
							d3d12ShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
							switch ( resourceType )
							{
							case Rhi::ResourceType::TEXTURE_1D:
								{
									const Texture1D* texture1D = static_cast<Texture1D*>(resource);
									d3d12ShaderResourceViewDesc.Format = texture1D->getDxgiFormat();
									d3d12ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
									d3d12ShaderResourceViewDesc.Texture1D.MipLevels = texture1D->getNumberOfMipmaps();
									d3d12Resource = texture1D->getD3D12Resource();
									break;
								}

							case Rhi::ResourceType::TEXTURE_1D_ARRAY:
								{
									const Texture1DArray* texture1DArray = static_cast<Texture1DArray*>(resource);
									d3d12ShaderResourceViewDesc.Format = texture1DArray->getDxgiFormat();
									d3d12ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
									d3d12ShaderResourceViewDesc.Texture1DArray.MipLevels = texture1DArray->getNumberOfMipmaps();
									d3d12ShaderResourceViewDesc.Texture1DArray.ArraySize = texture1DArray->getNumberOfSlices();
									d3d12Resource = texture1DArray->getD3D12Resource();
									break;
								}

							case Rhi::ResourceType::TEXTURE_2D:
								{
									const Texture2D* texture2D = static_cast<Texture2D*>(resource);
									d3d12ShaderResourceViewDesc.Format = texture2D->getDxgiFormat();
									d3d12ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
									d3d12ShaderResourceViewDesc.Texture2D.MipLevels = texture2D->getNumberOfMipmaps();
									d3d12Resource = texture2D->getD3D12Resource();
									break;
								}

							case Rhi::ResourceType::TEXTURE_2D_ARRAY:
								{
									const Texture2DArray* texture2DArray = static_cast<Texture2DArray*>(resource);
									d3d12ShaderResourceViewDesc.Format = texture2DArray->getDxgiFormat();
									d3d12ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
									d3d12ShaderResourceViewDesc.Texture2DArray.MipLevels = texture2DArray->getNumberOfMipmaps();
									d3d12ShaderResourceViewDesc.Texture2DArray.ArraySize = texture2DArray->getNumberOfSlices();
									d3d12Resource = texture2DArray->getD3D12Resource();
									break;
								}

							case Rhi::ResourceType::TEXTURE_3D:
								{
									const Texture3D* texture3D = static_cast<Texture3D*>(resource);
									d3d12ShaderResourceViewDesc.Format = texture3D->getDxgiFormat();
									d3d12ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
									d3d12ShaderResourceViewDesc.Texture3D.MipLevels = texture3D->getNumberOfMipmaps();
									d3d12Resource = texture3D->getD3D12Resource();
									break;
								}

							case Rhi::ResourceType::TEXTURE_CUBE:
								{
									const TextureCube* textureCube = static_cast<TextureCube*>(resource);
									d3d12ShaderResourceViewDesc.Format = textureCube->getDxgiFormat();
									d3d12ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
									d3d12ShaderResourceViewDesc.TextureCube.MipLevels = textureCube->getNumberOfMipmaps();
									d3d12Resource = textureCube->getD3D12Resource();
									break;
								}

							case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
								{
									// TODO(co) Implement me
									/*
									const TextureCubeArray* textureCubeArray = static_cast<TextureCubeArray*>(resource);
									d3d12ShaderResourceViewDesc.Format = textureCubeArray->getDxgiFormat();
									d3d12ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
									d3d12ShaderResourceViewDesc.TextureCubeArray.MipLevels = textureCubeArray->getNumberOfMipmaps();
									d3d12Resource = textureCubeArray->getD3D12Resource();
									*/
									break;
								}

							case Rhi::ResourceType::ROOT_SIGNATURE:
							case Rhi::ResourceType::RESOURCE_GROUP:
							case Rhi::ResourceType::GRAPHICS_PROGRAM:
							case Rhi::ResourceType::VERTEX_ARRAY:
							case Rhi::ResourceType::RENDER_PASS:
							case Rhi::ResourceType::QUERY_POOL:
							case Rhi::ResourceType::SWAP_CHAIN:
							case Rhi::ResourceType::FRAMEBUFFER:
							case Rhi::ResourceType::VERTEX_BUFFER:
							case Rhi::ResourceType::INDEX_BUFFER:
							case Rhi::ResourceType::INDIRECT_BUFFER:
							case Rhi::ResourceType::TEXTURE_BUFFER:
							case Rhi::ResourceType::STRUCTURED_BUFFER:
							case Rhi::ResourceType::UNIFORM_BUFFER:
							case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
							case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
							case Rhi::ResourceType::SAMPLER_STATE:
							case Rhi::ResourceType::VERTEX_SHADER:
							case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
							case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
							case Rhi::ResourceType::GEOMETRY_SHADER:
							case Rhi::ResourceType::FRAGMENT_SHADER:
							case Rhi::ResourceType::TASK_SHADER:
							case Rhi::ResourceType::MESH_SHADER:
							case Rhi::ResourceType::COMPUTE_SHADER:
								RHI_LOG(CRITICAL, "Invalid Direct3D 12 RHI implementation resource type")
									break;
							}
							SE_ASSERT(nullptr != d3d12Resource, "Invalid Direct3D 12 resource")
								d3d12Device.CreateShaderResourceView(d3d12Resource, &d3d12ShaderResourceViewDesc, d3d12CpuDescriptorHandle);
							break;
						}

					case Rhi::ResourceType::SAMPLER_STATE:
					case Rhi::ResourceType::ROOT_SIGNATURE:
					case Rhi::ResourceType::RESOURCE_GROUP:
					case Rhi::ResourceType::GRAPHICS_PROGRAM:
					case Rhi::ResourceType::VERTEX_ARRAY:
					case Rhi::ResourceType::RENDER_PASS:
					case Rhi::ResourceType::QUERY_POOL:
					case Rhi::ResourceType::SWAP_CHAIN:
					case Rhi::ResourceType::FRAMEBUFFER:
					case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
					case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
					case Rhi::ResourceType::VERTEX_SHADER:
					case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
					case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
					case Rhi::ResourceType::GEOMETRY_SHADER:
					case Rhi::ResourceType::FRAGMENT_SHADER:
					case Rhi::ResourceType::TASK_SHADER:
					case Rhi::ResourceType::MESH_SHADER:
					case Rhi::ResourceType::COMPUTE_SHADER:
						RHI_LOG(CRITICAL, "Invalid Direct3D 12 RHI implementation resource type")
							break;
					}
					d3d12CpuDescriptorHandle.ptr += descriptorSize;
				}
				SE_ASSERT(d3d12CpuDescriptorHandle.ptr == descriptorHeap.getD3D12CpuDescriptorHandleForHeapStart().ptr + (mDescriptorHeapOffset + mNumberOfResources) * descriptorSize, "Direct3D 12 descriptor heap invalid")
			}
			else
			{
				::detail::DescriptorHeap& descriptorHeap = direct3D12Rhi.getSamplerDescriptorHeap();
				mDescriptorHeapOffset = descriptorHeap.allocate(static_cast<uint16_t>(numberOfResources));
				const uint32_t descriptorSize = descriptorHeap.getDescriptorSize();
				D3D12_CPU_DESCRIPTOR_HANDLE d3d12CpuDescriptorHandle = descriptorHeap.getD3D12CpuDescriptorHandleForHeapStart();
				d3d12CpuDescriptorHandle.ptr += mDescriptorHeapOffset * descriptorSize;
				for ( uint32_t resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex, ++resources )
				{
					Rhi::IResource* resource = *resources;
					SE_ASSERT(nullptr != resource, "Invalid Direct3D 12 resource")
						mResources[resourceIndex] = resource;
					resource->AddReference();

					// Check the type of resource to set
					// TODO(co) Some additional resource type root signature security checks in debug build?
					switch ( resource->getResourceType() )
					{
					case Rhi::ResourceType::SAMPLER_STATE:
						d3d12Device.CreateSampler(reinterpret_cast<const D3D12_SAMPLER_DESC*>(&static_cast<SamplerState*>(resource)->getSamplerState()), d3d12CpuDescriptorHandle);
						break;

					case Rhi::ResourceType::VERTEX_BUFFER:
					case Rhi::ResourceType::INDEX_BUFFER:
					case Rhi::ResourceType::TEXTURE_BUFFER:
					case Rhi::ResourceType::STRUCTURED_BUFFER:
					case Rhi::ResourceType::INDIRECT_BUFFER:
					case Rhi::ResourceType::UNIFORM_BUFFER:
					case Rhi::ResourceType::TEXTURE_1D:
					case Rhi::ResourceType::TEXTURE_1D_ARRAY:
					case Rhi::ResourceType::TEXTURE_2D:
					case Rhi::ResourceType::TEXTURE_2D_ARRAY:
					case Rhi::ResourceType::TEXTURE_3D:
					case Rhi::ResourceType::TEXTURE_CUBE:
					case Rhi::ResourceType::TEXTURE_CUBE_ARRAY:
					case Rhi::ResourceType::ROOT_SIGNATURE:
					case Rhi::ResourceType::RESOURCE_GROUP:
					case Rhi::ResourceType::GRAPHICS_PROGRAM:
					case Rhi::ResourceType::VERTEX_ARRAY:
					case Rhi::ResourceType::RENDER_PASS:
					case Rhi::ResourceType::QUERY_POOL:
					case Rhi::ResourceType::SWAP_CHAIN:
					case Rhi::ResourceType::FRAMEBUFFER:
					case Rhi::ResourceType::GRAPHICS_PIPELINE_STATE:
					case Rhi::ResourceType::COMPUTE_PIPELINE_STATE:
					case Rhi::ResourceType::VERTEX_SHADER:
					case Rhi::ResourceType::TESSELLATION_CONTROL_SHADER:
					case Rhi::ResourceType::TESSELLATION_EVALUATION_SHADER:
					case Rhi::ResourceType::GEOMETRY_SHADER:
					case Rhi::ResourceType::FRAGMENT_SHADER:
					case Rhi::ResourceType::TASK_SHADER:
					case Rhi::ResourceType::MESH_SHADER:
					case Rhi::ResourceType::COMPUTE_SHADER:
						RHI_LOG(CRITICAL, "Invalid Direct3D 12 RHI implementation resource type")
							break;
					}
					d3d12CpuDescriptorHandle.ptr += descriptorSize;
				}
				SE_ASSERT(d3d12CpuDescriptorHandle.ptr == descriptorHeap.getD3D12CpuDescriptorHandleForHeapStart().ptr + (mDescriptorHeapOffset + mNumberOfResources) * descriptorSize, "Direct3D 12 descriptor heap invalid")
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ResourceGroup() override
		{
			// Remove our reference from the RHI resources
			Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());
			if ( nullptr != mSamplerStates )
			{
				for ( uint32_t resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex )
				{
					Rhi::ISamplerState* samplerState = mSamplerStates[resourceIndex];
					if ( nullptr != samplerState )
					{
						samplerState->ReleaseReference();
					}
				}
				RHI_FREE(mSamplerStates);
			}
			for ( uint32_t resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex )
			{
				mResources[resourceIndex]->ReleaseReference();
			}
			RHI_FREE(mResources);
			mRootSignature.ReleaseReference();

			// Release descriptor heap
			((D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV == mD3D12DescriptorHeapType) ? direct3D12Rhi.getShaderResourceViewDescriptorHeap() : direct3D12Rhi.getSamplerDescriptorHeap()).release(mDescriptorHeapOffset, mDescriptorHeapSize);
		}

		/**
		*  @brief
		*    Return the Direct3D 12 descriptor heap type
		*
		*  @return
		*    The Direct3D 12 descriptor heap type ("D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV" or "D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER")
		*/
		[[nodiscard]] inline D3D12_DESCRIPTOR_HEAP_TYPE getD3D12DescriptorHeapType() const
		{
			return mD3D12DescriptorHeapType;
		}

		/**
		*  @brief
		*    Return the descriptor heap offset
		*
		*  @return
		*    The descriptor heap offset
		*/
		[[nodiscard]] inline uint16_t getDescriptorHeapOffset() const
		{
			return mDescriptorHeapOffset;
		}

		/**
		*  @brief
		*    Return the descriptor heap size
		*
		*  @return
		*    The descriptor heap size
		*/
		[[nodiscard]] inline uint16_t getDescriptorHeapSize() const
		{
			return mDescriptorHeapSize;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() noexcept override
		{
			RHI_DELETE(ResourceGroup, this);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit ResourceGroup(const ResourceGroup&) = delete;
		ResourceGroup& operator =(const ResourceGroup&) = delete;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		RootSignature&			   mRootSignature;				// Root signature
		D3D12_DESCRIPTOR_HEAP_TYPE mD3D12DescriptorHeapType;	// The Direct3D 12 descriptor heap type ("D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV" or "D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER")
		uint32_t				   mNumberOfResources;			// Number of resources this resource group groups together
		Rhi::IResource**		   mResources;					// RHI resource, we keep a reference to it
		Rhi::ISamplerState**	   mSamplerStates;				// Sampler states, we keep a reference to it
		uint16_t				   mDescriptorHeapOffset;
		uint16_t				   mDescriptorHeapSize;


	};

	Rhi::IResourceGroup* RootSignature::createResourceGroup([[maybe_unused]] uint32_t rootParameterIndex, uint32_t numberOfResources, Rhi::IResource** resources, [[maybe_unused]] Rhi::ISamplerState** samplerStates RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		// Sanity checks
		Direct3D12Rhi& direct3D12Rhi = static_cast<Direct3D12Rhi&>(getRhi());
		SE_ASSERT(numberOfResources > 0, "The number of Direct3D 12 resources must not be zero")
			SE_ASSERT(nullptr != resources, "The Direct3D 12 resource pointers must be valid")

			// Figure out the Direct3D 12 descriptor heap type
			D3D12_DESCRIPTOR_HEAP_TYPE d3d12DescriptorHeapType = D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES;
		for ( uint32_t resourceIndex = 0; resourceIndex < numberOfResources; ++resourceIndex )
		{
			Rhi::IResource* resource = *resources;
			SE_ASSERT(nullptr != resource, "Invalid Direct3D 12 resource")
				const Rhi::ResourceType resourceType = resource->getResourceType();
			if ( Rhi::ResourceType::SAMPLER_STATE == resourceType )
			{
				SE_ASSERT(D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES == d3d12DescriptorHeapType || D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER == d3d12DescriptorHeapType, "Direct3D 12 resource groups can't mix samplers with other resource types")
					d3d12DescriptorHeapType = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
			}
			else
			{
				SE_ASSERT(D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES == d3d12DescriptorHeapType || D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV == d3d12DescriptorHeapType, "Direct3D 12 resource groups can't mix samplers with other resource types")
					d3d12DescriptorHeapType = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			}
		}

		// Create resource group
		return RHI_NEW(ResourceGroup)(*this, d3d12DescriptorHeapType, numberOfResources, resources, samplerStates RHI_RESOURCE_DEBUG_PASS_PARAMETER);
	}




	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
} // Direct3D12Rhi

//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{
		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		namespace ImplementationDispatch
		{
			//[-------------------------------------------------------]
			//[ Command buffer                                        ]
			//[-------------------------------------------------------]
			void ExecuteCommandBuffer(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::ExecuteCommandBuffer* realData = static_cast<const Rhi::Command::ExecuteCommandBuffer*>(data);
				SE_ASSERT(nullptr != realData->commandBufferToExecute, "The Direct3D 12 command buffer to execute must be valid")
					rhi.submitCommandBuffer(*realData->commandBufferToExecute);
			}

			//[-------------------------------------------------------]
			//[ Graphics                                              ]
			//[-------------------------------------------------------]
			void SetGraphicsRootSignature(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetGraphicsRootSignature* realData = static_cast<const Rhi::Command::SetGraphicsRootSignature*>(data);
				static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).setGraphicsRootSignature(realData->rootSignature);
			}

			void SetGraphicsPipelineState(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetGraphicsPipelineState* realData = static_cast<const Rhi::Command::SetGraphicsPipelineState*>(data);
				static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).setGraphicsPipelineState(realData->graphicsPipelineState);
			}

			void SetGraphicsResourceGroup(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetGraphicsResourceGroup* realData = static_cast<const Rhi::Command::SetGraphicsResourceGroup*>(data);
				static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).setGraphicsResourceGroup(realData->rootParameterIndex, realData->resourceGroup);
			}

			void SetGraphicsVertexArray(const void* data, Rhi::IRhi& rhi)
			{
				// Input-assembler (IA) stage
				const Rhi::Command::SetGraphicsVertexArray* realData = static_cast<const Rhi::Command::SetGraphicsVertexArray*>(data);
				static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).setGraphicsVertexArray(realData->vertexArray);
			}

			void SetGraphicsViewports(const void* data, Rhi::IRhi& rhi)
			{
				// Rasterizer (RS) stage
				const Rhi::Command::SetGraphicsViewports* realData = static_cast<const Rhi::Command::SetGraphicsViewports*>(data);
				static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).setGraphicsViewports(realData->numberOfViewports, (nullptr != realData->viewports) ? realData->viewports : reinterpret_cast<const Rhi::Viewport*>(Rhi::CommandPacketHelper::getAuxiliaryMemory(realData)));
			}

			void SetGraphicsScissorRectangles(const void* data, Rhi::IRhi& rhi)
			{
				// Rasterizer (RS) stage
				const Rhi::Command::SetGraphicsScissorRectangles* realData = static_cast<const Rhi::Command::SetGraphicsScissorRectangles*>(data);
				static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).setGraphicsScissorRectangles(realData->numberOfScissorRectangles, (nullptr != realData->scissorRectangles) ? realData->scissorRectangles : reinterpret_cast<const Rhi::ScissorRectangle*>(Rhi::CommandPacketHelper::getAuxiliaryMemory(realData)));
			}

			void SetGraphicsRenderTarget(const void* data, Rhi::IRhi& rhi)
			{
				// Output-merger (OM) stage
				const Rhi::Command::SetGraphicsRenderTarget* realData = static_cast<const Rhi::Command::SetGraphicsRenderTarget*>(data);
				static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).setGraphicsRenderTarget(realData->renderTarget);
			}

			void ClearGraphics(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::ClearGraphics* realData = static_cast<const Rhi::Command::ClearGraphics*>(data);
				static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).clearGraphics(realData->clearFlags, realData->color, realData->z, realData->stencil);
			}

			void DrawGraphics(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::DrawGraphics* realData = static_cast<const Rhi::Command::DrawGraphics*>(data);
				if ( nullptr != realData->indirectBuffer )
				{
					static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).drawGraphics(*realData->indirectBuffer, realData->indirectBufferOffset, realData->numberOfDraws);
				}
				else
				{
					static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).drawGraphicsEmulated(Rhi::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
				}
			}

			void DrawIndexedGraphics(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::DrawIndexedGraphics* realData = static_cast<const Rhi::Command::DrawIndexedGraphics*>(data);
				if ( nullptr != realData->indirectBuffer )
				{
					static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).drawIndexedGraphics(*realData->indirectBuffer, realData->indirectBufferOffset, realData->numberOfDraws);
				}
				else
				{
					static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).drawIndexedGraphicsEmulated(Rhi::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
				}
			}

			void DrawMeshTasks(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::DrawMeshTasks* realData = static_cast<const Rhi::Command::DrawMeshTasks*>(data);
				if ( nullptr != realData->indirectBuffer )
				{
					static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).drawMeshTasks(*realData->indirectBuffer, realData->indirectBufferOffset, realData->numberOfDraws);
				}
				else
				{
					static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).drawMeshTasksEmulated(Rhi::CommandPacketHelper::getAuxiliaryMemory(realData), realData->indirectBufferOffset, realData->numberOfDraws);
				}
			}

			//[-------------------------------------------------------]
			//[ Compute                                               ]
			//[-------------------------------------------------------]
			void SetComputeRootSignature(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetComputeRootSignature* realData = static_cast<const Rhi::Command::SetComputeRootSignature*>(data);
				static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).setComputeRootSignature(realData->rootSignature);
			}

			void SetComputePipelineState(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetComputePipelineState* realData = static_cast<const Rhi::Command::SetComputePipelineState*>(data);
				static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).setComputePipelineState(realData->computePipelineState);
			}

			void SetComputeResourceGroup(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetComputeResourceGroup* realData = static_cast<const Rhi::Command::SetComputeResourceGroup*>(data);
				static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).setComputeResourceGroup(realData->rootParameterIndex, realData->resourceGroup);
			}

			void DispatchCompute(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::DispatchCompute* realData = static_cast<const Rhi::Command::DispatchCompute*>(data);
				static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).dispatchCompute(realData->groupCountX, realData->groupCountY, realData->groupCountZ);
			}

			//[-------------------------------------------------------]
			//[ Resource                                              ]
			//[-------------------------------------------------------]
			void SetTextureMinimumMaximumMipmapIndex(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetTextureMinimumMaximumMipmapIndex* realData = static_cast<const Rhi::Command::SetTextureMinimumMaximumMipmapIndex*>(data);
				if ( realData->texture->getResourceType() == Rhi::ResourceType::TEXTURE_2D )
				{
					static_cast<Direct3D12Rhi::Texture2D*>(realData->texture)->setMinimumMaximumMipmapIndex(realData->minimumMipmapIndex, realData->maximumMipmapIndex);
				}
				else
				{
					RHI_LOG(CRITICAL, "Unsupported Direct3D 12 texture resource type")
				}
			}

			void ResolveMultisampleFramebuffer(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::ResolveMultisampleFramebuffer* realData = static_cast<const Rhi::Command::ResolveMultisampleFramebuffer*>(data);
				static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).resolveMultisampleFramebuffer(*realData->destinationRenderTarget, *realData->sourceMultisampleFramebuffer);
			}

			void CopyResource(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::CopyResource* realData = static_cast<const Rhi::Command::CopyResource*>(data);
				static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).copyResource(*realData->destinationResource, *realData->sourceResource);
			}

			void GenerateMipmaps(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::GenerateMipmaps* realData = static_cast<const Rhi::Command::GenerateMipmaps*>(data);
				static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).generateMipmaps(*realData->resource);
			}

			//[-------------------------------------------------------]
			//[ Query                                                 ]
			//[-------------------------------------------------------]
			void ResetQueryPool(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::ResetQueryPool* realData = static_cast<const Rhi::Command::ResetQueryPool*>(data);
				static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).resetQueryPool(*realData->queryPool, realData->firstQueryIndex, realData->numberOfQueries);
			}

			void BeginQuery(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::BeginQuery* realData = static_cast<const Rhi::Command::BeginQuery*>(data);
				static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).beginQuery(*realData->queryPool, realData->queryIndex, realData->queryControlFlags);
			}

			void EndQuery(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::EndQuery* realData = static_cast<const Rhi::Command::EndQuery*>(data);
				static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).endQuery(*realData->queryPool, realData->queryIndex);
			}

			void WriteTimestampQuery(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::WriteTimestampQuery* realData = static_cast<const Rhi::Command::WriteTimestampQuery*>(data);
				static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).writeTimestampQuery(*realData->queryPool, realData->queryIndex);
			}


			//[-------------------------------------------------------]
			//[ Debug                                                 ]
			//[-------------------------------------------------------]
#if SE_DEBUG
			void SetDebugMarker(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::SetDebugMarker* realData = static_cast<const Rhi::Command::SetDebugMarker*>(data);
				static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).setDebugMarker(realData->name);
			}

			void BeginDebugEvent(const void* data, Rhi::IRhi& rhi)
			{
				const Rhi::Command::BeginDebugEvent* realData = static_cast<const Rhi::Command::BeginDebugEvent*>(data);
				static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).beginDebugEvent(realData->name);
			}

			void EndDebugEvent(const void*, Rhi::IRhi& rhi)
			{
				static_cast<Direct3D12Rhi::Direct3D12Rhi&>(rhi).endDebugEvent();
			}
#else
			void SetDebugMarker(const void*, Rhi::IRhi&)
			{
				// Nothing here
			}

			void BeginDebugEvent(const void*, Rhi::IRhi&)
			{
				// Nothing here
			}

			void EndDebugEvent(const void*, Rhi::IRhi&)
			{
				// Nothing here
			}
#endif


		}


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		static constexpr Rhi::ImplementationDispatchFunction DISPATCH_FUNCTIONS[static_cast<uint8_t>(Rhi::CommandDispatchFunctionIndex::NUMBER_OF_FUNCTIONS)] =
		{
			// Command buffer
			&ImplementationDispatch::ExecuteCommandBuffer,
			// Graphics
			&ImplementationDispatch::SetGraphicsRootSignature,
			&ImplementationDispatch::SetGraphicsPipelineState,
			&ImplementationDispatch::SetGraphicsResourceGroup,
			&ImplementationDispatch::SetGraphicsVertexArray,		// Input-assembler (IA) stage
			&ImplementationDispatch::SetGraphicsViewports,			// Rasterizer (RS) stage
			&ImplementationDispatch::SetGraphicsScissorRectangles,	// Rasterizer (RS) stage
			&ImplementationDispatch::SetGraphicsRenderTarget,		// Output-merger (OM) stage
			&ImplementationDispatch::ClearGraphics,
			&ImplementationDispatch::DrawGraphics,
			&ImplementationDispatch::DrawIndexedGraphics,
			&ImplementationDispatch::DrawMeshTasks,
			// Compute
			&ImplementationDispatch::SetComputeRootSignature,
			&ImplementationDispatch::SetComputePipelineState,
			&ImplementationDispatch::SetComputeResourceGroup,
			&ImplementationDispatch::DispatchCompute,
			// Resource
			&ImplementationDispatch::SetTextureMinimumMaximumMipmapIndex,
			&ImplementationDispatch::ResolveMultisampleFramebuffer,
			&ImplementationDispatch::CopyResource,
			&ImplementationDispatch::GenerateMipmaps,
			// Query
			&ImplementationDispatch::ResetQueryPool,
			&ImplementationDispatch::BeginQuery,
			&ImplementationDispatch::EndQuery,
			&ImplementationDispatch::WriteTimestampQuery,
			// Debug
			&ImplementationDispatch::SetDebugMarker,
			&ImplementationDispatch::BeginDebugEvent,
			&ImplementationDispatch::EndDebugEvent
		};
	} // detail
}