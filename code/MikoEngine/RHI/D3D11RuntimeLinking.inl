//[-------------------------------------------------------]
//[ AMD AGS functions                                     ]
//[-------------------------------------------------------]

#define RHI_DIRECT3D11_EXPORTS

extern "C" {
	// -> Using AMD AGS v5.3.0 - November 22, 2018
	// -> From https://github.com/GPUOpen-LibrariesAndSDKs/AGS_SDK and https://raw.githubusercontent.com/GPUOpen-LibrariesAndSDKs/Barycentrics12/master/ags_lib/inc/amd_ags.h
	// -> We're using the static linked version of AMD AGS in order to reduce the number of individual shipped files
	// -> For debug builds there are no static AMD AGS libraries available
#if SE_DEBUG
#define DYNAMIC_AMD_AGS
#endif
	struct AGSGPUInfo;
	typedef void* (__stdcall *AGS_ALLOC_CALLBACK)(size_t allocationSize);
	typedef void(__stdcall *AGS_FREE_CALLBACK)(void* allocationPtr);
	struct AGSConfiguration
	{
		AGS_ALLOC_CALLBACK allocCallback;
		AGS_FREE_CALLBACK  freeCallback;
	};
	struct AGSDX11DeviceCreationParams
	{
		IDXGIAdapter*               pAdapter;
		D3D_DRIVER_TYPE             DriverType;
		HMODULE                     Software;
		UINT                        Flags;
		const D3D_FEATURE_LEVEL*    pFeatureLevels;
		UINT                        FeatureLevels;
		UINT                        SDKVersion;
		const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc;
	};
	enum AGSCrossfireMode
	{
		AGS_CROSSFIRE_MODE_DRIVER_AFR = 0,
		AGS_CROSSFIRE_MODE_EXPLICIT_AFR,
		AGS_CROSSFIRE_MODE_DISABLE
	};
	struct AGSDX11ExtensionParams
	{
		const WCHAR*     pAppName;
		const WCHAR*     pEngineName;
		unsigned int     appVersion;
		unsigned int     engineVersion;
		unsigned int     numBreadcrumbMarkers;
		unsigned int     uavSlot;
		AGSCrossfireMode crossfireMode;
	};
	struct AGSDX11ReturnedParams
	{
		ID3D11Device*        pDevice;
		ID3D11DeviceContext* pImmediateContext;
		IDXGISwapChain*      pSwapChain;
		D3D_FEATURE_LEVEL    FeatureLevel;
		unsigned int         extensionsSupported;
		unsigned int         crossfireGPUCount;
		void*                breadcrumbBuffer;
	};
	enum AGSReturnCode
	{
		AGS_SUCCESS,
		AGS_FAILURE,
		AGS_INVALID_ARGS,
		AGS_OUT_OF_MEMORY,
		AGS_ERROR_MISSING_DLL,
		AGS_ERROR_LEGACY_DRIVER,
		AGS_EXTENSION_NOT_SUPPORTED,
		AGS_ADL_FAILURE,
		AGS_DX_FAILURE
	};
#ifdef DYNAMIC_AMD_AGS
#define FNDEF_AMD_AGS(retType, funcName, args) retType (WINAPIV *funcPtr_##funcName) args
#else
#define FNDEF_AMD_AGS(retType, funcName, args) retType WINAPIV funcName args
#endif
	FNDEF_AMD_AGS(AGSReturnCode, agsInit, (AGSContext** context, const AGSConfiguration* config, AGSGPUInfo* gpuInfo));
	FNDEF_AMD_AGS(AGSReturnCode, agsDeInit, (AGSContext* context));
	FNDEF_AMD_AGS(AGSReturnCode, agsDriverExtensionsDX11_CreateDevice, (AGSContext* context, AGSDX11DeviceCreationParams* creationParams, AGSDX11ExtensionParams* extensionParams, AGSDX11ReturnedParams* returnedParams));
	FNDEF_AMD_AGS(AGSReturnCode, agsDriverExtensionsDX11_DestroyDevice, (AGSContext* context, ID3D11Device* device, unsigned int* deviceReferences, ID3D11DeviceContext* immediateContext, unsigned int* immediateContextReferences));
	FNDEF_AMD_AGS(AGSReturnCode, agsDriverExtensionsDX11_MultiDrawInstancedIndirect, (AGSContext* context, ID3D11DeviceContext* dxContext, unsigned int drawCount, ID3D11Buffer* pBufferForArgs, unsigned int alignedByteOffsetForArgs, unsigned int byteStrideForArgs));
	FNDEF_AMD_AGS(AGSReturnCode, agsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirect, (AGSContext* context, ID3D11DeviceContext* dxContext, unsigned int drawCount, ID3D11Buffer* pBufferForArgs, unsigned int alignedByteOffsetForArgs, unsigned int byteStrideForArgs));
#ifdef DYNAMIC_AMD_AGS
#define agsInit														FNPTR(agsInit)
#define agsDeInit													FNPTR(agsDeInit)
#define agsDriverExtensionsDX11_CreateDevice						FNPTR(agsDriverExtensionsDX11_CreateDevice)
#define agsDriverExtensionsDX11_DestroyDevice						FNPTR(agsDriverExtensionsDX11_DestroyDevice)
#define agsDriverExtensionsDX11_MultiDrawInstancedIndirect			FNPTR(agsDriverExtensionsDX11_MultiDrawInstancedIndirect)
#define agsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirect	FNPTR(agsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirect)
#else
#	if SE_ARCH_64BIT
#ifdef RHI_DIRECT3D11_EXPORTS
	// Dynamic library is using "Multi-threaded DLL (/MD)"
#pragma comment(lib, "amd_ags_x64_2017_MD.lib")
#else
	// Static library is using "Multi-threaded (/MT)"
#pragma comment(lib, "amd_ags_x64_2017_MT.lib")
#endif
#	else
#ifdef RHI_DIRECT3D11_EXPORTS
	// Dynamic library is using "Multi-threaded DLL (/MD)"
#pragma comment(lib, "amd_ags_x86_2017_MD.lib")
#else
	// Static library is using "Multi-threaded (/MT)"
#pragma comment(lib, "amd_ags_x86_2017_MT.lib")
#endif
#endif
#endif
	[[nodiscard]] void* __stdcall AmdAgsAllocCallback(size_t allocationSize)
	{
		return DefaultAllocator::Reallocate(nullptr, allocationSize, 1);
	}
	void __stdcall AmdAgsFreeCallback(void* allocationPtr)
	{
		DefaultAllocator::Reallocate(allocationPtr, 0, 1);
	}
}

