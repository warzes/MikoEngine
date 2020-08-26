#pragma once

using namespace Microsoft::WRL;

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace Direct3D12Rhi
{
	class VertexArray;
	class RootSignature;
	class Direct3D12RuntimeLinking;
}

namespace
{
	namespace detail
	{
		static constexpr const char* HLSL_NAME = "HLSL";	// ASCII name of this shader language, always valid (do not free the memory the returned pointer is pointing to)
		static constexpr const uint32_t NUMBER_OF_BUFFERED_FRAMES = 2;

		template <typename T0, typename T1>
		inline T0 align(const T0 x, const T1 a)
		{
			return (x + (a - 1)) & (T0)(~(a - 1));
		}

		inline void updateWidthHeight(uint32_t mipmapIndex, uint32_t textureWidth, uint32_t textureHeight, uint32_t& width, uint32_t& height)
		{
			Rhi::ITexture::getMipmapSize(mipmapIndex, textureWidth, textureHeight);
			if ( width > textureWidth )
			{
				width = textureWidth;
			}
			if ( height > textureHeight )
			{
				height = textureHeight;
			}
		}

		class UploadCommandListAllocator final
		{
		public:
			UploadCommandListAllocator() = default;

			inline ID3D12GraphicsCommandList* getD3D12GraphicsCommandList() const
			{
				return mD3D12GraphicsCommandList;
			}

			inline ID3D12Resource* getD3D12ResourceUploadBuffer() const
			{
				return mD3D12ResourceUploadBuffer;
			}

			inline uint8_t* getData() const
			{
				return mData;
			}

			void create(ID3D12Device& d3d12Device)
			{
				mD3D12Device = &d3d12Device;
				[[maybe_unused]] HRESULT result = d3d12Device.CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), reinterpret_cast<void**>(&mD3D12CommandAllocator));
				SE_ASSERT(SUCCEEDED(result), "Direct3D 12 create command allocator failed")

					// Create the command list
					result = d3d12Device.CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mD3D12CommandAllocator, nullptr, IID_PPV_ARGS(&mD3D12GraphicsCommandList));
				SE_ASSERT(SUCCEEDED(result), "Direct3D 12 create command list failed")

