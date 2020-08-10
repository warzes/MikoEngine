//[-------------------------------------------------------]
//[ Direct3D12Rhi/Direct3D12RuntimeLinking.h              ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Macros & definitions                                  ]
//[-------------------------------------------------------]
// Redirect D3D12* function calls to funcPtr_D3D12*
#ifndef FNPTR
#define FNPTR(name) funcPtr_##name
#endif

//[-------------------------------------------------------]
//[ DXGI core functions                                   ]
//[-------------------------------------------------------]
#define FNDEF_DXGI(retType, funcName, args) retType (WINAPI *funcPtr_##funcName) args
FNDEF_DXGI(HRESULT, CreateDXGIFactory1, (REFIID riid, _COM_Outptr_ void **ppFactory));
#define CreateDXGIFactory1	FNPTR(CreateDXGIFactory1)

//[-------------------------------------------------------]
//[ D3D12 core functions                                  ]
//[-------------------------------------------------------]
#define FNDEF_D3D12(retType, funcName, args) retType (WINAPI *funcPtr_##funcName) args
FNDEF_D3D12(HRESULT, D3D12CreateDevice, (_In_opt_ IUnknown* pAdapter, D3D_FEATURE_LEVEL MinimumFeatureLevel, _In_ REFIID riid, _COM_Outptr_opt_ void** ppDevice));
FNDEF_D3D12(HRESULT, D3D12SerializeRootSignature, (_In_ const D3D12_ROOT_SIGNATURE_DESC* pRootSignature, _In_ D3D_ROOT_SIGNATURE_VERSION Version, _Out_ ID3DBlob** ppBlob, _Always_(_Outptr_opt_result_maybenull_) ID3DBlob** ppErrorBlob));
#define D3D12CreateDevice			FNPTR(D3D12CreateDevice)
#define D3D12SerializeRootSignature	FNPTR(D3D12SerializeRootSignature)
#if SE_DEBUG
FNDEF_D3D12(HRESULT, D3D12GetDebugInterface, (_In_ REFIID riid, _COM_Outptr_opt_ void** ppvDebug));
#define D3D12GetDebugInterface	FNPTR(D3D12GetDebugInterface)
#endif

//[-------------------------------------------------------]
//[ D3DCompiler functions                                 ]
//[-------------------------------------------------------]
#define FNDEF_D3DX12(retType, funcName, args) retType (WINAPI *funcPtr_##funcName) args
typedef __interface ID3D10Blob *LPD3D10BLOB;	// "__interface" is no keyword of the ISO C++ standard, shouldn't be a problem because this in here is Microsoft Windows only and it's also within the Direct3D headers we have to use
typedef ID3D10Blob ID3DBlob;
FNDEF_D3DX12(HRESULT, D3DCompile, (LPCVOID, SIZE_T, LPCSTR, CONST D3D_SHADER_MACRO*, ID3DInclude*, LPCSTR, LPCSTR, UINT, UINT, ID3DBlob**, ID3DBlob**));
FNDEF_D3DX12(HRESULT, D3DCreateBlob, (SIZE_T Size, ID3DBlob** ppBlob));
#define D3DCompile		FNPTR(D3DCompile)
#define D3DCreateBlob	FNPTR(D3DCreateBlob)