//[-------------------------------------------------------]
//[ NvAPI functions                                       ]
//[-------------------------------------------------------]
// From https://developer.nvidia.com/nvapi and http://developer.download.nvidia.com/NVAPI/PG-5116-001_v01_public.pdf
#define FNDEF_NvAPI(retType, funcName, args) retType (WINAPIV *funcPtr_##funcName) args
typedef int NvAPI_Status;
typedef unsigned int NvU32;
FNDEF_NvAPI(NvAPI_Status, NvAPI_Initialize, ());
FNDEF_NvAPI(NvAPI_Status, NvAPI_Unload, ());
FNDEF_NvAPI(NvAPI_Status, NvAPI_D3D11_MultiDrawInstancedIndirect, (__in ID3D11DeviceContext* pDevContext11, __in NvU32 drawCount, __in ID3D11Buffer* pBuffer, __in NvU32 alignedByteOffsetForArgs, __in NvU32 alignedByteStrideForArgs));
FNDEF_NvAPI(NvAPI_Status, NvAPI_D3D11_MultiDrawIndexedInstancedIndirect, (__in ID3D11DeviceContext* pDevContext11, __in NvU32 drawCount, __in ID3D11Buffer* pBuffer, __in NvU32 alignedByteOffsetForArgs, __in NvU32 alignedByteStrideForArgs));
#define NvAPI_Initialize								FNPTR(NvAPI_Initialize)
#define NvAPI_Unload									FNPTR(NvAPI_Unload)
#define NvAPI_D3D11_MultiDrawInstancedIndirect			FNPTR(NvAPI_D3D11_MultiDrawInstancedIndirect)
#define NvAPI_D3D11_MultiDrawIndexedInstancedIndirect	FNPTR(NvAPI_D3D11_MultiDrawIndexedInstancedIndirect)

namespace Direct3D11Rhi
{
#ifndef FNPTR
#define FNPTR(name) funcPtr_##name
#endif