					// Command lists are created in the recording state, but there is nothing to record yet. The main loop expects it to be closed, so close it now.
					result = mD3D12GraphicsCommandList->Close();
				SE_ASSERT(SUCCEEDED(result), "Direct3D 12 close command list failed")
			}

			void destroy()
			{
				if ( nullptr != mD3D12GraphicsCommandList )
					mD3D12GraphicsCommandList->Release();
				if ( nullptr != mD3D12CommandAllocator )
					mD3D12CommandAllocator->Release();
				if ( nullptr != mD3D12ResourceUploadBuffer )
					mD3D12ResourceUploadBuffer->Release();
			}

			void begin(uint32_t numberOfUploadBufferBytes)
			{
				SE_ASSERT(nullptr != mD3D12Device, "Invalid Direct3D 12 device")
					mD3D12CommandAllocator->Reset();
				mD3D12GraphicsCommandList->Reset(mD3D12CommandAllocator, nullptr);
				if ( numberOfUploadBufferBytes != mNumberOfUploadBufferBytes )
				{
					mNumberOfUploadBufferBytes = numberOfUploadBufferBytes;
					if ( nullptr != mD3D12ResourceUploadBuffer )
					{
						mD3D12ResourceUploadBuffer->Release();
					}
					mD3D12ResourceUploadBuffer = createBuffer(*mD3D12Device, D3D12_HEAP_TYPE_UPLOAD, numberOfUploadBufferBytes);
				}
				mOffset = 0;
				mData = nullptr;
			}

			void end()
			{
				if ( nullptr != mData )
				{
					const D3D12_RANGE d3d12Range = { 0, mOffset };
					mD3D12ResourceUploadBuffer->Unmap(0, &d3d12Range);
				}
				[[maybe_unused]] HRESULT result = mD3D12GraphicsCommandList->Close();
				SE_ASSERT(SUCCEEDED(result), "Direct3D 12 close command list failed")
			}

			uint32_t allocateUploadBuffer(uint32_t size, uint32_t alignment)
			{
				const uint32_t alignedOffset = align(mOffset, alignment);
				if ( alignedOffset + size > mNumberOfUploadBufferBytes )
				{
					// TODO(co) Reallocate
					SE_ASSERT(false, "Direct3D 12 allocate upload buffer failed")
				}
				if ( nullptr == mData )
				{
					mD3D12GpuVirtualAddress = mD3D12ResourceUploadBuffer->GetGPUVirtualAddress();
					const D3D12_RANGE d3d12Range = { 0, 0 };
					[[maybe_unused]] HRESULT result = mD3D12ResourceUploadBuffer->Map(0, &d3d12Range, reinterpret_cast<void**>(&mData));
					SE_ASSERT(SUCCEEDED(result), "Direct3D 12 map buffer failed")
				}
				mOffset = alignedOffset + size;
				return alignedOffset;
			}

		private:
			static ID3D12Resource* createBuffer(ID3D12Device& d3d12Device, D3D12_HEAP_TYPE d3dHeapType, size_t numberOfBytes)
			{
				D3D12_HEAP_PROPERTIES d3d12HeapProperties = {};
				d3d12HeapProperties.Type = d3dHeapType;

				D3D12_RESOURCE_DESC d3d12ResourceDesc = {};
				d3d12ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
				d3d12ResourceDesc.Width = numberOfBytes;
				d3d12ResourceDesc.Height = 1;
				d3d12ResourceDesc.DepthOrArraySize = 1;
				d3d12ResourceDesc.MipLevels = 1;
				d3d12ResourceDesc.SampleDesc.Count = 1;
				d3d12ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

				ID3D12Resource* d3d12Resource = nullptr;
				const D3D12_RESOURCE_STATES d3d12ResourceStates = (d3dHeapType == D3D12_HEAP_TYPE_READBACK) ? D3D12_RESOURCE_STATE_COPY_DEST : D3D12_RESOURCE_STATE_GENERIC_READ;
				[[maybe_unused]] HRESULT result = d3d12Device.CreateCommittedResource(&d3d12HeapProperties, D3D12_HEAP_FLAG_NONE, &d3d12ResourceDesc, d3d12ResourceStates, nullptr, __uuidof(ID3D12Resource), reinterpret_cast<void**>(&d3d12Resource));
				SE_ASSERT(SUCCEEDED(result), "Direct3D 12 create committed resource failed")

					return d3d12Resource;
			}

		private:
			ID3D12Device*			   mD3D12Device = nullptr;
			ID3D12CommandAllocator*	   mD3D12CommandAllocator = nullptr;
			ID3D12GraphicsCommandList* mD3D12GraphicsCommandList = nullptr;
			ID3D12Resource*			   mD3D12ResourceUploadBuffer = nullptr;
			D3D12_GPU_VIRTUAL_ADDRESS  mD3D12GpuVirtualAddress = {};
			uint8_t*				   mData = nullptr;
			uint32_t				   mOffset = 0;
			uint32_t				   mNumberOfUploadBufferBytes = 0;
		};

		struct UploadContext final
		{
		public:
			UploadContext() = default;

			inline void create(ID3D12Device& d3d12Device)
			{
				for ( uint32_t i = 0; i < NUMBER_OF_BUFFERED_FRAMES; ++i )
				{
					mUploadCommandListAllocator[i].create(d3d12Device);
				}
				begin();
			}

			inline void destroy()
			{
				for ( uint32_t i = 0; i < NUMBER_OF_BUFFERED_FRAMES; ++i )
				{
					mUploadCommandListAllocator[i].destroy();
				}
			}

			inline UploadCommandListAllocator* getUploadCommandListAllocator() const
			{
				return mCurrentUploadCommandListAllocator;
			}

			inline ID3D12GraphicsCommandList* getD3d12GraphicsCommandList() const
			{
				return mCurrentD3d12GraphicsCommandList;
			}

			void begin()
			{
				// End previous upload command list allocator
				if ( nullptr != mCurrentUploadCommandListAllocator )
				{
					mCurrentUploadCommandListAllocator->end();
					mCurrentFrameIndex = (mCurrentFrameIndex + 1) % NUMBER_OF_BUFFERED_FRAMES;
				}

				// Begin new upload command list allocator
				const uint32_t numberOfUploadBufferBytes = 1024 * 1024 * 1024;	// TODO(co) This must be a decent size with emergency reallocation if really necessary
				mCurrentUploadCommandListAllocator = &mUploadCommandListAllocator[mCurrentFrameIndex];
				mCurrentD3d12GraphicsCommandList = mCurrentUploadCommandListAllocator->getD3D12GraphicsCommandList();
				mCurrentUploadCommandListAllocator->begin(numberOfUploadBufferBytes);
			}

		private:
			UploadCommandListAllocator mUploadCommandListAllocator[NUMBER_OF_BUFFERED_FRAMES];
			// Current
			uint32_t							  mCurrentFrameIndex = 0;
			::detail::UploadCommandListAllocator* mCurrentUploadCommandListAllocator = nullptr;
			ID3D12GraphicsCommandList*			  mCurrentD3d12GraphicsCommandList = nullptr;
		};

		class DescriptorHeap final
		{
		public:
			inline DescriptorHeap(ID3D12Device& d3d12Device, D3D12_DESCRIPTOR_HEAP_TYPE d3d12DescriptorHeapType, uint16_t size, bool shaderVisible) :
				mMakeIDAllocator(size - 1u)
			{
				D3D12_DESCRIPTOR_HEAP_DESC d3d12DescriptorHeadDescription = {};
				d3d12DescriptorHeadDescription.Type = d3d12DescriptorHeapType;
				d3d12DescriptorHeadDescription.NumDescriptors = size;
				d3d12DescriptorHeadDescription.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				d3d12Device.CreateDescriptorHeap(&d3d12DescriptorHeadDescription, __uuidof(ID3D12DescriptorHeap), reinterpret_cast<void**>(&mD3D12DescriptorHeap));
				mD3D12CpuDescriptorHandleForHeapStart = mD3D12DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
				mD3D12GpuDescriptorHandleForHeapStart = mD3D12DescriptorHeap->GetGPUDescriptorHandleForHeapStart();
				mDescriptorSize = d3d12Device.GetDescriptorHandleIncrementSize(d3d12DescriptorHeapType);
			}

			inline ~DescriptorHeap()
			{
				mD3D12DescriptorHeap->Release();
			}

			inline uint16_t allocate(uint16_t count)
			{
				uint16_t index = 0;
				[[maybe_unused]] const bool result = mMakeIDAllocator.CreateRangeID(index, count);
				SE_ASSERT(result, "Direct3D 12 create range ID failed")
					return index;
			}

			inline void release(uint16_t offset, uint16_t count)
			{
				[[maybe_unused]] const bool result = mMakeIDAllocator.DestroyRangeID(offset, count);
				SE_ASSERT(result, "Direct3D 12 destroy range ID failed")
			}

			inline ID3D12DescriptorHeap* getD3D12DescriptorHeap() const
			{
				return mD3D12DescriptorHeap;
			}

			inline D3D12_CPU_DESCRIPTOR_HANDLE getD3D12CpuDescriptorHandleForHeapStart() const
			{
				return mD3D12CpuDescriptorHandleForHeapStart;
			}

			inline D3D12_CPU_DESCRIPTOR_HANDLE getOffsetD3D12CpuDescriptorHandleForHeapStart(uint16_t offset) const
			{
				D3D12_CPU_DESCRIPTOR_HANDLE d3d12CpuDescriptorHandle = mD3D12CpuDescriptorHandleForHeapStart;
				d3d12CpuDescriptorHandle.ptr += offset * mDescriptorSize;
				return d3d12CpuDescriptorHandle;
			}

			inline D3D12_GPU_DESCRIPTOR_HANDLE getD3D12GpuDescriptorHandleForHeapStart() const
			{
				return mD3D12GpuDescriptorHandleForHeapStart;
			}

			inline uint32_t getDescriptorSize() const
			{
				return mDescriptorSize;
			}

		private:
			explicit DescriptorHeap(const DescriptorHeap&) = delete;
			DescriptorHeap& operator =(const DescriptorHeap&) = delete;

			ID3D12DescriptorHeap*		mD3D12DescriptorHeap = nullptr;
			D3D12_CPU_DESCRIPTOR_HANDLE mD3D12CpuDescriptorHandleForHeapStart = {};
			D3D12_GPU_DESCRIPTOR_HANDLE mD3D12GpuDescriptorHandleForHeapStart = {};
			uint32_t					mDescriptorSize = 0;
			MakeID						mMakeIDAllocator;
		};
	} // detail
}