class Direct3D12RuntimeLinking final
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
	inline explicit Direct3D12RuntimeLinking(Direct3D12Rhi& direct3D12Rhi) :
		mDirect3D12Rhi(direct3D12Rhi),
		mDxgiSharedLibrary(nullptr),
		mD3D12SharedLibrary(nullptr),
		mD3DCompilerSharedLibrary(nullptr),
		mEntryPointsRegistered(false),
		mInitialized(false)
	{
	}

	/**
	*  @brief
	*    Destructor
	*/
	~Direct3D12RuntimeLinking()
	{
		// Destroy the shared library instances
		if ( nullptr != mDxgiSharedLibrary )
		{
			::FreeLibrary(static_cast<HMODULE>(mDxgiSharedLibrary));
		}
		if ( nullptr != mD3D12SharedLibrary )
		{
			::FreeLibrary(static_cast<HMODULE>(mD3D12SharedLibrary));
		}
		if ( nullptr != mD3DCompilerSharedLibrary )
		{
			::FreeLibrary(static_cast<HMODULE>(mD3DCompilerSharedLibrary));
		}
	}

	/**
	*  @brief
	*    Return whether or not Direct3D 12 is available
	*
	*  @return
	*    "true" if Direct3D 12 is available, else "false"
	*/
	[[nodiscard]] bool isDirect3D12Avaiable()
	{
		// Already initialized?
		if ( !mInitialized )
		{
			// We're now initialized
			mInitialized = true;

			// Load the shared libraries
			if ( loadSharedLibraries() )
			{
				// Load the DXGI, D3D12 and D3DCompiler entry points
				mEntryPointsRegistered = (loadDxgiEntryPoints() && loadD3D12EntryPoints() && loadD3DCompilerEntryPoints());
			}
		}

		// Entry points successfully registered?
		return mEntryPointsRegistered;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
private:
	explicit Direct3D12RuntimeLinking(const Direct3D12RuntimeLinking& source) = delete;
	Direct3D12RuntimeLinking& operator =(const Direct3D12RuntimeLinking& source) = delete;

	/**
	*  @brief
	*    Load the shared libraries
	*
	*  @return
	*    "true" if all went fine, else "false"
	*/
	[[nodiscard]] bool loadSharedLibraries()
	{
		// Load the shared library
		mDxgiSharedLibrary = ::LoadLibraryExA("dxgi.dll", nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
		if ( nullptr != mDxgiSharedLibrary )
		{
			mD3D12SharedLibrary = ::LoadLibraryExA("d3d12.dll", nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
			if ( nullptr != mD3D12SharedLibrary )
			{
				mD3DCompilerSharedLibrary = ::LoadLibraryExA("D3DCompiler_47.dll", nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
				if ( nullptr == mD3DCompilerSharedLibrary )
				{
					RHI_LOG(CRITICAL, "Failed to load in the shared Direct3D 12 library \"D3DCompiler_47.dll\"")
				}
			}
			else
			{
				RHI_LOG(CRITICAL, "Failed to load in the shared Direct3D 12 library \"d3d12.dll\"")
			}
		}
		else
		{
			RHI_LOG(CRITICAL, "Failed to load in the shared Direct3D 12 library \"dxgi.dll\"")
		}

		// Done
		return (nullptr != mDxgiSharedLibrary && nullptr != mD3D12SharedLibrary && nullptr != mD3DCompilerSharedLibrary);
	}

	/**
	*  @brief
	*    Load the DXGI entry points
	*
	*  @return
	*    "true" if all went fine, else "false"
	*/
	[[nodiscard]] bool loadDxgiEntryPoints()
	{
		bool result = true;	// Success by default

		// Define a helper macro
#define IMPORT_FUNC(funcName)																																						\
				if (result)																																										\
				{																																												\
					void* symbol = ::GetProcAddress(static_cast<HMODULE>(mDxgiSharedLibrary), #funcName);																						\
					if (nullptr != symbol)																																						\
					{																																											\
						*(reinterpret_cast<void**>(&(funcName))) = symbol;																														\
					}																																											\
					else																																										\
					{																																											\
						wchar_t moduleFilename[MAX_PATH];																																		\
						moduleFilename[0] = '\0';																																				\
						::GetModuleFileNameW(static_cast<HMODULE>(mDxgiSharedLibrary), moduleFilename, MAX_PATH);																				\
						RHI_LOG(CRITICAL, "Failed to locate the entry point \"%s\" within the Direct3D 12 DXGI shared library \"%s\"", #funcName, moduleFilename)	\
						result = false;																																							\
					}																																											\
				}

			// Load the entry points
		IMPORT_FUNC(CreateDXGIFactory1)

			// Undefine the helper macro
#undef IMPORT_FUNC

// Done
return result;
	}

	/**
	*  @brief
	*    Load the D3D12 entry points
	*
	*  @return
	*    "true" if all went fine, else "false"
	*/
	[[nodiscard]] bool loadD3D12EntryPoints()
	{
		bool result = true;	// Success by default

		// Define a helper macro
#define IMPORT_FUNC(funcName)																																					\
				if (result)																																									\
				{																																											\
					void* symbol = ::GetProcAddress(static_cast<HMODULE>(mD3D12SharedLibrary), #funcName);																					\
					if (nullptr != symbol)																																					\
					{																																										\
						*(reinterpret_cast<void**>(&(funcName))) = symbol;																													\
					}																																										\
					else																																									\
					{																																										\
						wchar_t moduleFilename[MAX_PATH];																																	\
						moduleFilename[0] = '\0';																																			\
						::GetModuleFileNameW(static_cast<HMODULE>(mD3D12SharedLibrary), moduleFilename, MAX_PATH);																			\
						RHI_LOG(CRITICAL, "Failed to locate the entry point \"%s\" within the Direct3D 12 shared library \"%s\"", #funcName, moduleFilename)	\
						result = false;																																						\
					}																																										\
				}

			// Load the entry points
		IMPORT_FUNC(D3D12CreateDevice)
			IMPORT_FUNC(D3D12SerializeRootSignature)
#if SE_DEBUG
			IMPORT_FUNC(D3D12GetDebugInterface)
#endif

			// Undefine the helper macro
#undef IMPORT_FUNC

// Done
return result;
	}

	/**
	*  @brief
	*    Load the D3DCompiler entry points
	*
	*  @return
	*    "true" if all went fine, else "false"
	*/
	[[nodiscard]] bool loadD3DCompilerEntryPoints()
	{
		bool result = true;	// Success by default

		// Define a helper macro
#define IMPORT_FUNC(funcName)																																					\
				if (result)																																									\
				{																																											\
					void* symbol = ::GetProcAddress(static_cast<HMODULE>(mD3DCompilerSharedLibrary), #funcName);																			\
					if (nullptr != symbol)																																					\
					{																																										\
						*(reinterpret_cast<void**>(&(funcName))) = symbol;																													\
					}																																										\
					else																																									\
					{																																										\
						wchar_t moduleFilename[MAX_PATH];																																	\
						moduleFilename[0] = '\0';																																			\
						::GetModuleFileNameW(static_cast<HMODULE>(mD3DCompilerSharedLibrary), moduleFilename, MAX_PATH);																	\
						RHI_LOG(CRITICAL, "Failed to locate the entry point \"%s\" within the Direct3D 12 shared library \"%s\"", #funcName, moduleFilename)	\
						result = false;																																						\
					}																																										\
				}

			// Load the entry points
		IMPORT_FUNC(D3DCompile)
			IMPORT_FUNC(D3DCreateBlob)

			// Undefine the helper macro
#undef IMPORT_FUNC

// Done
return result;
	}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
private:
	Direct3D12Rhi&	mDirect3D12Rhi;				///< Owner Direct3D 12 RHI instance
	void*			mDxgiSharedLibrary;			///< DXGI shared library, can be a null pointer
	void*			mD3D12SharedLibrary;		///< D3D12 shared library, can be a null pointer
	void*			mD3DCompilerSharedLibrary;	///< D3DCompiler shared library, can be a null pointer
	bool			mEntryPointsRegistered;		///< Entry points successfully registered?
	bool			mInitialized;				///< Already initialized?
};