	class Direct3D11RuntimeLinking final
	{
	public:
		inline explicit Direct3D11RuntimeLinking(Direct3D11Rhi& direct3D11Rhi) :
			mDirect3D11Rhi(direct3D11Rhi),
#ifdef DYNAMIC_AMD_AGS
			mAmdAgsSharedLibrary(nullptr),
#endif
			mAgsContext(nullptr),
			mNvAPISharedLibrary(nullptr),
			mInitialized(false)
		{
		}

		~Direct3D11RuntimeLinking()
		{
#ifdef DYNAMIC_AMD_AGS
			if ( nullptr != mAmdAgsSharedLibrary )
			{
				if ( nullptr != agsDeInit && AGS_SUCCESS != agsDeInit(mAgsContext) )
				{
					RHI_LOG(CRITICAL, "Direct3D 11: Failed to unload AMG AGS");
				}
				::FreeLibrary(static_cast<HMODULE>(mAmdAgsSharedLibrary));
			}
#else
			if ( AGS_SUCCESS != agsDeInit(mAgsContext) )
			{
				RHI_LOG(CRITICAL, "Direct3D 11: Failed to unload AMG AGS");
			}
#endif
			if ( nullptr != mNvAPISharedLibrary )
			{
				if ( nullptr != NvAPI_Unload && 0 != NvAPI_Unload() )
				{
					RHI_LOG(CRITICAL, "Direct3D 11: Failed to unload NvAPI");
				}
				::FreeLibrary(static_cast<HMODULE>(mNvAPISharedLibrary));
			}
		}

		[[nodiscard]] bool isDirect3D11Avaiable()
		{
			// Already initialized?
			if ( !mInitialized )
			{
				// We're now initialized
				mInitialized = true;

				// AMD AGS and NvAPI for e.g. multi-draw-indirect support

					// Check whether or not the primary DXGI adapter is an AMD GPU
				bool amdDxgiAdapter = false;
				bool nvidiaDxgiAdapter = false;
				{
					// Get the primary DXGI adapter
					IDXGIFactory* dxgiFactory = nullptr;
					FAILED_DEBUG_BREAK(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&dxgiFactory))
						if ( nullptr == dxgiFactory )
						{
							// Error!
							return false;
						}
					IDXGIAdapter* dxgiAdapter = nullptr;
					FAILED_DEBUG_BREAK(dxgiFactory->EnumAdapters(0, &dxgiAdapter))
						DXGI_ADAPTER_DESC dxgiAdapterDesc = {};
					FAILED_DEBUG_BREAK(dxgiAdapter->GetDesc(&dxgiAdapterDesc));
					if ( 0x1414 == dxgiAdapterDesc.VendorId )	// 0x1414 = "Capture Adapter" when using Visual Studio graphics debugger
					{
						RHI_LOG(COMPATIBILITY_WARNING, "Direct3D 11 capture adapter used (e.g. Visual Studio graphics debugger), AMD AGS and NvAPI support disabled")
					}
					else
					{
						amdDxgiAdapter = (0x1002 == dxgiAdapterDesc.VendorId);		// 0x1002 -> See "How-To Identify the Manufacturer and Model of an AMD Graphics Card" at http://support.amd.com/en-us/kb-articles/Pages/HowtoidentifythemodelofanATIgraphicscard.aspx
						nvidiaDxgiAdapter = (0x10DE == dxgiAdapterDesc.VendorId);	// 0x10DE -> See "Device IDs" at http://www.nvidia.com/object/device_ids.html
					}
					dxgiAdapter->Release();
					dxgiFactory->Release();
				}

				// Optional vendor specific part: AMD AGS
				if ( amdDxgiAdapter )
				{
#ifdef DYNAMIC_AMD_AGS
#if SE_ARCH_64BIT
					static constexpr const char* AMD_AGS_SHARED_LIBRARY_NAME = "amd_ags_x64.dll";
#else
					static constexpr const char* AMD_AGS_SHARED_LIBRARY_NAME = "amd_ags_x86.dll";
#endif
					mAmdAgsSharedLibrary = ::LoadLibraryExA(AMD_AGS_SHARED_LIBRARY_NAME, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
					if ( nullptr != mAmdAgsSharedLibrary )
					{
						if ( !loadAmdAgsEntryPoints() )
						{
							RHI_LOG(CRITICAL, "Direct3D 11: Failed to load AMD AGS function entry points")
								::FreeLibrary(static_cast<HMODULE>(mAmdAgsSharedLibrary));
							mAmdAgsSharedLibrary = nullptr;
							agsInit = nullptr;
							agsDeInit = nullptr;
							agsDriverExtensionsDX11_CreateDevice = nullptr;
							agsDriverExtensionsDX11_DestroyDevice = nullptr;
							agsDriverExtensionsDX11_MultiDrawInstancedIndirect = nullptr;
							agsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirect = nullptr;
						}
					}
					else
					{
						RHI_LOG(PERFORMANCE_WARNING, "Direct3D 11: Failed to load the AMD AGS shared library \"%s\"", AMD_AGS_SHARED_LIBRARY_NAME)
					}
#else
					{
						// Initialize AMD AGS (e.g. for multi-indirect-draw support)
						const AGSConfiguration agsConfiguration = { &AmdAgsAllocCallback, &AmdAgsFreeCallback };
						if ( AGS_SUCCESS == agsInit(&mAgsContext, &agsConfiguration, nullptr) )
						{
							RHI_LOG(TRACE, "Direct3D 11: Successfully initialized AMD AGS")
						}
						else
						{
							RHI_LOG(CRITICAL, "Direct3D 11: Failed to initialize AMD AGS")
						}
					}
#endif
				}

				// Optional vendor specific part: NvAPI
				if ( nvidiaDxgiAdapter )
				{
#if SE_ARCH_64BIT
					static constexpr const char* NVAPI_SHARED_LIBRARY_NAME = "nvapi64.dll";
#else
					static constexpr const char* NVAPI_SHARED_LIBRARY_NAME = "nvapi.dll";
#endif
					mNvAPISharedLibrary = ::LoadLibraryExA(NVAPI_SHARED_LIBRARY_NAME, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
					if ( nullptr != mNvAPISharedLibrary )
					{
						if ( !loadNvAPIEntryPoints() )
						{
							RHI_LOG(PERFORMANCE_WARNING, "Direct3D 11: Failed to load NvAPI function entry points, maybe a graphics debugger like RenderDoc disabled NvAPI")
								::FreeLibrary(static_cast<HMODULE>(mNvAPISharedLibrary));
							mNvAPISharedLibrary = nullptr;
							NvAPI_Initialize = nullptr;
							NvAPI_Unload = nullptr;
							NvAPI_D3D11_MultiDrawInstancedIndirect = nullptr;
							NvAPI_D3D11_MultiDrawIndexedInstancedIndirect = nullptr;
						}
					}
					else
					{
						RHI_LOG(PERFORMANCE_WARNING, "Direct3D 11: Failed to load the NvAPI shared library \"%s\"", NVAPI_SHARED_LIBRARY_NAME)
					}
				}
			}

			// Entry points successfully registered?
			return true;
		}

		/**
		*  @brief
		*    Return the AMD AGS instance
		*
		*  @return
		*    The AMD AGS instance, can be a null pointer, don't destroy the returned instance
		*/
		[[nodiscard]] inline AGSContext* getAgsContext() const
		{
			return mAgsContext;
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
	private:
		explicit Direct3D11RuntimeLinking(const Direct3D11RuntimeLinking&) = delete;
		Direct3D11RuntimeLinking& operator =(const Direct3D11RuntimeLinking&) = delete;

#ifdef DYNAMIC_AMD_AGS
		[[nodiscard]] bool loadAmdAgsEntryPoints()
		{
			bool result = true;	// Success by default

			// Define a helper macro
#define IMPORT_FUNC(funcName)																																				\
					if (result)																																								\
					{																																										\
						void* symbol = ::GetProcAddress(static_cast<HMODULE>(mAmdAgsSharedLibrary), #funcName);																				\
						if (nullptr != symbol)																																				\
						{																																									\
							*(reinterpret_cast<void**>(&(funcName))) = symbol;																												\
						}																																									\
						else																																								\
						{																																									\
							wchar_t moduleFilename[MAX_PATH];																																\
							moduleFilename[0] = '\0';																																		\
							::GetModuleFileNameW(static_cast<HMODULE>(mAmdAgsSharedLibrary), moduleFilename, MAX_PATH);																		\
							RHI_LOG(CRITICAL, "Failed to locate the entry point \"%s\" within the AMD AGS shared library \"%s\"", #funcName, moduleFilename)	\
							result = false;																																					\
						}																																									\
					}

				// Load the entry points
			IMPORT_FUNC(agsInit);
			IMPORT_FUNC(agsDeInit);
			IMPORT_FUNC(agsDriverExtensionsDX11_CreateDevice);
			IMPORT_FUNC(agsDriverExtensionsDX11_DestroyDevice);
			IMPORT_FUNC(agsDriverExtensionsDX11_MultiDrawInstancedIndirect);
			IMPORT_FUNC(agsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirect);

			// Undefine the helper macro
#undef IMPORT_FUNC

			// Initialize AMD AGS (e.g. for multi-indirect-draw support)
			if ( nullptr != agsInit )
			{
				const AGSConfiguration agsConfiguration = { &AmdAgsAllocCallback, &AmdAgsFreeCallback };
				if ( AGS_SUCCESS == agsInit(&mAgsContext, &agsConfiguration, nullptr) )
				{
					RHI_LOG(TRACE, "Direct3D 11: Successfully initialized AMD AGS");
				}
				else
				{
					RHI_LOG(CRITICAL, "Direct3D 11: Failed to initialize AMD AGS");
					result = false;
				}
			}

			// Done
			return result;
		}
#endif

		/**
		*  @brief
		*    Load the NvAPI entry points
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		[[nodiscard]] bool loadNvAPIEntryPoints()
		{
			bool result = true;	// Success by default

			// Define a helper macro
#define IMPORT_FUNC(funcName)																																		\
				void* symbol = ::GetProcAddress(static_cast<HMODULE>(mNvAPISharedLibrary), #funcName);																			\
				if (nullptr != symbol)																																			\
				{																																								\
					*(reinterpret_cast<void**>(&(funcName))) = symbol;																											\
				}																																								\
				else																																							\
				{																																								\
					wchar_t moduleFilename[MAX_PATH];																															\
					moduleFilename[0] = '\0';																																	\
					::GetModuleFileNameW(static_cast<HMODULE>(mNvAPISharedLibrary), moduleFilename, MAX_PATH);																	\
					RHI_LOG(CRITICAL, "Failed to locate the entry point \"%s\" within the NvAPI shared library \"%s\"", #funcName, moduleFilename)	\
					result = false;																																				\
				}
			FNDEF_NvAPI(void*, nvapi_QueryInterface, (unsigned int offset));
#define nvapi_QueryInterface	FNPTR(nvapi_QueryInterface)
#define IMPORT_NVAPI_FUNC(funcName, ID) \
				*(reinterpret_cast<void**>(&(funcName))) = nvapi_QueryInterface(ID); \
				if (nullptr == funcName) \
				{ \
					result = false; \
				}

			// Load the entry points
			nvapi_QueryInterface = nullptr;
			IMPORT_FUNC(nvapi_QueryInterface)

				// Query function pointers
				if ( nullptr != nvapi_QueryInterface )
				{
					IMPORT_NVAPI_FUNC(NvAPI_Initialize, 0x150E828UL);
					IMPORT_NVAPI_FUNC(NvAPI_Unload, 0xD22BDD7EUL);
					IMPORT_NVAPI_FUNC(NvAPI_D3D11_MultiDrawInstancedIndirect, 0xD4E26BBF);
					IMPORT_NVAPI_FUNC(NvAPI_D3D11_MultiDrawIndexedInstancedIndirect, 0x59E890F9);
				}

			// Undefine the helper macro
#undef IMPORT_NVAPI_FUNC
#undef NvAPI_QueryInterface
#undef IMPORT_FUNC

// Initialize NvAPI (e.g. for multi-indirect-draw support)
			if ( result )
			{
				if ( nullptr != NvAPI_Initialize && 0 == NvAPI_Initialize() )
				{
					RHI_LOG(TRACE, "Direct3D 11: Successfully initialized NvAPI")
				}
				else
				{
					RHI_LOG(CRITICAL, "Direct3D 11: Failed to initialize NvAPI")
						result = false;
				}
			}

			// Done
			return result;
		}

	private:
		Direct3D11Rhi&	mDirect3D11Rhi;				// Owner Direct3D 11 RHI instance
#ifdef DYNAMIC_AMD_AGS
		void*		mAmdAgsSharedLibrary;		// AMD AGS shared library, can be a null pointer
#endif
		AGSContext*		mAgsContext;				// AMD AGS context, can be a null pointer
		void*			mNvAPISharedLibrary;		// NvAPI shared library, can be a null pointer
		bool			mInitialized;				// Already initialized?
	};
} // Direct3D11Rhi