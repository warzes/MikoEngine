/** Amalgamated/unity build rendering hardware interface (RHI)*/
#pragma once

#include "DefaultAssert.h"
#include "DefaultLog.h"

#if SE_DEBUG
	#include <cassert>
	#define ASSERT(expression, message) assert((expression) && message);	// TODO(co) "RHI_ASSERT()" should be used everywhere

	/**
	*  @brief
	*    Resource name for debugging purposes, ignored when not using "SE_DEBUG"
	*
	*  @param[in] debugName
	*    ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
	*/
	#define RHI_RESOURCE_DEBUG_NAME_PARAMETER , const char debugName[] = ""
	#define RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT , const char debugName[]

	/**
	*  @brief
	*    Pass resource name for debugging purposes, ignored when not using "SE_DEBUG"
	*/
	#define RHI_RESOURCE_DEBUG_PASS_PARAMETER , debugName
#else
	#define ASSERT(expression, message)	// TODO(co) "RHI_ASSERT()" should be used everywhere

	/**
	*  @brief
	*    Resource name for debugging purposes, ignored when not using "SE_DEBUG"
	*
	*  @param[in] debugName
	*    ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
	*/
	#define RHI_RESOURCE_DEBUG_NAME_PARAMETER
	#define RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT

	/**
	*  @brief
	*    Pass resource name for debugging purposes, ignored when not using "SE_DEBUG"
	*/
	#define RHI_RESOURCE_DEBUG_PASS_PARAMETER
#endif

namespace Rhi
{
	class ILog;
	class IAssert;
	class Context;
	class IRhi;
	class IAllocator;
	class IShaderLanguage;
	class IResource;
		class IRootSignature;
		class IResourceGroup;
		class IGraphicsProgram;
		class IVertexArray;
		class IRenderPass;
		class IQueryPool;
		class IRenderTarget;
			class IRenderWindow;
			class ISwapChain;
			class IFramebuffer;
			struct FramebufferAttachment;
		class CommandBuffer;
		class IBufferManager;
		class IBuffer;
			class IVertexBuffer;
			class IIndexBuffer;
			class ITextureBuffer;
			class IStructuredBuffer;
			class IIndirectBuffer;
			class IUniformBuffer;
		class ITextureManager;
		class ITexture;
			class ITexture1D;
			class ITexture1DArray;
			class ITexture2D;
			class ITexture2DArray;
			class ITexture3D;
			class ITextureCube;
			class ITextureCubeArray;
		class IState;
			class IPipelineState;
				class IGraphicsPipelineState;
				class IComputePipelineState;
			class ISamplerState;
		class IShader;
			class IVertexShader;
			class ITessellationControlShader;
			class ITessellationEvaluationShader;
			class IGeometryShader;
			class IFragmentShader;
			class ITaskShader;
			class IMeshShader;
			class IComputeShader;
}

namespace Rhi
{
	//[-------------------------------------------------------]
	//[ Rhi/Context.h                                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Context class encapsulating all embedding related wirings
	*/
	class Context
	{

	// Public definitions
	public:
		enum class ContextType
		{
			WINDOWS,
			X11,
			WAYLAND
		};

	// Public methods
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] log
		*    Log instance to use, the log instance must stay valid as long as the RHI instance exists
		*  @param[in] allocator
		*    Allocator instance to use, the allocator instance must stay valid as long as the RHI instance exists
		*  @param[in] nativeWindowHandle
		*    Native window handle
		*  @param[in] useExternalContext
		*    Indicates if an external RHI context is used; in this case the RHI itself has nothing to do with the creation/managing of an RHI context
		*  @param[in] contextType
		*    The type of the context
		*/
		inline Context(IAllocator& allocator, handle nativeWindowHandle = 0, bool useExternalContext = false, ContextType contextType = Context::ContextType::WINDOWS) :
			mAllocator(allocator),
			mNativeWindowHandle(nativeWindowHandle),
			mUseExternalContext(useExternalContext),
			mContextType(contextType),
			mRhiApiSharedLibrary(nullptr)
		{}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Context()
		{}

		/**
		*  @brief
		*    Return the allocator instance
		*
		*  @return
		*    The allocator instance
		*/
		[[nodiscard]] inline IAllocator& getAllocator() const
		{
			return mAllocator;
		}

		/**
		*  @brief
		*    Return the native window handle
		*
		*  @return
		*    The native window handle
		*/
		[[nodiscard]] inline handle getNativeWindowHandle() const
		{
			return mNativeWindowHandle;
		}

		/**
		*  @brief
		*    Return whether or not an external context is used
		*
		*  @return
		*    "true" if an external context is used, else "false"
		*/
		[[nodiscard]] inline bool isUsingExternalContext() const
		{
			return mUseExternalContext;
		}

		/**
		*  @brief
		*    Return the type of the context
		*
		*  @return
		*    The context type
		*/
		[[nodiscard]] inline ContextType getType() const
		{
			return mContextType;
		}

		/**
		*  @brief
		*    Return a handle to the RHI API shared library
		*
		*  @return
		*    The handle to the RHI API shared library, can be a null pointer
		*/
		[[nodiscard]] inline void* getRhiApiSharedLibrary() const
		{
			return mRhiApiSharedLibrary;
		}

		/**
		*  @brief
		*    Set the handle for the RHI API shared library to use instead of let it load by the RHI instance
		*
		*  @param[in] rhiApiSharedLibrary
		*    A handle to the RHI API shared library, can be a null pointer; the RHI will use this handle instead of loading the RHI API shared library itself
		*/
		inline void setRhiApiSharedLibrary(void* rhiApiSharedLibrary)
		{
			mRhiApiSharedLibrary = rhiApiSharedLibrary;
		}

	// Private methods
	private:
		explicit Context(const Context&) = delete;
		Context& operator=(const Context&) = delete;

	// Private data
	private:
		IAllocator&	mAllocator;
		handle		mNativeWindowHandle;
		bool		mUseExternalContext;
		ContextType	mContextType;
		void*		mRhiApiSharedLibrary;	///< A handle to the RHI API shared library (e.g. obtained via "dlopen()" and co), can be a null pointer

	};

	#ifdef LINUX
		class X11Context final : public Context
		{
		public:
			inline X11Context(IAllocator& allocator, _XDisplay* display, handle nativeWindowHandle = 0, bool useExternalContext = false) :
				Context(log, assert, allocator, nativeWindowHandle, useExternalContext, Context::ContextType::X11),
				mDisplay(display)
			{}

			[[nodiscard]] inline _XDisplay* getDisplay() const
			{
				return mDisplay;
			}
		private:
			_XDisplay* mDisplay;
		};

		class WaylandContext final : public Context
		{
		public:			
			inline WaylandContext(IAllocator& allocator, wl_display* display, wl_surface* surface = 0, bool useExternalContext = false) :
				Context(log, assert, allocator, 1, useExternalContext, Context::ContextType::WAYLAND),	// Under Wayland the surface (aka window) handle is not an integer, but the RHI implementation expects an integer as window handle so we give here an value != 0 so that a swap chain is created
				mDisplay(display),
				mSurface(surface)
			{}

			[[nodiscard]] inline wl_display* getDisplay() const
			{
				return mDisplay;
			}

			[[nodiscard]] inline wl_surface* getSurface() const
			{
				return mSurface;
			}
		private:
			wl_display* mDisplay;
			wl_surface* mSurface;
		};
	#endif

	//[-------------------------------------------------------]
	//[ Rhi/IAllocator.h                                      ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract memory allocator interface
	*
	*  @note
	*    - The implementation must be multithreading safe since the RHI is allowed to internally use multiple threads
	*    - The interface design is basing on "Nicholas Frechette's Blog Raw bits" - "A memory allocator interface" - http://nfrechette.github.io/2015/05/11/memory_allocator_interface/
	*/
	class IAllocator
	{

	// Public static methods
	public:
		template<typename Type>
		[[nodiscard]] static inline Type* constructN(Type* basePointer, size_t count)
		{
			for (size_t i = 0; i < count; ++i)
			{
				new ((void*)(basePointer + i)) Type();
			}
			return basePointer;
		}

	// Public methods
	public:
		/**
		*  @brief
		*    Reallocate
		*
		*  @param[in] oldPointer
		*    Old pointer, can be a null pointer
		*  @param[in] oldNumberOfBytes
		*    Old number of bytes, must be zero of the old pointer is a null pointer, else can be zero if this information isn't available
		*  @param[in] newNumberOfBytes
		*    New number of bytes
		*  @param[in] alignment
		*    Alignment
		*
		*  @return
		*    New pointer, can be a null pointer
		*/
		inline void* reallocate(void* oldPointer, size_t oldNumberOfBytes, size_t newNumberOfBytes, size_t alignment)
		{
			ASSERT(nullptr != mReallocateFuntion, "Invalid reallocate function")
			ASSERT(nullptr != oldPointer || 0 == oldNumberOfBytes, "Invalid old pointer")
			return (*mReallocateFuntion)(*this, oldPointer, oldNumberOfBytes, newNumberOfBytes, alignment);
		}

	// Protected definitions
	protected:
		typedef void* (*ReallocateFuntion)(IAllocator&, void*, size_t, size_t, size_t);

	// Protected methods
	protected:
		inline explicit IAllocator(ReallocateFuntion reallocateFuntion) :
			mReallocateFuntion(reallocateFuntion)
		{
			ASSERT(nullptr != mReallocateFuntion, "Invalid reallocate function")
		}

		inline virtual ~IAllocator()
		{}

		explicit IAllocator(const IAllocator&) = delete;
		IAllocator& operator=(const IAllocator&) = delete;

	// Private methods
	private:
		ReallocateFuntion mReallocateFuntion;

	};

	// Macros & definitions

	// Malloc and free
	#define RHI_MALLOC(context, newNumberOfBytes) (context).getAllocator().reallocate(nullptr, 0, newNumberOfBytes, 1)
	#define RHI_MALLOC_TYPED(context, type, newNumberOfElements) reinterpret_cast<type*>((context).getAllocator().reallocate(nullptr, 0, sizeof(type) * (newNumberOfElements), 1))
	#define RHI_FREE(context, oldPointer) (context).getAllocator().reallocate(oldPointer, 0, 0, 1)

	// New and delete
	// - Using placement new and explicit destructor call
	// - See http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/ - "2.  Wrap your macros in do { � } while(0)." for background information about the do-while wrap
	#define RHI_NEW(context, type) new ((context).getAllocator().reallocate(nullptr, 0, sizeof(type), 1)) type
	#define RHI_DELETE(context, type, oldPointer) \
		do \
		{ \
			if (nullptr != oldPointer) \
			{ \
				typedef type destructor; \
				static_cast<type*>(oldPointer)->~destructor(); \
				(context).getAllocator().reallocate(oldPointer, 0, 0, 1); \
			} \
		} while (0)

	// New and delete of arrays
	// - Using placement new and explicit destructor call, not using the array version since it's using an undocumented additional amount of memory
	// - See http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/ - "2.  Wrap your macros in do { � } while(0)." for background information about the do-while wrap
	#define RHI_NEW_ARRAY(context, type, count) Rhi::IAllocator::constructN(static_cast<type*>(((context).getAllocator().reallocate(nullptr, 0, sizeof(type) * (count), 1))), count)
	#define RHI_DELETE_ARRAY(context, type, oldPointer, count) \
		do \
		{ \
			if (nullptr != oldPointer) \
			{ \
				for (size_t allocatorArrayIndex = 0; allocatorArrayIndex < count; ++allocatorArrayIndex) \
				{ \
					typedef type destructor; \
					(static_cast<type*>(oldPointer))[allocatorArrayIndex].~destructor(); \
				} \
				(context).getAllocator().reallocate(oldPointer, 0, 0, 1); \
			} \
		} while (0)




	//[-------------------------------------------------------]
	//[ Rhi/RhiTypes.h                                        ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Holds information about an window into which the rendering should be done
	*
	*  @note
	*    - One of those members must be valid
	*/
	struct WindowHandle final
	{
		handle			nativeWindowHandle;	///< The native window handle
		IRenderWindow*	renderWindow;		///< A pointer to an "Rhi::IRenderWindow"-instance, can be a null pointer
		#ifdef LINUX
			wl_surface*	waylandSurface;		///< A Wayland surface can't be put into a handle type, so we store a pointer to the Wayland surface here
		#else
			void*		unused;				///< For writing e.g. "Rhi::WindowHandle{nativeWindowHandle, nullptr, nullptr}" for all platforms // TODO(co) We might want to implement another solution like "WindowHandle::fromNativeWindowHandle()", "WindowHandle::fromRenderWindow()", "WindowHandle::fromWaylandSurface()", while there we could add a type and then using a data-union
		#endif
	};

	/**
	*  @brief
	*    Map types
	*
	*  @note
	*    - These constants directly map to Direct3D 10 & 11 constants, do not change them
	*/
	// TODO(co) Comments
	enum class MapType
	{
		READ			   = 1,	///<
		WRITE			   = 2,	///<
		READ_WRITE		   = 3,	///<
		WRITE_DISCARD	   = 4,	///<
		WRITE_NO_OVERWRITE = 5	///<
	};

	/**
	*  @brief
	*    Map flags
	*
	*  @note
	*    - These constants directly map to Direct3D 11 constants, do not change them
	*/
	struct MapFlag final
	{
		enum Enum
		{
			DO_NOT_WAIT = 0x100000L	///< In case the resource is currently used when "Rhi::IRhi::map()" is called, let the method return with an error, cannot be used with "Rhi::MapType::WRITE_DISCARD" or "Rhi::MapType::WRITE_NO_OVERWRITE"
		};
	};

	/**
	*  @brief
	*    Clear flags
	*/
	struct ClearFlag final
	{
		enum Enum
		{
			COLOR       = 1 << 0,		///< Clear color buffer
			DEPTH       = 1 << 1,		///< Clear depth buffer
			STENCIL     = 1 << 2,		///< Clear stencil buffer
			COLOR_DEPTH = COLOR | DEPTH	///< Clear color and depth buffer
		};
	};

	/**
	*  @brief
	*    Comparison function
	*
	*  @note
	*    - Original Direct3D comments from http://msdn.microsoft.com/en-us/library/windows/desktop/bb204902%28v=vs.85%29.aspx are used in here
	*    - These constants directly map to Direct3D 10 & 11 & 12 constants, do not change them
	*
	*  @see
	*    - "D3D12_COMPARISON_FUNC"-documentation for details
	*/
	enum class ComparisonFunc
	{
		NEVER		  = 1,	///< Never pass the comparison
		LESS		  = 2,	///< If the source data is less than the destination data, the comparison passes
		EQUAL		  = 3,	///< If the source data is equal to the destination data, the comparison passes
		LESS_EQUAL	  = 4,	///< If the source data is less than or equal to the destination data, the comparison passes
		GREATER		  = 5,	///< If the source data is greater than the destination data, the comparison passes
		NOT_EQUAL	  = 6,	///< If the source data is not equal to the destination data, the comparison passes
		GREATER_EQUAL = 7,	///< If the source data is greater than or equal to the destination data, the comparison passes
		ALWAYS		  = 8	///< Always pass the comparison
	};

	/**
	*  @brief
	*    Color write enable flags
	*
	*  @note
	*    - These constants directly map to Direct3D 10 & 11 constants, do not change them
	*/
	// TODO(co) Rhi::ColorWriteEnableFlag, document
	// TODO(co) A flags-class would be nice to avoid invalid flags
	struct ColorWriteEnableFlag final
	{
		enum Enum
		{
			RED   = 1,
			GREEN = 2,
			BLUE  = 4,
			ALPHA = 8,
			ALL   = (((RED | GREEN) | BLUE) | ALPHA)
		};
	};

	/**
	*  @brief
	*    Mapped subresource
	*
	*  @note
	*    - This structure directly maps to Direct3D 11, do not change it
	*/
	// TODO(co) Comments
	struct MappedSubresource final
	{
		void*	 data;
		uint32_t rowPitch;
		uint32_t depthPitch;
	};

	/**
	*  @brief
	*    Viewport
	*
	*  @note
	*    - This structure directly maps to Direct3D 11 & 12 as well as Vulkan, do not change it
	*
	*  @see
	*    - "D3D12_VIEWPORT" or "VkViewport" documentation for details
	*/
	struct Viewport final
	{
		float topLeftX;	///< Top left x start position
		float topLeftY;	///< Top left y start position
		float width;	///< Viewport width
		float height;	///< Viewport height
		float minDepth;	///< Minimum depth value, usually 0.0f, between [0, 1]
		float maxDepth;	///< Maximum depth value, usually 1.0f, between [0, 1]
	};

	/**
	*  @brief
	*    Scissor rectangle
	*
	*  @note
	*    - This structure directly maps to Direct3D 9 & 10 & 11 & 12, do not change it
	*
	*  @see
	*    - "D3D12_RECT"-documentation for details
	*/
	struct ScissorRectangle final
	{
		long topLeftX;		///< Top left x-coordinate of the scissor rectangle
		long topLeftY;		///< Top left y-coordinate of the scissor rectangle
		long bottomRightX;	///< Bottom right x-coordinate of the scissor rectangle
		long bottomRightY;	///< Bottom right y-coordinate of the scissor rectangle
	};




	//[-------------------------------------------------------]
	//[ Rhi/ResourceType.h                                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Resource type
	*/
	enum class ResourceType
	{
		ROOT_SIGNATURE				   = 0,		///< Root signature
		RESOURCE_GROUP				   = 1,		///< Resource group
		GRAPHICS_PROGRAM			   = 2,		///< Graphics program, "Rhi::IShader"-related
		VERTEX_ARRAY				   = 3,		///< Vertex array object (VAO, input-assembler (IA) stage), "Rhi::IBuffer"-related
		RENDER_PASS					   = 4,		///< Render pass
		QUERY_POOL					   = 5,		///< Asynchronous query pool
		// IRenderTarget
		SWAP_CHAIN					   = 6,		///< Swap chain
		FRAMEBUFFER					   = 7,		///< Framebuffer object (FBO)
		// IBuffer
		VERTEX_BUFFER				   = 8,		///< Vertex buffer object (VBO, input-assembler (IA) stage)
		INDEX_BUFFER				   = 9,		///< Index buffer object (IBO, input-assembler (IA) stage)
		TEXTURE_BUFFER				   = 10,	///< Texture buffer object (TBO)
		STRUCTURED_BUFFER			   = 11,	///< Structured buffer object (SBO)
		INDIRECT_BUFFER				   = 12,	///< Indirect buffer object
		UNIFORM_BUFFER				   = 13,	///< Uniform buffer object (UBO, "constant buffer" in Direct3D terminology)
		// ITexture
		TEXTURE_1D					   = 14,	///< Texture 1D
		TEXTURE_1D_ARRAY			   = 15,	///< Texture 1D array
		TEXTURE_2D					   = 16,	///< Texture 2D
		TEXTURE_2D_ARRAY			   = 17,	///< Texture 2D array
		TEXTURE_3D					   = 18,	///< Texture 3D
		TEXTURE_CUBE				   = 19,	///< Texture cube
		TEXTURE_CUBE_ARRAY			   = 20,	///< Texture cube array
		// IState
			// IPipelineState
			GRAPHICS_PIPELINE_STATE	   = 21,	///< Graphics pipeline state (PSO)
			COMPUTE_PIPELINE_STATE	   = 22,	///< Compute pipeline state (PSO)
		SAMPLER_STATE				   = 23,	///< Sampler state
		// IShader
		VERTEX_SHADER				   = 24,	///< Vertex shader (VS)
		TESSELLATION_CONTROL_SHADER	   = 25,	///< Tessellation control shader (TCS, "hull shader" in Direct3D terminology)
		TESSELLATION_EVALUATION_SHADER = 26,	///< Tessellation evaluation shader (TES, "domain shader" in Direct3D terminology)
		GEOMETRY_SHADER				   = 27,	///< Geometry shader (GS)
		FRAGMENT_SHADER				   = 28,	///< Fragment shader (FS, "pixel shader" in Direct3D terminology)
		TASK_SHADER					   = 29,	///< Task shader (TS, "amplification shader" in Direct3D terminology)
		MESH_SHADER					   = 30,	///< Mesh shader (MS)
		COMPUTE_SHADER				   = 31		///< Compute shader (CS)
	};




	//[-------------------------------------------------------]
	//[ Rhi/State/SamplerStateTypes.h                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Filter mode
	*
	*  @note
	*    - Original Direct3D comments from http://msdn.microsoft.com/en-us/library/windows/desktop/bb205060%28v=vs.85%29.aspx are used in here
	*    - These constants directly map to Direct3D 10 & 11 & 12 constants, do not change them
	*    - "Point" = "nearest" in OpenGL terminology
	*
	*  @see
	*    - "D3D12_FILTER"-documentation for details
	*/
	enum class FilterMode
	{
		MIN_MAG_MIP_POINT						   = 0,		///< Use point sampling for minification, magnification, and mip-level sampling.
		MIN_MAG_POINT_MIP_LINEAR				   = 0x1,	///< Use point sampling for minification and magnification; use linear interpolation for mip-level sampling.
		MIN_POINT_MAG_LINEAR_MIP_POINT			   = 0x4,	///< Use point sampling for minification; use linear interpolation for magnification; use point sampling for mip-level sampling.
		MIN_POINT_MAG_MIP_LINEAR				   = 0x5,	///< Use point sampling for minification; use linear interpolation for magnification and mip-level sampling.
		MIN_LINEAR_MAG_MIP_POINT				   = 0x10,	///< Use linear interpolation for minification; use point sampling for magnification and mip-level sampling.
		MIN_LINEAR_MAG_POINT_MIP_LINEAR			   = 0x11,	///< Use linear interpolation for minification; use point sampling for magnification; use linear interpolation for mip-level sampling.
		MIN_MAG_LINEAR_MIP_POINT				   = 0x14,	///< Use linear interpolation for minification and magnification; use point sampling for mip-level sampling.
		MIN_MAG_MIP_LINEAR						   = 0x15,	///< Use linear interpolation for minification, magnification, and mip-level sampling.
		ANISOTROPIC								   = 0x55,	///< Use anisotropic interpolation for minification, magnification, and mip-level sampling.
		COMPARISON_MIN_MAG_MIP_POINT			   = 0x80,	///< Use point sampling for minification, magnification, and mip-level sampling. Compare the result to the comparison value.
		COMPARISON_MIN_MAG_POINT_MIP_LINEAR		   = 0x81,	///< Use point sampling for minification and magnification; use linear interpolation for mip-level sampling. Compare the result to the comparison value.
		COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT  = 0x84,	///< Use point sampling for minification; use linear interpolation for magnification; use point sampling for mip-level sampling. Compare the result to the comparison value.
		COMPARISON_MIN_POINT_MAG_MIP_LINEAR		   = 0x85,	///< Use point sampling for minification; use linear interpolation for magnification and mip-level sampling. Compare the result to the comparison value.
		COMPARISON_MIN_LINEAR_MAG_MIP_POINT		   = 0x90,	///< Use linear interpolation for minification; use point sampling for magnification and mip-level sampling. Compare the result to the comparison value.
		COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,	///< Use linear interpolation for minification; use point sampling for magnification; use linear interpolation for mip-level sampling. Compare the result to the comparison value.
		COMPARISON_MIN_MAG_LINEAR_MIP_POINT		   = 0x94,	///< Use linear interpolation for minification and magnification; use point sampling for mip-level sampling. Compare the result to the comparison value.
		COMPARISON_MIN_MAG_MIP_LINEAR			   = 0x95,	///< Use linear interpolation for minification, magnification, and mip-level sampling. Compare the result to the comparison value.
		COMPARISON_ANISOTROPIC					   = 0xd5,	///< Use anisotropic interpolation for minification, magnification, and mip-level sampling. Compare the result to the comparison value.
		UNKNOWN									   = 0xd6	///< Unknown invalid setting
	};

	/**
	*  @brief
	*    Texture address mode
	*
	*  @note
	*    - Original Direct3D comments from http://msdn.microsoft.com/en-us/library/windows/desktop/bb172483%28v=vs.85%29.aspx are used in here
	*    - These constants directly map to Direct3D 10 & 11 & 12 constants, do not change them
	*
	*  @see
	*    - "D3D12_TEXTURE_ADDRESS_MODE"-documentation for details
	*/
	enum class TextureAddressMode
	{
		WRAP		= 1,	///< Tile the texture at every integer junction. For example, for u values between 0 and 3, the texture is repeated three times.
		MIRROR		= 2,	///< Flip the texture at every integer junction. For u values between 0 and 1, for example, the texture is addressed normally; between 1 and 2, the texture is flipped (mirrored); between 2 and 3, the texture is normal again; and so on.
		CLAMP		= 3,	///< Texture coordinates outside the range [0.0, 1.0] are set to the texture color at 0.0 or 1.0, respectively.
		BORDER		= 4,	///< Texture coordinates outside the range [0.0, 1.0] are set to the border color specified in "SamplerState::borderColor".
		MIRROR_ONCE	= 5		///< Similar to "MIRROR" and "CLAMP". Takes the absolute value of the texture coordinate (thus, mirroring around 0), and then clamps to the maximum value.
	};

	/**
	*  @brief
	*    Sampler state
	*
	*  @remarks
	*    == About mipmapping ==
	*    The texture filter mode does not support explicitly disabling mipmapping. In case our texture does not have
	*    any mipmaps, set "Rhi::SamplerState::maxLod" to zero in order to ensure a correct behaviour across the
	*    difference graphics APIs. When not doing this you usually have no issues when using OpenGL, OpenGL ES 3, Direct 10,
	*    Direct3D 11 or Direct3D 9 with the "ps_2_0"-profile, but when using Direct3D 9 with the "ps_3_0"-profile you might
	*    get into trouble due to another internal graphics API behaviour.
	*
	*  @note
	*    - This sampler state maps directly to Direct3D 10 & 11, do not change it
	*    - If you want to know how the default values were chosen, have a look into the "Rhi::ISamplerState::getDefaultSamplerState()"-implementation
	*
	*  @see
	*    - "D3D12_SAMPLER_DESC"-documentation for details
	*/
	struct SamplerState final
	{
		FilterMode		   filter;			///< Default: "Rhi::FilterMode::MIN_MAG_MIP_LINEAR"
		TextureAddressMode addressU;		///< (also known as "S"), Default: "Rhi::TextureAddressMode::CLAMP"
		TextureAddressMode addressV;		///< (also known as "T"), Default: "Rhi::TextureAddressMode::CLAMP"
		TextureAddressMode addressW;		///< (also known as "R"), Default: "Rhi::TextureAddressMode::CLAMP"
		float			   mipLodBias;		///< Default: "0.0f"
		uint32_t		   maxAnisotropy;	///< Default: "16"
		ComparisonFunc	   comparisonFunc;	///< Default: "Rhi::ComparisonFunc::NEVER"
		float			   borderColor[4];	///< Default: 0.0f, 0.0f, 0.0f, 0.0f
		float			   minLod;			///< Default: -3.402823466e+38f (-FLT_MAX)
		float			   maxLod;			///< Default: 3.402823466e+38f (FLT_MAX)
	};




	//[-------------------------------------------------------]
	//[ Rhi/RootSignatureTypes.h                              ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Descriptor range type
	*
	*  @note
	*    - These constants directly map to Direct3D 12 constants, do not change them
	*
	*  @see
	*    - "D3D12_DESCRIPTOR_RANGE_TYPE"-documentation for details
	*/
	enum class DescriptorRangeType
	{
		SRV					  = 0,			///< Shader resource view (SRV), in HLSL: "t<index>"-register
		UAV					  = SRV + 1,	///< Unordered access view (UAV), in HLSL: "u<index>"-register
		UBV					  = UAV + 1,	///< Uniform buffer view (UBV), in HLSL: "b<index>"-register, "UBV" = "CBV"; we're using the OpenGL/Vulkan terminology of "uniform buffer" instead of "constant buffer" as DirectX does
		SAMPLER				  = UBV + 1,	///< In HLSL: "s<index>"-register
		NUMBER_OF_RANGE_TYPES = SAMPLER + 1	///< Number of range type, invalid descriptor range type
	};

	/**
	*  @brief
	*    Shader visibility
	*
	*  @note
	*    - These constants directly map to Direct3D 12 constants, do not change them
	*
	*  @see
	*    - "D3D12_SHADER_VISIBILITY"-documentation for details
	*/
	enum class ShaderVisibility
	{
		ALL                     = 0,
		VERTEX                  = 1,
		TESSELLATION_CONTROL    = 2,
		TESSELLATION_EVALUATION = 3,
		GEOMETRY                = 4,
		FRAGMENT                = 5,
		TASK                    = 6,
		MESH                    = 7,
		// The rest is not part of "D3D12_SHADER_VISIBILITY"
		COMPUTE                 = 8,
		ALL_GRAPHICS            = 9
	};

	/**
	*  @brief
	*    Descriptor range
	*
	*  @note
	*    - "Rhi::DescriptorRange" is not identical to "D3D12_DESCRIPTOR_RANGE" because it had to be extended by information required by OpenGL
	*
	*  @see
	*    - "D3D12_DESCRIPTOR_RANGE"-documentation for details
	*/
	struct DescriptorRange
	{
		DescriptorRangeType rangeType;
		uint32_t			numberOfDescriptors;
		uint32_t			baseShaderRegister;	///< When using explicit binding locations
		uint32_t			registerSpace;
		uint32_t			offsetInDescriptorsFromTableStart;

		// The rest is not part of "D3D12_DESCRIPTOR_RANGE" and was added to support Vulkan, OpenGL and Direct3D 9 as well
		static constexpr uint32_t NAME_LENGTH = 31 + 1;	///< +1 for the terminating zero
		char					  baseShaderRegisterName[NAME_LENGTH];	///< When not using explicit binding locations (OpenGL ES 3, legacy GLSL profiles)
		ShaderVisibility		  shaderVisibility;
		ResourceType			  resourceType;
	};
	struct DescriptorRangeBuilder final : public DescriptorRange
	{
		static constexpr uint32_t OFFSET_APPEND = 0xffffffff;
		static inline void initialize(
			DescriptorRange& range,
			ResourceType _resourceType,
			uint32_t _baseShaderRegister,
			const char _baseShaderRegisterName[NAME_LENGTH],
			ShaderVisibility _shaderVisibility,
			DescriptorRangeType _rangeType =  DescriptorRangeType::NUMBER_OF_RANGE_TYPES,	// Automatically determine the descriptor range type basing on the resource type
			uint32_t _numberOfDescriptors = 1,
			uint32_t _registerSpace = 0,
			uint32_t _offsetInDescriptorsFromTableStart = OFFSET_APPEND)
		{
			if (DescriptorRangeType::NUMBER_OF_RANGE_TYPES == _rangeType)
			{
				// Automatically determine the descriptor range type basing on the resource type
				switch (_resourceType)
				{
					case ResourceType::VERTEX_BUFFER:
					case ResourceType::INDEX_BUFFER:
					case ResourceType::TEXTURE_BUFFER:
					case ResourceType::STRUCTURED_BUFFER:
					case ResourceType::INDIRECT_BUFFER:
						range.rangeType = DescriptorRangeType::SRV;
						break;

					case ResourceType::UNIFORM_BUFFER:
						range.rangeType = DescriptorRangeType::UBV;
						break;

					case ResourceType::TEXTURE_1D:
					case ResourceType::TEXTURE_1D_ARRAY:
					case ResourceType::TEXTURE_2D:
					case ResourceType::TEXTURE_2D_ARRAY:
					case ResourceType::TEXTURE_3D:
					case ResourceType::TEXTURE_CUBE:
					case ResourceType::TEXTURE_CUBE_ARRAY:
						range.rangeType = DescriptorRangeType::SRV;
						break;

					case ResourceType::SAMPLER_STATE:
						range.rangeType = DescriptorRangeType::SAMPLER;
						break;

					case ResourceType::ROOT_SIGNATURE:
					case ResourceType::RESOURCE_GROUP:
					case ResourceType::GRAPHICS_PROGRAM:
					case ResourceType::VERTEX_ARRAY:
					case ResourceType::RENDER_PASS:
					case ResourceType::QUERY_POOL:
					case ResourceType::SWAP_CHAIN:
					case ResourceType::FRAMEBUFFER:
					case ResourceType::GRAPHICS_PIPELINE_STATE:
					case ResourceType::COMPUTE_PIPELINE_STATE:
					case ResourceType::VERTEX_SHADER:
					case ResourceType::TESSELLATION_CONTROL_SHADER:
					case ResourceType::TESSELLATION_EVALUATION_SHADER:
					case ResourceType::GEOMETRY_SHADER:
					case ResourceType::FRAGMENT_SHADER:
					case ResourceType::TASK_SHADER:
					case ResourceType::MESH_SHADER:
					case ResourceType::COMPUTE_SHADER:
						ASSERT(false, "Invalid resource type")
						break;
				}
			}
			else
			{
				range.rangeType = _rangeType;
			}
			range.numberOfDescriptors = _numberOfDescriptors;
			range.baseShaderRegister = _baseShaderRegister;
			range.registerSpace = _registerSpace;
			range.offsetInDescriptorsFromTableStart = _offsetInDescriptorsFromTableStart;
			strcpy(range.baseShaderRegisterName, _baseShaderRegisterName);
			range.shaderVisibility = _shaderVisibility;
			range.resourceType = _resourceType;
			ASSERT(ResourceType::UNIFORM_BUFFER != range.resourceType || DescriptorRangeType::UAV != range.rangeType, "Uniform buffer doesn't support UAV")
		}
		inline DescriptorRangeBuilder()
		{}
		inline explicit DescriptorRangeBuilder(const DescriptorRangeBuilder&)
		{}
		inline DescriptorRangeBuilder(
			ResourceType _resourceType,
			uint32_t _baseShaderRegister,
			const char _baseShaderRegisterName[NAME_LENGTH],
			ShaderVisibility _shaderVisibility,
			DescriptorRangeType _rangeType = DescriptorRangeType::NUMBER_OF_RANGE_TYPES,	// Automatically determine the descriptor range type basing on the resource type
			uint32_t _numberOfDescriptors = 1,
			uint32_t _registerSpace = 0,
			uint32_t _offsetInDescriptorsFromTableStart = OFFSET_APPEND)
		{
			initialize(_resourceType, _baseShaderRegister, _baseShaderRegisterName, _shaderVisibility, _rangeType, _numberOfDescriptors, _registerSpace, _offsetInDescriptorsFromTableStart);
		}
		inline void initializeSampler(
			uint32_t _baseShaderRegister,
			ShaderVisibility _shaderVisibility,
			uint32_t _numberOfDescriptors = 1,
			uint32_t _registerSpace = 0,
			uint32_t _offsetInDescriptorsFromTableStart = OFFSET_APPEND)
		{
			initialize(*this, ResourceType::SAMPLER_STATE, _baseShaderRegister, "", _shaderVisibility, DescriptorRangeType::SAMPLER, _numberOfDescriptors, _registerSpace, _offsetInDescriptorsFromTableStart);
		}
		inline void initialize(
			ResourceType _resourceType,
			uint32_t _baseShaderRegister,
			const char _baseShaderRegisterName[NAME_LENGTH],
			ShaderVisibility _shaderVisibility,
			DescriptorRangeType _rangeType = DescriptorRangeType::NUMBER_OF_RANGE_TYPES,	// Automatically determine the descriptor range type basing on the resource type
			uint32_t _numberOfDescriptors = 1,
			uint32_t _registerSpace = 0,
			uint32_t _offsetInDescriptorsFromTableStart = OFFSET_APPEND)
		{
			initialize(*this, _resourceType, _baseShaderRegister, _baseShaderRegisterName, _shaderVisibility, _rangeType, _numberOfDescriptors, _registerSpace, _offsetInDescriptorsFromTableStart);
		}
	};

	/**
	*  @brief
	*    Root descriptor table
	*
	*  @note
	*    - This structure directly maps to Direct3D 12 structure, do not change it
	*    - Samplers are not allowed in the same descriptor table as UBV/UAV/SRVs
	*
	*  @see
	*    - "D3D12_ROOT_DESCRIPTOR_TABLE"-documentation for details
	*/
	struct RootDescriptorTable
	{
		uint32_t numberOfDescriptorRanges;
		uint64_t descriptorRanges;			///< Can't use "const DescriptorRange*" because we need to have something platform neutral we can easily serialize without getting too fine granular
	};
	struct RootDescriptorTableBuilder final : public RootDescriptorTable
	{
		static inline void initialize(
			RootDescriptorTable& rootDescriptorTable,
			uint32_t _numberOfDescriptorRanges,
			const DescriptorRange* _descriptorRanges)
		{
			rootDescriptorTable.numberOfDescriptorRanges = _numberOfDescriptorRanges;
			rootDescriptorTable.descriptorRanges = reinterpret_cast<uintptr_t>(_descriptorRanges);
		}
		inline RootDescriptorTableBuilder()
		{}
		inline explicit RootDescriptorTableBuilder(const RootDescriptorTableBuilder&)
		{}
		inline RootDescriptorTableBuilder(
			uint32_t _numberOfDescriptorRanges,
			const DescriptorRange* _descriptorRanges)
		{
			initialize(_numberOfDescriptorRanges, _descriptorRanges);
		}
		inline void initialize(
			uint32_t _numberOfDescriptorRanges,
			const DescriptorRange* _descriptorRanges)
		{
			initialize(*this, _numberOfDescriptorRanges, _descriptorRanges);
		}
	};

	/**
	*  @brief
	*    Root parameter type
	*
	*  @note
	*    - These constants directly map to Direct3D 12 constants, do not change them
	*
	*  @see
	*    - "D3D12_ROOT_PARAMETER_TYPE"-documentation for details
	*/
	enum class RootParameterType
	{
		DESCRIPTOR_TABLE = 0,
		CONSTANTS_32BIT  = DESCRIPTOR_TABLE + 1,
		UBV              = CONSTANTS_32BIT + 1,		///< Uniform buffer view (UBV), in HLSL: "b<index>"-register, "UBV" = "CBV"; we're using the OpenGL/Vulkan terminology of "uniform buffer" instead of "constant buffer" as DirectX does
		SRV              = UBV + 1,					///< Shader resource view (SRV), in HLSL: "t<index>"-register
		UAV              = SRV + 1					///< Unordered access view (UAV), in HLSL: "u<index>"-register
	};

	/**
	*  @brief
	*    Root constants
	*
	*  @note
	*    - This structure directly maps to Direct3D 12 structure, do not change it
	*
	*  @see
	*    - "D3D12_ROOT_CONSTANTS"-documentation for details
	*/
	struct RootConstants
	{
		uint32_t shaderRegister;
		uint32_t registerSpace;
		uint32_t numberOf32BitValues;
	};
	struct RootConstantsBuilder final : public RootConstants
	{
		static inline void initialize(
			RootConstants& rootConstants,
			uint32_t _numberOf32BitValues,
			uint32_t _shaderRegister,
			uint32_t _registerSpace = 0)
		{
			rootConstants.numberOf32BitValues = _numberOf32BitValues;
			rootConstants.shaderRegister = _shaderRegister;
			rootConstants.registerSpace = _registerSpace;
		}
		inline RootConstantsBuilder()
		{}
		inline explicit RootConstantsBuilder(const RootConstantsBuilder&)
		{}
		inline RootConstantsBuilder(
			uint32_t _numberOf32BitValues,
			uint32_t _shaderRegister,
			uint32_t _registerSpace = 0)
		{
			initialize(_numberOf32BitValues, _shaderRegister, _registerSpace);
		}
		inline void initialize(
			uint32_t _numberOf32BitValues,
			uint32_t _shaderRegister,
			uint32_t _registerSpace = 0)
		{
			initialize(*this, _numberOf32BitValues, _shaderRegister, _registerSpace);
		}
	};

	/**
	*  @brief
	*    Root descriptor
	*
	*  @note
	*    - This structure directly maps to Direct3D 12 structure, do not change it
	*
	*  @see
	*    - "D3D12_ROOT_DESCRIPTOR"-documentation for details
	*/
	struct RootDescriptor
	{
		uint32_t shaderRegister;
		uint32_t registerSpace;
	};
	struct RootDescriptorBuilder final : public RootDescriptor
	{
		static inline void initialize(RootDescriptor& table, uint32_t _shaderRegister, uint32_t _registerSpace = 0)
		{
			table.shaderRegister = _shaderRegister;
			table.registerSpace = _registerSpace;
		}
		inline RootDescriptorBuilder()
		{}
		inline explicit RootDescriptorBuilder(const RootDescriptorBuilder&)
		{}
		inline RootDescriptorBuilder(
			uint32_t _shaderRegister,
			uint32_t _registerSpace = 0)
		{
			initialize(_shaderRegister, _registerSpace);
		}
		inline void initialize(
			uint32_t _shaderRegister,
			uint32_t _registerSpace = 0)
		{
			initialize(*this, _shaderRegister, _registerSpace);
		}
	};

	/**
	*  @brief
	*    Root parameter
	*
	*  @note
	*    - "Rhi::RootParameter" is not identical to "D3D12_ROOT_PARAMETER", the shader visibility is defined per descriptor since Vulkan needs it this way
	*
	*  @see
	*    - "D3D12_ROOT_PARAMETER"-documentation for details
	*/
	struct RootParameter
	{
		RootParameterType		parameterType;
		union
		{
			RootDescriptorTable	descriptorTable;
			RootConstants		constants;
			RootDescriptor		descriptor;
		};
	};
	struct RootParameterData final
	{
		RootParameterType	parameterType;
		uint32_t			numberOfDescriptorRanges;
	};
	struct RootParameterBuilder final : public RootParameter
	{
		static inline void initializeAsDescriptorTable(
			RootParameter& rootParam,
			uint32_t numberOfDescriptorRanges,
			const DescriptorRange* descriptorRanges)
		{
			rootParam.parameterType = RootParameterType::DESCRIPTOR_TABLE;
			RootDescriptorTableBuilder::initialize(rootParam.descriptorTable, numberOfDescriptorRanges, descriptorRanges);
		}
		static inline void initializeAsConstants(
			RootParameter& rootParam,
			uint32_t numberOf32BitValues,
			uint32_t shaderRegister,
			uint32_t registerSpace = 0)
		{
			rootParam.parameterType = RootParameterType::CONSTANTS_32BIT;
			RootConstantsBuilder::initialize(rootParam.constants, numberOf32BitValues, shaderRegister, registerSpace);
		}
		static inline void initializeAsConstantBufferView(
			RootParameter& rootParam,
			uint32_t shaderRegister,
			uint32_t registerSpace = 0)
		{
			rootParam.parameterType = RootParameterType::UBV;
			RootDescriptorBuilder::initialize(rootParam.descriptor, shaderRegister, registerSpace);
		}
		static inline void initializeAsShaderResourceView(
			RootParameter& rootParam,
			uint32_t shaderRegister,
			uint32_t registerSpace = 0)
		{
			rootParam.parameterType = RootParameterType::SRV;
			RootDescriptorBuilder::initialize(rootParam.descriptor, shaderRegister, registerSpace);
		}
		static inline void initializeAsUnorderedAccessView(
			RootParameter& rootParam,
			uint32_t shaderRegister,
			uint32_t registerSpace = 0)
		{
			rootParam.parameterType = RootParameterType::UAV;
			RootDescriptorBuilder::initialize(rootParam.descriptor, shaderRegister, registerSpace);
		}
		inline RootParameterBuilder()
		{}
		inline explicit RootParameterBuilder(const RootParameterBuilder&)
		{}
		inline void initializeAsDescriptorTable(
			uint32_t numberOfDescriptorRanges,
			const DescriptorRange* descriptorRanges)
		{
			initializeAsDescriptorTable(*this, numberOfDescriptorRanges, descriptorRanges);
		}
		inline void initializeAsConstants(
			uint32_t numberOf32BitValues,
			uint32_t shaderRegister,
			uint32_t registerSpace = 0)
		{
			initializeAsConstants(*this, numberOf32BitValues, shaderRegister, registerSpace);
		}
		inline void initializeAsConstantBufferView(
			uint32_t shaderRegister,
			uint32_t registerSpace = 0)
		{
			initializeAsConstantBufferView(*this, shaderRegister, registerSpace);
		}
		inline void initializeAsShaderResourceView(
			uint32_t shaderRegister,
			uint32_t registerSpace = 0)
		{
			initializeAsShaderResourceView(*this, shaderRegister, registerSpace);
		}
		inline void initializeAsUnorderedAccessView(
			uint32_t shaderRegister,
			uint32_t registerSpace = 0)
		{
			initializeAsUnorderedAccessView(*this, shaderRegister, registerSpace);
		}
	};

	/**
	*  @brief
	*    Root signature flags
	*
	*  @note
	*    - These constants directly map to Direct3D 12 constants, do not change them
	*
	*  @see
	*    - "D3D12_ROOT_SIGNATURE_FLAGS"-documentation for details
	*/
	struct RootSignatureFlags final
	{
		enum Enum
		{
			NONE                                            = 0,
			ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT              = 0x1,
			DENY_VERTEX_SHADER_ROOT_ACCESS                  = 0x2,
			DENY_TESSELLATION_CONTROL_SHADER_ROOT_ACCESS    = 0x4,
			DENY_TESSELLATION_EVALUATION_SHADER_ROOT_ACCESS = 0x8,
			DENY_GEOMETRY_SHADER_ROOT_ACCESS                = 0x10,
			DENY_FRAGMENT_SHADER_ROOT_ACCESS                = 0x20,
			ALLOW_STREAM_OUTPUT                             = 0x40
		};
	};

	/**
	*  @brief
	*    Static border color
	*
	*  @note
	*    - These constants directly map to Direct3D 12 constants, do not change them
	*
	*  @see
	*    - "D3D12_STATIC_BORDER_COLOR"-documentation for details
	*/
	enum class StaticBorderColor
	{
		TRANSPARENT_BLACK = 0,
		OPAQUE_BLACK      = TRANSPARENT_BLACK + 1,
		OPAQUE_WHITE      = OPAQUE_BLACK + 1
	};

	/**
	*  @brief
	*    Static sampler
	*
	*  @note
	*    - This structure directly maps to Direct3D 12 structure, do not change it
	*
	*  @see
	*    - "D3D12_STATIC_SAMPLER_DESC"-documentation for details
	*/
	struct StaticSampler final
	{
		FilterMode			filter;
		TextureAddressMode	addressU;
		TextureAddressMode	addressV;
		TextureAddressMode	addressW;
		float				mipLodBias;
		uint32_t			maxAnisotropy;
		ComparisonFunc		comparisonFunc;
		StaticBorderColor	borderColor;
		float				minLod;
		float				maxLod;
		uint32_t			shaderRegister;
		uint32_t			registerSpace;
		ShaderVisibility	shaderVisibility;
	};

	/**
	*  @brief
	*    Root signature
	*
	*  @note
	*    - "Rhi::RootSignature" is not totally identical to "D3D12_ROOT_SIGNATURE_DESC" because it had to be extended by information required by OpenGL, so can't cast to Direct3D 12 structure
	*    - In order to be RHI implementation independent, do always define and set samplers first
	*    - "Rhi::DescriptorRange": In order to be RHI implementation independent, do always provide "baseShaderRegisterName" for "Rhi::DescriptorRangeType::SRV" range types
	*
	*  @see
	*    - "D3D12_ROOT_SIGNATURE_DESC"-documentation for details
	*/
	struct RootSignature
	{
		uint32_t				 numberOfParameters;
		const RootParameter*	 parameters;
		uint32_t				 numberOfStaticSamplers;
		const StaticSampler*	 staticSamplers;
		RootSignatureFlags::Enum flags;
	};
	struct RootSignatureBuilder final : public RootSignature
	{
		static inline void initialize(
			RootSignature& rootSignature,
			uint32_t _numberOfParameters,
			const RootParameter* _parameters,
			uint32_t _numberOfStaticSamplers = 0,
			const StaticSampler* _staticSamplers = nullptr,
			RootSignatureFlags::Enum _flags = RootSignatureFlags::NONE)
		{
			rootSignature.numberOfParameters = _numberOfParameters;
			rootSignature.parameters = _parameters;
			rootSignature.numberOfStaticSamplers = _numberOfStaticSamplers;
			rootSignature.staticSamplers = _staticSamplers;
			rootSignature.flags = _flags;
		}
		inline RootSignatureBuilder()
		{}
		inline explicit RootSignatureBuilder(const RootSignatureBuilder&)
		{}
		inline RootSignatureBuilder(
			uint32_t _numberOfParameters,
			const RootParameter* _parameters,
			uint32_t _numberOfStaticSamplers = 0,
			const StaticSampler* _staticSamplers = nullptr,
			RootSignatureFlags::Enum _flags = RootSignatureFlags::NONE)
		{
			initialize(_numberOfParameters, _parameters, _numberOfStaticSamplers, _staticSamplers, _flags);
		}
		inline void initialize(
			uint32_t _numberOfParameters,
			const RootParameter* _parameters,
			uint32_t _numberOfStaticSamplers = 0,
			const StaticSampler* _staticSamplers = nullptr,
			RootSignatureFlags::Enum _flags = RootSignatureFlags::NONE)
		{
			initialize(*this, _numberOfParameters, _parameters, _numberOfStaticSamplers, _staticSamplers, _flags);
		}
	};




	//[-------------------------------------------------------]
	//[ Rhi/Texture/TextureTypes.h                            ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Texture format
	*/
	struct TextureFormat final
	{
		enum Enum : uint8_t
		{
			R8				  = 0,	///< 8-bit pixel format, all bits red
			R8G8B8			  = 1,	///< 24-bit pixel format, 8 bits for red, green and blue
			R8G8B8A8		  = 2,	///< 32-bit pixel format, 8 bits for red, green, blue and alpha
			R8G8B8A8_SRGB	  = 3,	///< 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
			B8G8R8A8		  = 4,	///< 32-bit pixel format, 8 bits for red, green, blue and alpha
			R11G11B10F		  = 5,	///< 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent, see e.g. https://www.khronos.org/opengl/wiki/Small_Float_Formats#R11F_G11F_B10F -> "Small Float Formats" -> "Numeric limits and precision" and "Small float formats � R11G11B10F precision" by Bart Wronski ( https://bartwronski.com/2017/04/02/small-float-formats-r11g11b10f-precision/ )
			R16G16B16A16F	  = 6,	///< 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
			R32G32B32A32F	  = 7,	///< 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
			BC1				  = 8,	///< DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block)
			BC1_SRGB		  = 9,	///< DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
			BC2				  = 10,	///< DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
			BC2_SRGB		  = 11,	///< DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
			BC3				  = 12,	///< DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
			BC3_SRGB		  = 13,	///< DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
			BC4				  = 14,	///< 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block)
			BC5				  = 15,	///< 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block)
			ETC1			  = 16,	///< 3 component texture compression meant for mobile devices
			R16_UNORM		  = 17,	///< 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
			R32_UINT		  = 18,	///< 32-bit unsigned integer format
			R32_FLOAT		  = 19,	///< 32-bit float format
			D32_FLOAT		  = 20,	///< 32-bit float depth format
			R16G16_SNORM	  = 21,	///< A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
			R16G16_FLOAT	  = 22,	///< A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
			UNKNOWN			  = 23,	///< Unknown
			NUMBER_OF_FORMATS = 24	///< Number of texture formats
		};

		/**
		*  @brief
		*    Return whether or not the given "Rhi::TextureFormat" is a compressed format
		*
		*  @param[in] textureFormat
		*    "Rhi::TextureFormat" to check
		*
		*  @return
		*    "true" if the given "Rhi::TextureFormat" is a compressed format, else "false"
		*/
		[[nodiscard]] static inline bool isCompressed(Enum textureFormat)
		{
			static constexpr bool MAPPING[] =
			{
				false,	// Rhi::TextureFormat::R8            - 8-bit pixel format, all bits red
				false,	// Rhi::TextureFormat::R8G8B8        - 24-bit pixel format, 8 bits for red, green and blue
				false,	// Rhi::TextureFormat::R8G8B8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
				false,	// Rhi::TextureFormat::R8G8B8A8_SRGB - 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				false,	// Rhi::TextureFormat::B8G8R8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
				false,	// Rhi::TextureFormat::R11G11B10F    - 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent
				false,	// Rhi::TextureFormat::R16G16B16A16F - 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
				false,	// Rhi::TextureFormat::R32G32B32A32F - 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
				true,	// Rhi::TextureFormat::BC1           - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) - when being uncompressed
				true,	// Rhi::TextureFormat::BC1_SRGB      - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				true,	// Rhi::TextureFormat::BC2           - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed
				true,	// Rhi::TextureFormat::BC2_SRGB      - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				true,	// Rhi::TextureFormat::BC3           - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed
				true,	// Rhi::TextureFormat::BC3_SRGB      - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				true,	// Rhi::TextureFormat::BC4           - 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block) - when being uncompressed
				true,	// Rhi::TextureFormat::BC5           - 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block) - when being uncompressed
				true,	// Rhi::TextureFormat::ETC1          - 3 component texture compression meant for mobile devices
				false,	// Rhi::TextureFormat::R16_UNORM     - 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
				false,	// Rhi::TextureFormat::R32_UINT      - 32-bit unsigned integer format
				false,	// Rhi::TextureFormat::R32_FLOAT     - 32-bit float format
				false,	// Rhi::TextureFormat::D32_FLOAT     - 32-bit float depth format
				false,	// Rhi::TextureFormat::R16G16_SNORM  - A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
				false,	// Rhi::TextureFormat::R16G16_FLOAT  - A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
				false	// Rhi::TextureFormat::UNKNOWN       - Unknown
			};
			return MAPPING[textureFormat];
		}

		/**
		*  @brief
		*    Return whether or not the given "Rhi::TextureFormat" is a depth format
		*
		*  @param[in] textureFormat
		*    "Rhi::TextureFormat" to check
		*
		*  @return
		*    "true" if the given "Rhi::TextureFormat" is a depth format, else "false"
		*/
		[[nodiscard]] static inline bool isDepth(Enum textureFormat)
		{
			static constexpr bool MAPPING[] =
			{
				false,	// Rhi::TextureFormat::R8            - 8-bit pixel format, all bits red
				false,	// Rhi::TextureFormat::R8G8B8        - 24-bit pixel format, 8 bits for red, green and blue
				false,	// Rhi::TextureFormat::R8G8B8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
				false,	// Rhi::TextureFormat::R8G8B8A8_SRGB - 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				false,	// Rhi::TextureFormat::B8G8R8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
				false,	// Rhi::TextureFormat::R11G11B10F    - 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent
				false,	// Rhi::TextureFormat::R16G16B16A16F - 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
				false,	// Rhi::TextureFormat::R32G32B32A32F - 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
				false,	// Rhi::TextureFormat::BC1           - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) - when being uncompressed
				false,	// Rhi::TextureFormat::BC1_SRGB      - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				false,	// Rhi::TextureFormat::BC2           - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed
				false,	// Rhi::TextureFormat::BC2_SRGB      - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				false,	// Rhi::TextureFormat::BC3           - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed
				false,	// Rhi::TextureFormat::BC3_SRGB      - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				false,	// Rhi::TextureFormat::BC4           - 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block) - when being uncompressed
				false,	// Rhi::TextureFormat::BC5           - 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block) - when being uncompressed
				false,	// Rhi::TextureFormat::ETC1          - 3 component texture compression meant for mobile devices
				false,	// Rhi::TextureFormat::R16_UNORM     - 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
				false,	// Rhi::TextureFormat::R32_UINT      - 32-bit unsigned integer format
				false,	// Rhi::TextureFormat::R32_FLOAT     - 32-bit float format
				true,	// Rhi::TextureFormat::D32_FLOAT     - 32-bit float depth format
				false,	// Rhi::TextureFormat::R16G16_SNORM  - A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
				false,	// Rhi::TextureFormat::R16G16_FLOAT  - A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
				false	// Rhi::TextureFormat::UNKNOWN       - Unknown
			};
			return MAPPING[textureFormat];
		}

		/**
		*  @brief
		*    "Rhi::TextureFormat" to number of bytes per element
		*
		*  @param[in] textureFormat
		*    "Rhi::TextureFormat" to map
		*
		*  @return
		*    Number of bytes per element
		*/
		[[nodiscard]] static inline uint32_t getNumberOfBytesPerElement(Enum textureFormat)
		{
			static constexpr uint32_t MAPPING[] =
			{
				sizeof(uint8_t),		// Rhi::TextureFormat::R8            - 8-bit pixel format, all bits red
				sizeof(uint8_t) * 3,	// Rhi::TextureFormat::R8G8B8        - 24-bit pixel format, 8 bits for red, green and blue
				sizeof(uint8_t) * 4,	// Rhi::TextureFormat::R8G8B8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
				sizeof(uint8_t) * 4,	// Rhi::TextureFormat::R8G8B8A8_SRGB - 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				sizeof(uint8_t) * 4,	// Rhi::TextureFormat::B8G8R8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
				sizeof(float),			// Rhi::TextureFormat::R11G11B10F    - 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent
				sizeof(float) * 2,		// Rhi::TextureFormat::R16G16B16A16F - 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
				sizeof(float) * 4,		// Rhi::TextureFormat::R32G32B32A32F - 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
				sizeof(uint8_t) * 3,	// Rhi::TextureFormat::BC1           - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) - when being uncompressed
				sizeof(uint8_t) * 3,	// Rhi::TextureFormat::BC1_SRGB      - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				sizeof(uint8_t) * 4,	// Rhi::TextureFormat::BC2           - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed
				sizeof(uint8_t) * 4,	// Rhi::TextureFormat::BC2_SRGB      - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				sizeof(uint8_t) * 4,	// Rhi::TextureFormat::BC3           - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed
				sizeof(uint8_t) * 4,	// Rhi::TextureFormat::BC3_SRGB      - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - when being uncompressed; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
				sizeof(uint8_t) * 1,	// Rhi::TextureFormat::BC4           - 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block) - when being uncompressed
				sizeof(uint8_t) * 2,	// Rhi::TextureFormat::BC5           - 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block) - when being uncompressed
				sizeof(uint8_t) * 3,	// Rhi::TextureFormat::ETC1          - 3 component texture compression meant for mobile devices - when being uncompressed
				sizeof(uint16_t),		// Rhi::TextureFormat::R16_UNORM     - 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
				sizeof(uint32_t),		// Rhi::TextureFormat::R32_UINT      - 32-bit unsigned integer format
				sizeof(float),			// Rhi::TextureFormat::R32_FLOAT     - 32-bit float format
				sizeof(float),			// Rhi::TextureFormat::D32_FLOAT     - 32-bit float depth format
				sizeof(uint32_t),		// Rhi::TextureFormat::R16G16_SNORM  - A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
				sizeof(float),			// Rhi::TextureFormat::R16G16_FLOAT  - A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
				0						// Rhi::TextureFormat::UNKNOWN       - Unknown
			};
			return MAPPING[textureFormat];
		}

		/**
		*  @brief
		*    "Rhi::TextureFormat" to number of bytes per row
		*
		*  @param[in] textureFormat
		*    "Rhi::TextureFormat" to map
		*  @param[in] width
		*    Row width
		*
		*  @return
		*    Number of bytes per row
		*/
		[[nodiscard]] static inline uint32_t getNumberOfBytesPerRow(Enum textureFormat, uint32_t width)
		{
			switch (textureFormat)
			{
				// 8-bit pixel format, all bits red
				case R8:
					return width;

				// 24-bit pixel format, 8 bits for red, green and blue
				case R8G8B8:
					return 3 * width;

				// 32-bit pixel format, 8 bits for red, green, blue and alpha
				case R8G8B8A8:
				case R8G8B8A8_SRGB:
				case B8G8R8A8:
					return 4 * width;

				// 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent
				case R11G11B10F:
					return 4 * width;

				// 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
				case R16G16B16A16F:
					return 8 * width;

				// 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
				case R32G32B32A32F:
					return 16 * width;

				// DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block)
				case BC1:
				case BC1_SRGB:
					return ((width + 3) >> 2) * 8;

				// DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
				case BC2:
				case BC2_SRGB:
					return ((width + 3) >> 2) * 16;

				// DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
				case BC3:
				case BC3_SRGB:
					return ((width + 3) >> 2) * 16;

				// 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block)
				case BC4:
					return ((width + 3) >> 2) * 8;

				// 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block)
				case BC5:
					return ((width + 3) >> 2) * 16;

				// 3 component texture compression meant for mobile devices
				case ETC1:
					return (width >> 1);

				// 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
				case R16_UNORM:
					return sizeof(uint16_t) * width;

				// 32-bit unsigned integer format
				case R32_UINT:
					return sizeof(uint32_t) * width;

				// 32-bit float red/depth format
				case R32_FLOAT:
				case D32_FLOAT:
					return sizeof(float) * width;

				// A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
				case R16G16_SNORM:
					return sizeof(uint32_t) * width;

				// A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
				case R16G16_FLOAT:
					return sizeof(float) * width;

				// Unknown
				case UNKNOWN:
				case NUMBER_OF_FORMATS:
					return 0;

				default:
					return 0;
			}
		}

		/**
		*  @brief
		*    "Rhi::TextureFormat" to number of bytes per slice
		*
		*  @param[in] textureFormat
		*    "Rhi::TextureFormat" to map
		*  @param[in] width
		*    Slice width
		*  @param[in] height
		*    Slice height
		*
		*  @return
		*    Number of bytes per slice
		*/
		[[nodiscard]] static inline uint32_t getNumberOfBytesPerSlice(Enum textureFormat, uint32_t width, uint32_t height)
		{
			switch (textureFormat)
			{
				// 8-bit pixel format, all bits red
				case R8:
					return width * height;

				// 24-bit pixel format, 8 bits for red, green and blue
				case R8G8B8:
					return 3 * width * height;

				// 32-bit pixel format, 8 bits for red, green, blue and alpha
				case R8G8B8A8:
				case R8G8B8A8_SRGB:
				case B8G8R8A8:
					return 4 * width * height;

				// 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent
				case R11G11B10F:
					return 4 * width * height;

				// 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
				case R16G16B16A16F:
					return 8 * width * height;

				// 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
				case R32G32B32A32F:
					return 16 * width * height;

				// DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block)
				case BC1:
				case BC1_SRGB:
					return ((width + 3) >> 2) * ((height + 3) >> 2) * 8;

				// DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
				case BC2:
				case BC2_SRGB:
					return ((width + 3) >> 2) * ((height + 3) >> 2) * 16;

				// DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
				case BC3:
				case BC3_SRGB:
					return ((width + 3) >> 2) * ((height + 3) >> 2) * 16;

				// 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block)
				case BC4:
					return ((width + 3) >> 2) * ((height + 3) >> 2) * 8;

				// 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block)
				case BC5:
					return ((width + 3) >> 2) * ((height + 3) >> 2) * 16;

				// 3 component texture compression meant for mobile devices
				case ETC1:
				{
					const uint32_t numberOfBytesPerSlice = (width * height) >> 1;
					return (numberOfBytesPerSlice > 8) ? numberOfBytesPerSlice : 8;
				}

				// 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
				case R16_UNORM:
					return sizeof(uint16_t) * width * height;

				// 32-bit unsigned integer format
				case R32_UINT:
					return sizeof(uint32_t) * width * height;

				// 32-bit float depth format
				case R32_FLOAT:
				case D32_FLOAT:
					return sizeof(float) * width * height;

				// A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
				case R16G16_SNORM:
					return sizeof(uint32_t) * width * height;

				// A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
				case R16G16_FLOAT:
					return sizeof(float) * width * height;

				// Unknown
				case UNKNOWN:
				case NUMBER_OF_FORMATS:
					return 0;

				default:
					return 0;
			}
		}

	};

	/**
	*  @brief
	*    Texture flags
	*/
	struct TextureFlag final
	{
		enum Enum : uint32_t
		{
			UNORDERED_ACCESS      = 1 << 0,	///< This texture can be used for unordered access which is needed for compute shader read/write textures (when using Direct3D 11 a unordered access view (UAV) will be generated)
			SHADER_RESOURCE       = 1 << 1,	///< This texture can be used as shader resource (when using Direct3D 11 a shader resource view (SRV) will be generated)
			RENDER_TARGET         = 1 << 2,	///< This texture can be used as framebuffer object (FBO) attachment render target
			DATA_CONTAINS_MIPMAPS = 1 << 3,	///< The user provided data containing mipmaps from 0-n down to 1x1 linearly in memory
			GENERATE_MIPMAPS      = 1 << 4	///< Allocate mipmap chain, automatically generate mipmaps if static data has been provided (avoid this if you can, will be ignored in case the "DATA_CONTAINS_MIPMAPS"-flag is set), for depth textures the mipmaps can only be allocated but not automatically be generated
		};
	};

	/**
	*  @brief
	*    Texture usage indication
	*
	*  @note
	*    - Only relevant for Direct3D, OpenGL has no texture usage indication
	*    - Original Direct3D comments from http://msdn.microsoft.com/en-us/library/windows/desktop/ff476259%28v=vs.85%29.aspx are used in here
	*    - These constants directly map to Direct3D 10 & 11 constants, do not change them
	*/
	enum class TextureUsage
	{
		DEFAULT   = 0,	///< A resource that requires read and write access by the GPU. This is likely to be the most common usage choice.
		IMMUTABLE = 1,	///< A resource that can only be read by the GPU. It cannot be written by the GPU, and cannot be accessed at all by the CPU. This type of resource must be initialized when it is created, since it cannot be changed after creation.
		DYNAMIC   = 2,	///< A resource that is accessible by both the GPU (read only) and the CPU (write only). A dynamic resource is a good choice for a resource that will be updated by the CPU at least once per frame. To update a dynamic resource, use a map method.
		STAGING   = 3	///< A resource that supports data transfer (copy) from the GPU to the CPU.
	};

	/**
	*  @brief
	*    Optimized clear value
	*
	*  @see
	*    - "ID3D12Device::CreateCommittedResource method" documentation at https://msdn.microsoft.com/de-de/library/windows/desktop/dn899178%28v=vs.85%29.aspx
	*/
	struct OptimizedTextureClearValue final
	{
		union
		{
			float color[4];
			struct
			{
				float depth;
				uint8_t stencil;
			} DepthStencil;
		};
	};




	//[-------------------------------------------------------]
	//[ Rhi/State/BlendStateTypes.h                           ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Blend
	*
	*  @note
	*    - These constants directly map to Direct3D 10 & 11 & 12 constants, do not change them
	*
	*  @see
	*    - "D3D12_BLEND"-documentation for details
	*/
	enum class Blend
	{
		ZERO			 = 1,
		ONE				 = 2,
		SRC_COLOR		 = 3,
		INV_SRC_COLOR	 = 4,
		SRC_ALPHA		 = 5,
		INV_SRC_ALPHA	 = 6,
		DEST_ALPHA		 = 7,
		INV_DEST_ALPHA	 = 8,
		DEST_COLOR		 = 9,
		INV_DEST_COLOR	 = 10,
		SRC_ALPHA_SAT	 = 11,
		BLEND_FACTOR	 = 14,
		INV_BLEND_FACTOR = 15,
		SRC_1_COLOR		 = 16,
		INV_SRC_1_COLOR	 = 17,
		SRC_1_ALPHA		 = 18,
		INV_SRC_1_ALPHA	 = 19
	};

	/**
	*  @brief
	*    Blend operation
	*
	*  @note
	*    - These constants directly map to Direct3D 10 & 11 constants, do not change them
	*
	*  @see
	*    - "D3D12_BLEND_OP"-documentation for details
	*/
	enum class BlendOp
	{
		ADD			 = 1,
		SUBTRACT	 = 2,
		REV_SUBTRACT = 3,
		MIN			 = 4,
		MAX			 = 5
	};

	/**
	*  @brief
	*    Render target blend description
	*
	*  @note
	*    - This render target blend description maps directly to Direct3D 10.1 & 11, do not change it
	*    - This also means that "int" is used over "bool" because in Direct3D it's defined this way
	*    - If you want to know how the default values were chosen, have a look into the "Rhi::BlendStateBuilder::getDefaultBlendState()"-implementation
	*
	*  @see
	*    - "D3D12_RENDER_TARGET_BLEND_DESC"-documentation for details
	*/
	struct RenderTargetBlendDesc final
	{
		int		blendEnable;			///< Boolean value. Default: "false"
		Blend	srcBlend;				///< Default: "Rhi::Blend::ONE"
		Blend	destBlend;				///< Default: "Rhi::Blend::ZERO"
		BlendOp	blendOp;				///< Default: "Rhi::BlendOp::ADD"
		Blend	srcBlendAlpha;			///< Default: "Rhi::Blend::ONE"
		Blend	destBlendAlpha;			///< Default: "Rhi::Blend::ZERO"
		BlendOp	blendOpAlpha;			///< Default: "Rhi::BlendOp::ADD"
		uint8_t	renderTargetWriteMask;	///< Combination of "Rhi::ColorWriteEnableFlag"-flags. Default: "Rhi::ColorWriteEnableFlag::ALL"
	};

	/**
	*  @brief
	*    Blend state
	*
	*  @note
	*    - This blend state maps directly to Direct3D 10.1 & 11 & 12, do not change it
	*    - This also means that "int" is used over "bool" because in Direct3D it's defined this way
	*    - If you want to know how the default values were chosen, have a look into the "Rhi::BlendStateBuilder::getDefaultBlendState()"-implementation
	*
	*  @see
	*    - "D3D12_BLEND_DESC"-documentation for details
	*/
	struct BlendState final
	{
		int					  alphaToCoverageEnable;	///< Boolean value. Default: "false"
		int					  independentBlendEnable;	///< Boolean value. Default: "false"
		RenderTargetBlendDesc renderTarget[8];			///< Default: See "Rhi::RenderTargetBlendDesc"
	};
	struct BlendStateBuilder final
	{
		/**
		*  @brief
		*    Return the default blend state
		*
		*  @return
		*    The default blend state, see "Rhi::BlendState" for the default values
		*/
		[[nodiscard]] static inline const BlendState& getDefaultBlendState()
		{
			// As default values, the one of Direct3D 11 and Direct 10 were chosen in order to make it easier for those RHI implementations
			// (choosing OpenGL default values would bring no benefit due to the design of the OpenGL API)
			// - Direct3D 11 "D3D11_BLEND_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/ff476087%28v=vs.85%29.aspx
			// - Direct3D 10 "D3D10_BLEND_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb204893%28v=vs.85%29.aspx
			// - Direct3D 9 "D3DRENDERSTATETYPE enumeration"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb172599%28v=vs.85%29.aspx
			// - OpenGL & OpenGL ES 3: The official specifications (unlike Direct3D, OpenGL versions are more compatible to each other)

			// Return default values
			// TODO(co) Finish default state comments
			static constexpr BlendState BLEND_STATE =
			{																															//	Direct3D 11	Direct3D 10	Direct3D 9			OpenGL
				false,								// alphaToCoverageEnable (int)															"false"			"false"
				false,								// independentBlendEnable (int)															"false"			"false"
			
				{ // renderTarget[8]
					// renderTarget[0]
					{ 
						false,						// blendEnable (int)																	"false"			"false"
						Blend::ONE,					// srcBlend (Rhi::Blend)																"ONE"			"ONE"
						Blend::ZERO,				// destBlend (Rhi::Blend)																"ZERO"			"ZERO"
						BlendOp::ADD,				// blendOp (Rhi::BlendOp)																"ADD"			"ADD"
						Blend::ONE,					// srcBlendAlpha (Rhi::Blend)															"ONE"			"ONE"
						Blend::ZERO,				// destBlendAlpha (Rhi::Blend)															"ZERO"			"ZERO"
						BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)															"ADD"			"ADD"
						ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
					},
					// renderTarget[1]
					{
						false,						// blendEnable (int)																	"false"			"false"
						Blend::ONE,					// srcBlend (Rhi::Blend)																"ONE"			"ONE"
						Blend::ZERO,				// destBlend (Rhi::Blend)																"ZERO"			"ZERO"
						BlendOp::ADD,				// blendOp (Rhi::BlendOp)																"ADD"			"ADD"
						Blend::ONE,					// srcBlendAlpha (Rhi::Blend)															"ONE"			"ONE"
						Blend::ZERO,				// destBlendAlpha (Rhi::Blend)															"ZERO"			"ZERO"
						BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)															"ADD"			"ADD"
						ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
					},
					// renderTarget[2]
					{
						false,						// blendEnable (int)																	"false"			"false"
						Blend::ONE,					// srcBlend (Rhi::Blend)																"ONE"			"ONE"
						Blend::ZERO,				// destBlend (Rhi::Blend)																"ZERO"			"ZERO"
						BlendOp::ADD,				// blendOp (Rhi::BlendOp)																"ADD"			"ADD"
						Blend::ONE,					// srcBlendAlpha (Rhi::Blend)															"ONE"			"ONE"
						Blend::ZERO,				// destBlendAlpha (Rhi::Blend)															"ZERO"			"ZERO"
						BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)															"ADD"			"ADD"
						ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
					},
					// renderTarget[3]
					{
						false,						// blendEnable (int)																	"false"			"false"
						Blend::ONE,					// srcBlend (Rhi::Blend)																"ONE"			"ONE"
						Blend::ZERO,				// destBlend (Rhi::Blend)																"ZERO"			"ZERO"
						BlendOp::ADD,				// blendOp (Rhi::BlendOp)																"ADD"			"ADD"
						Blend::ONE,					// srcBlendAlpha (Rhi::Blend)															"ONE"			"ONE"
						Blend::ZERO,				// destBlendAlpha (Rhi::Blend)															"ZERO"			"ZERO"
						BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)															"ADD"			"ADD"
						ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
					},
					// renderTarget[4]
					{
						false,						// blendEnable (int)																	"false"			"false"
						Blend::ONE,					// srcBlend (Rhi::Blend)																"ONE"			"ONE"
						Blend::ZERO,				// destBlend (Rhi::Blend)																"ZERO"			"ZERO"
						BlendOp::ADD,				// blendOp (Rhi::BlendOp)																"ADD"			"ADD"
						Blend::ONE,					// srcBlendAlpha (Rhi::Blend)															"ONE"			"ONE"
						Blend::ZERO,				// destBlendAlpha (Rhi::Blend)															"ZERO"			"ZERO"
						BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)															"ADD"			"ADD"
						ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
					},
					// renderTarget[5]
					{
						false,						// blendEnable (int)																	"false"			"false"
						Blend::ONE,					// srcBlend (Rhi::Blend)																"ONE"			"ONE"
						Blend::ZERO,				// destBlend (Rhi::Blend)																"ZERO"			"ZERO"
						BlendOp::ADD,				// blendOp (Rhi::BlendOp)																"ADD"			"ADD"
						Blend::ONE,					// srcBlendAlpha (Rhi::Blend)															"ONE"			"ONE"
						Blend::ZERO,				// destBlendAlpha (Rhi::Blend)															"ZERO"			"ZERO"
						BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)															"ADD"			"ADD"
						ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
					},
					// renderTarget[6]
					{
						false,						// blendEnable (int)																	"false"			"false"
						Blend::ONE,					// srcBlend (Rhi::Blend)																"ONE"			"ONE"
						Blend::ZERO,				// destBlend (Rhi::Blend)																"ZERO"			"ZERO"
						BlendOp::ADD,				// blendOp (Rhi::BlendOp)																"ADD"			"ADD"
						Blend::ONE,					// srcBlendAlpha (Rhi::Blend)															"ONE"			"ONE"
						Blend::ZERO,				// destBlendAlpha (Rhi::Blend)															"ZERO"			"ZERO"
						BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)															"ADD"			"ADD"
						ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
					},
					// renderTarget[7]
					{
						false,						// blendEnable (int)																	"false"			"false"
						Blend::ONE,					// srcBlend (Rhi::Blend)																"ONE"			"ONE"
						Blend::ZERO,				// destBlend (Rhi::Blend)																"ZERO"			"ZERO"
						BlendOp::ADD,				// blendOp (Rhi::BlendOp)																"ADD"			"ADD"
						Blend::ONE,					// srcBlendAlpha (Rhi::Blend)															"ONE"			"ONE"
						Blend::ZERO,				// destBlendAlpha (Rhi::Blend)															"ZERO"			"ZERO"
						BlendOp::ADD,				// blendOpAlpha (Rhi::BlendOp)															"ADD"			"ADD"
						ColorWriteEnableFlag::ALL	// renderTargetWriteMask (uint8_t), combination of "Rhi::ColorWriteEnableFlag"-flags	"ALL"			"ALL"
					}
				}
			};
			return BLEND_STATE;
		}
	};




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/BufferTypes.h                              ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Buffer flags
	*/
	struct BufferFlag final
	{
		enum Enum
		{
			UNORDERED_ACCESS = 1 << 0,	///< This buffer can be used for unordered access which is needed for compute shader read/write buffers (when using Direct3D 11 a unordered access view (UAV) will be generated)
			SHADER_RESOURCE  = 1 << 1	///< This buffer can be used as shader resource (when using Direct3D 11 a shader resource view (SRV) will be generated)
		};
	};

	/**
	*  @brief
	*    Indirect buffer flags
	*/
	struct IndirectBufferFlag final
	{
		enum Enum
		{
			UNORDERED_ACCESS	   = 1 << 0,	///< This indirect buffer can be used for unordered access which is needed for compute shader read/write indirect buffers (when using Direct3D 11 a unordered access view (UAV) will be generated)
			SHADER_RESOURCE		   = 1 << 1,	///< This indirect buffer can be used as shader resource (when using Direct3D 11 a shader resource view (SRV) will be generated)
			DRAW_ARGUMENTS		   = 1 << 2,	///< This indirect buffer contains "Rhi::DrawArguments" as elements, if "Rhi::IndirectBufferFlag::DRAW_ARGUMENTS" is set it's invalid to have "Rhi::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS" set as well
			DRAW_INDEXED_ARGUMENTS = 1 << 3		///< This indirect buffer contains "Rhi::DrawIndexedArguments" as elements, if "Rhi::IndirectBufferFlag::DRAW_ARGUMENTS" is set it's invalid to have "Rhi::IndirectBufferFlag::DRAW_INDEXED_ARGUMENTS" set as well
		};
	};

	/**
	*  @brief
	*    Buffer usage indication
	*
	*  @note
	*    - Direct3D and OpenGL ES 3 have less fine granular usage settings, in this case the usage will be mapped to the closed match
	*    - Original OpenGL "GL_ARB_vertex_buffer_object" extension comments from http://www.opengl.org/registry/specs/ARB/vertex_buffer_object.txt are used in here
	*    - These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
	*    - Most common usages: "STREAM_DRAW", "STATIC_DRAW" and "DYNAMIC_DRAW"
	*/
	enum class BufferUsage
	{
		STREAM_DRAW  = 0x88E0,	///< The data store contents will be specified once by the application, and used at most a few times as the source of a OpenGL (drawing) command. (also exists in OpenGL ES 3)
		STREAM_READ  = 0x88E1,	///< The data store contents will be specified once by reading data from the OpenGL, and queried at most a few times by the application.
		STREAM_COPY  = 0x88E2,	///< The data store contents will be specified once by reading data from the OpenGL, and used at most a few times as the source of a OpenGL (drawing) command.
		STATIC_DRAW  = 0x88E4,	///< The data store contents will be specified once by the application, and used many times as the source for OpenGL (drawing) commands. (also exists in OpenGL ES 3)
		STATIC_READ  = 0x88E5,	///< The data store contents will be specified once by reading data from the OpenGL, and queried many times by the application.
		STATIC_COPY  = 0x88E6,	///< The data store contents will be specified once by reading data from the OpenGL, and used many times as the source for OpenGL (drawing) commands.
		DYNAMIC_DRAW = 0x88E8,	///< The data store contents will be respecified repeatedly by the application, and used many times as the source for OpenGL (drawing) commands. (also exists in OpenGL ES 3)
		DYNAMIC_READ = 0x88E9,	///< The data store contents will be respecified repeatedly by reading data from the OpenGL, and queried many times by the application.
		DYNAMIC_COPY = 0x88EA	///< The data store contents will be respecified repeatedly by reading data from the OpenGL, and used many times as the source for OpenGL (drawing) commands.
	};




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/VertexArrayTypes.h                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vertex attribute format
	*/
	enum class VertexAttributeFormat : uint8_t
	{
		FLOAT_1			= 0,	///< Float 1 (one component per element, 32 bit floating point per component), supported by DirectX 9, DirectX 10, DirectX 11, OpenGL and OpenGL ES 3
		FLOAT_2			= 1,	///< Float 2 (two components per element, 32 bit floating point per component), supported by DirectX 9, DirectX 10, DirectX 11, OpenGL and OpenGL ES 3
		FLOAT_3			= 2,	///< Float 3 (three components per element, 32 bit floating point per component), supported by DirectX 9, DirectX 10, DirectX 11, OpenGL and OpenGL ES 3
		FLOAT_4			= 3,	///< Float 4 (four components per element, 32 bit floating point per component), supported by DirectX 9, DirectX 10, DirectX 11, OpenGL and OpenGL ES 3
		R8G8B8A8_UNORM	= 4,	///< Unsigned byte 4 (four components per element, 8 bit integer per component), will be passed in a normalized form into shaders, supported by DirectX 9, DirectX 10, DirectX 11, OpenGL and OpenGL ES 3
		R8G8B8A8_UINT	= 5,	///< Unsigned byte 4 (four components per element, 8 bit integer per component), supported by DirectX 9, DirectX 10, DirectX 11, OpenGL and OpenGL ES 3
		SHORT_2			= 6,	///< Short 2 (two components per element, 16 bit integer per component), supported by DirectX 9, DirectX 10, DirectX 11, OpenGL and OpenGL ES 3
		SHORT_4			= 7,	///< Short 4 (four components per element, 16 bit integer per component), supported by DirectX 9, DirectX 10, DirectX 11, OpenGL and OpenGL ES 3
		UINT_1			= 8		///< Unsigned integer 1 (one components per element, 32 bit unsigned integer per component), supported by DirectX 10, DirectX 11, OpenGL and OpenGL ES 3
	};
	/**
	*  @brief
	*    Vertex attribute ("Input element description" in Direct3D terminology)
	*
	*  @note
	*    - This piece of data is POD and can be serialized/deserialized as a whole (hence the byte alignment compiler setting)
	*/
	#pragma pack(push)
	#pragma pack(1)
		struct VertexAttribute final
		{
			// Data destination
			VertexAttributeFormat vertexAttributeFormat;	///< Vertex attribute format
			char				  name[32];					///< Vertex attribute name
			char				  semanticName[32];			///< Vertex attribute semantic name
			uint32_t			  semanticIndex;			///< Vertex attribute semantic index
			// Data source
			uint32_t			  inputSlot;				///< Index of the vertex input slot to use (see "Rhi::VertexArrayVertexBuffer")
			uint32_t			  alignedByteOffset;		///< Offset (in bytes) from the start of the vertex to this certain attribute
			uint32_t			  strideInBytes;			///< Specifies the size in bytes of each vertex entry
			uint32_t			  instancesPerElement;		/**< Number of instances to draw with the same data before advancing in the buffer by one element.
																 0 for no instancing meaning the data is per-vertex instead of per-instance, 1 for drawing one
																 instance with the same data, 2 for drawing two instances with the same data and so on.
																 Instanced arrays is a shader model 3 feature, only supported if "Rhi::Capabilities::instancedArrays" is true.
																 In order to support Direct3D 9, do not use this within the first attribute. */
		};
	#pragma pack(pop)

	/**
	*  @brief
	*    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
	*
	*  @see
	*    - "Rhi::IVertexArray" class documentation
	*/
	#pragma pack(push)
	#pragma pack(1)
		struct VertexAttributes final
		{
			// Public data
				  uint32_t		   numberOfAttributes;	///< Number of attributes (position, color, texture coordinate, normal...), having zero attributes is valid
			const VertexAttribute* attributes;			///< At least "numberOfAttributes" instances of vertex array attributes, can be a null pointer in case there are zero attributes, the data is internally copied and you have to free your memory if you no longer need it

			// Public methods
			inline VertexAttributes() :
				numberOfAttributes(0),
				attributes(nullptr)
			{}
			inline VertexAttributes(uint32_t _numberOfAttributes, const VertexAttribute* _attributes) :
				numberOfAttributes(_numberOfAttributes),
				attributes(_attributes)
			{}
		};
	#pragma pack(pop)

	/**
	*  @brief
	*    Vertex array vertex buffer
	*
	*  @see
	*    - "Rhi::IVertexArray" class documentation
	*/
	struct VertexArrayVertexBuffer final
	{
		IVertexBuffer* vertexBuffer;	///< Vertex buffer used at this vertex input slot (vertex array instances keep a reference to the vertex buffers used by the vertex array attributes, see "Rhi::IRhi::createVertexArray()" for details)
	};




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/IndexBufferTypes.h                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Index buffer data format
	*/
	struct IndexBufferFormat final
	{
		enum Enum
		{
			UNSIGNED_CHAR  = 0,	///< One byte per element, uint8_t (may not be supported by each RHI implementation, primarily for mobile devices)
			UNSIGNED_SHORT = 1,	///< Two bytes per element, uint16_t (best support across multiple RHI implementations)
			UNSIGNED_INT   = 2	///< Four bytes per element, uint32_t (may not be supported by each RHI implementation)
		};

		/**
		*  @brief
		*    "Rhi::IndexBufferFormat" to number of bytes per element
		*
		*  @param[in] indexFormat
		*    "Rhi::IndexBufferFormat" to map
		*
		*  @return
		*    Number of bytes per element
		*/
		[[nodiscard]] static inline uint32_t getNumberOfBytesPerElement(Enum indexFormat)
		{
			static constexpr uint32_t MAPPING[] =
			{
				1,	// One byte per element, uint8_t (may not be supported by each RHI implementation, primarily for mobile devices)
				2,	// Two bytes per element, uint16_t (best support across multiple RHI implementations)
				4	// Four bytes per element, uint32_t (may not be supported by each RHI implementation)
			};
			return MAPPING[indexFormat];
		}
	};




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/IndirectBufferTypes.h                      ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Draw instanced arguments
	*
	*  @note
	*    - This structure directly maps to Direct3D 12, DirectX 11, Vulkan, Metal and OpenGL, do not change it
	*    - Direct3D 12: "D3D12_DRAW_ARGUMENTS"
	*    - Direct3D 11: No structure documentation found, only indications that same arguments and order as "ID3D11DeviceContext::DrawInstanced()"
	*    - Vulkan: "VkDrawIndirectCommand"
	*    - Metal: "MTLDrawPrimitivesIndirectArguments"
	*    - OpenGL:"DrawArraysIndirectCommand"
	*
	*  @see
	*    - "D3D12_DRAW_ARGUMENTS"-documentation for details
	*/
	struct DrawArguments final
	{
		uint32_t vertexCountPerInstance;
		uint32_t instanceCount;
		uint32_t startVertexLocation;
		uint32_t startInstanceLocation;
		inline DrawArguments(uint32_t _vertexCountPerInstance, uint32_t _instanceCount = 1, uint32_t _startVertexLocation = 0, uint32_t _startInstanceLocation = 0) :
			vertexCountPerInstance(_vertexCountPerInstance),
			instanceCount(_instanceCount),
			startVertexLocation(_startVertexLocation),
			startInstanceLocation(_startInstanceLocation)
		{}
	};

	/**
	*  @brief
	*    Draw indexed instanced arguments
	*
	*  @note
	*    - This structure directly maps to Direct3D 12, DirectX 11, Vulkan, Metal and OpenGL, do not change it
	*    - Direct3D 12: "D3D12_DRAW_INDEXED_ARGUMENTS"
	*    - Direct3D 11: No structure documentation found, only indications that same arguments and order as "ID3D11DeviceContext::DrawIndexedInstanced()"
	*    - Vulkan: "VkDrawIndexedIndirectCommand"
	*    - Metal: "MTLDrawIndexedPrimitivesIndirectArguments"
	*    - OpenGL:"DrawElementsIndirectCommand"
	*
	*  @see
	*    - "D3D12_DRAW_INDEXED_ARGUMENTS"-documentation for details
	*/
	struct DrawIndexedArguments final
	{
		uint32_t indexCountPerInstance;
		uint32_t instanceCount;
		uint32_t startIndexLocation;
		int32_t  baseVertexLocation;
		uint32_t startInstanceLocation;
		inline DrawIndexedArguments(uint32_t _indexCountPerInstance, uint32_t _instanceCount = 1, uint32_t _startIndexLocation = 0, int32_t _baseVertexLocation = 0, uint32_t _startInstanceLocation = 0) :
			indexCountPerInstance(_indexCountPerInstance),
			instanceCount(_instanceCount),
			startIndexLocation(_startIndexLocation),
			baseVertexLocation(_baseVertexLocation),
			startInstanceLocation(_startInstanceLocation)
		{}
	};

	/**
	*  @brief
	*    Draw mesh tasks arguments
	*
	*  @note
	*    - This structure directly maps to Vulkan and OpenGL, do not change it
	*    - Vulkan: "VkDrawMeshTasksIndirectCommandNV"
	*    - OpenGL:"DrawMeshTasksIndirectCommandNV"
	*    - Direct3D 12 isn't compatible since it's using "D3D12_DISPATCH_ARGUMENTS" ("ThreadGroupCountX", "ThreadGroupCountY", "ThreadGroupCountZ")
	*
	*  @see
	*    - "VkDrawMeshTasksIndirectCommandNV"-documentation for details
	*/
	struct DrawMeshTasksArguments final
	{
		uint32_t numberOfTasks;
		uint32_t firstTask;
		inline DrawMeshTasksArguments(uint32_t _numberOfTasks, uint32_t _firstTask = 0) :
			numberOfTasks(_numberOfTasks),
			firstTask(_firstTask)
		{}
	};


	//[-------------------------------------------------------]
	//[ Rhi/State/RasterizerStateTypes.h                      ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Fill mode
	*
	*  @note
	*    - These constants directly map to Direct3D 10 & 11 & 12 constants, do not change them
	*
	*  @see
	*    - "D3D12_FILL_MODE"-documentation for details
	*/
	enum class FillMode
	{
		WIREFRAME = 2,	///< Wireframe
		SOLID     = 3	///< Solid
	};

	/**
	*  @brief
	*    Cull mode
	*
	*  @note
	*    - These constants directly map to Direct3D 10 & 11 & 12 constants, do not change them
	*
	*  @see
	*    - "D3D12_CULL_MODE"-documentation for details
	*/
	enum class CullMode
	{
		NONE  = 1,	///< No culling
		FRONT = 2,	///< Do not draw triangles that are front-facing
		BACK  = 3	///< Do not draw triangles that are back-facing
	};

	/**
	*  @brief
	*    Conservative rasterization mode
	*
	*  @note
	*    - These constants directly map to Direct3D 12 constants, do not change them
	*
	*  @see
	*    - "D3D12_CONSERVATIVE_RASTERIZATION_MODE"-documentation for details
	*/
	enum class ConservativeRasterizationMode
	{
		OFF	= 0,	///< Conservative rasterization is off
		ON	= 1		///< Conservative rasterization is on
	};

	/**
	*  @brief
	*    Rasterizer state
	*
	*  @note
	*    - This rasterizer state maps directly to Direct3D 12, do not change it
	*    - This also means that "int" is used over "bool" because in Direct3D it's defined this way
	*    - If you want to know how the default values were chosen, have a look into the "Rhi::RasterizerStateBuilder::getDefaultRasterizerState()"-implementation
	*    - Lookout! In Direct3D 12 the scissor test can't be deactivated and hence one always needs to set a valid scissor rectangle.
	*      Use the convenience "Rhi::Command::SetGraphicsViewportAndScissorRectangle"-command if possible to not walk into this Direct3D 12 trap.
	*
	*  @see
	*    - "D3D12_RASTERIZER_DESC"-documentation for details
	*/
	struct RasterizerState final
	{
		FillMode						fillMode;						///< Default: "Rhi::FillMode::SOLID"
		CullMode						cullMode;						///< Default: "Rhi::CullMode::BACK"
		int								frontCounterClockwise;			///< Select counter-clockwise polygons as front-facing? Boolean value. Default: "false"
		int								depthBias;						///< Default: "0"
		float							depthBiasClamp;					///< Default: "0.0f"
		float							slopeScaledDepthBias;			///< Default: "0.0f"
		int								depthClipEnable;				///< Boolean value. Default: "true"
		int								multisampleEnable;				///< Boolean value. Default: "false"
		int								antialiasedLineEnable;			///< Boolean value. Default: "false"
		unsigned int					forcedSampleCount;				///< Default: "0"
		ConservativeRasterizationMode	conservativeRasterizationMode;	///< Boolean value. >= Direct3D 12 only. Default: "false"
		int								scissorEnable;					///< Boolean value. Not available in Vulkan or Direct3D 12 (scissor testing is always enabled). Default: "false"
	};
	struct RasterizerStateBuilder final
	{
		/**
		*  @brief
		*    Return the default rasterizer state
		*
		*  @return
		*    The default rasterizer state, see "Rhi::RasterizerState" for the default values
		*/
		[[nodiscard]] static inline const RasterizerState& getDefaultRasterizerState()
		{
			// As default values, the one of Direct3D 11 and Direct 10 were chosen in order to make it easier for those RHI implementations
			// (choosing OpenGL default values would bring no benefit due to the design of the OpenGL API)
			// - Direct3D 12 "D3D12_RASTERIZER_DESC structure"-documentation at MSDN: https://msdn.microsoft.com/de-de/library/windows/desktop/dn770387%28v=vs.85%29.aspx
			// - Direct3D 11 "D3D11_RASTERIZER_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/ff476198%28v=vs.85%29.aspx
			// - Direct3D 10 "D3D10_RASTERIZER_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb172408(v=vs.85).aspx
			// - Direct3D 9 "D3DRENDERSTATETYPE enumeration"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb172599%28v=vs.85%29.aspx
			// - OpenGL & OpenGL ES 3: The official specifications (unlike Direct3D, OpenGL versions are more compatible to each other)

			// Return default values
			static constexpr RasterizerState RASTERIZER_STATE =
			{																												//	Direct3D 11	Direct3D 10	Direct3D 9		OpenGL
				FillMode::SOLID,					// fillMode (Rhi::FillMode)												"SOLID"			"SOLID"		"SOLID"			"SOLID"
				CullMode::BACK,						// cullMode (Rhi::CullMode)												"BACK"			"Back"		"BACK" (CCW)	"BACK"
				false,								// frontCounterClockwise (int)											"false"			"false"		"false" (CCW)	"true"
				0,									// depthBias (int)														"0"				"0"			"0"				TODO(co)
				0.0f,								// depthBiasClamp (float)												"0.0f"			"0.0f"		<unsupported>	TODO(co)
				0.0f,								// slopeScaledDepthBias (float)											"0.0f"			"0.0f"		"0.0f"			TODO(co)
				true,								// depthClipEnable (int)												"true"			"true"		<unsupported>	TODO(co)
				false,								// multisampleEnable (int)												"false"			"false"		"true"			"true"
				false,								// antialiasedLineEnable (int)											"false"			"false"		"false"			"false"
				0,									// forcedSampleCount (unsigned int)
				ConservativeRasterizationMode::OFF,	// conservativeRasterizationMode (Rhi::ConservativeRasterizationMode)
				false								// scissorEnable (int)													"false"			"false"		"false"			"false"
			};
			return RASTERIZER_STATE;
		}
	};




	//[-------------------------------------------------------]
	//[ Rhi/State/DepthStencilStateTypes.h                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Depth write mask
	*
	*  @note
	*    - These constants directly map to Direct3D 10 & 11 & 12 constants, do not change them
	*
	*  @see
	*    - "D3D12_DEPTH_WRITE_MASK"-documentation for details
	*/
	enum class DepthWriteMask
	{
		ZERO = 0,
		ALL  = 1
	};

	/**
	*  @brief
	*    Stencil operation
	*
	*  @note
	*    - These constants directly map to Direct3D 10 & 11 & 12 constants, do not change them
	*
	*  @see
	*    - "D3D12_STENCIL_OP"-documentation for details
	*/
	enum class StencilOp
	{
		KEEP	 = 1,
		ZERO	 = 2,
		REPLACE	 = 3,
		INCR_SAT = 4,
		DECR_SAT = 5,
		INVERT	 = 6,
		INCREASE = 7,
		DECREASE = 8
	};

	/**
	*  @brief
	*    Depth stencil operation description
	*
	*  @note
	*    - This depth stencil operation description maps directly to Direct3D 10 & 11 & 12, do not change it
	*    - If you want to know how the default values were chosen, have a look into the "Rhi::DepthStencilStateBuilder::getDefaultDepthStencilState()"-implementation
	*
	*  @see
	*    - "D3D12_DEPTH_STENCILOP_DESC"-documentation for details
	*/
	struct DepthStencilOpDesc final
	{
		StencilOp		stencilFailOp;		///< Default: "Rhi::StencilOp::KEEP"
		StencilOp		stencilDepthFailOp;	///< Default: "Rhi::StencilOp::KEEP"
		StencilOp		stencilPassOp;		///< Default: "Rhi::StencilOp::KEEP"
		ComparisonFunc	stencilFunc;		///< Default: "Rhi::ComparisonFunc::ALWAYS"
	};

	/**
	*  @brief
	*    Depth stencil state
	*
	*  @note
	*    - This depth stencil state maps directly to Direct3D 10 & 11 & 12, do not change it
	*    - This also means that "int" is used over "bool" because in Direct3D it's defined this way
	*    - If you want to know how the default values were chosen, have a look into the "Rhi::DepthStencilStateBuilder::getDefaultDepthStencilState()"-implementation
	*
	*  @see
	*    - "D3D12_DEPTH_STENCIL_DESC"-documentation for details
	*/
	struct DepthStencilState final
	{
		int					depthEnable;		///< Boolean value. Default: "true"
		DepthWriteMask		depthWriteMask;		///< Default: "Rhi::DepthWriteMask::ALL"
		ComparisonFunc		depthFunc;			///< Default: "Rhi::ComparisonFunc::GREATER" instead of "Rhi::ComparisonFunc::LESS" due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
		int					stencilEnable;		///< Boolean value. Default: "false"
		uint8_t				stencilReadMask;	///< Default: "0xff"
		uint8_t				stencilWriteMask;	///< Default: "0xff"
		DepthStencilOpDesc	frontFace;			///< Default: See "Rhi::DepthStencilOpDesc"
		DepthStencilOpDesc	backFace;			///< Default: See "Rhi::DepthStencilOpDesc"
	};
	struct DepthStencilStateBuilder final
	{
		/**
		*  @brief
		*    Return the default depth stencil state
		*
		*  @return
		*    The default depth stencil state, see "Rhi::DepthStencilState" for the default values
		*/
		[[nodiscard]] static inline const DepthStencilState& getDefaultDepthStencilState()
		{
			// As default values, the one of Direct3D 11 and Direct 10 were chosen in order to make it easier for those RHI implementations
			// (choosing OpenGL default values would bring no benefit due to the design of the OpenGL API)
			// - Direct3D 11 "D3D11_DEPTH_STENCIL_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/ff476110%28v=vs.85%29.aspx
			// - Direct3D 10 "D3D10_DEPTH_STENCIL_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb205036%28v=vs.85%29.aspx
			// - Direct3D 9 "D3DRENDERSTATETYPE enumeration"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb172599%28v=vs.85%29.aspx
			// - OpenGL & OpenGL ES 3: The official specifications (unlike Direct3D, OpenGL versions are more compatible to each other)

			// Return default values
			static constexpr DepthStencilState DEPTH_STENCIL_STATE =
			{																		//	Direct3D 11		Direct3D 10	Direct3D 9				OpenGL
				true,						// depthEnable (int)						"true"			"true"		"true"					TODO(co)
				DepthWriteMask::ALL,		// depthWriteMask (Rhi::DepthWriteMask)		"ALL"			"ALL"		"ALL"					TODO(co)
				ComparisonFunc::GREATER,	// depthFunc (Rhi::ComparisonFunc)			"LESS"			"LESS"		"LESS_EQUAL"			TODO(co)	- Default: "Rhi::ComparisonFunc::GREATER" instead of "Rhi::ComparisonFunc::LESS" due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
				false,						// stencilEnable (int)						"false"			"false"		"false"					TODO(co)
				0xff,						// stencilReadMask (uint8_t)				"0xff"			"0xff"		"0xffffffff"			TODO(co)
				0xff,						// stencilWriteMask (uint8_t)				"0xff"			"0xff"		"0xffffffff"			TODO(co)
				{ // sFrontFace (Rhi::DepthStencilOpDesc)
					StencilOp::KEEP,		// stencilFailOp (Rhi::StencilOp			"KEEP"			"KEEP"		"KEEP" (both sides)		TODO(co)
					StencilOp::KEEP,		// stencilDepthFailOp (Rhi::StencilOp)		"KEEP"			"KEEP"		"KEEP" (both sides)		TODO(co)
					StencilOp::KEEP,		// stencilPassOp (Rhi::StencilOp)			"KEEP"			"KEEP"		"KEEP" (both sides)		TODO(co)
					ComparisonFunc::ALWAYS	// stencilFunc (Rhi::ComparisonFunc)		"ALWAYS"		"ALWAYS"	"ALWAYS" (both sides)
				},
				{ // sBackFace (Rhi::DepthStencilOpDesc)
					StencilOp::KEEP,		// stencilFailOp (Rhi::StencilOp)			"KEEP"			"KEEP"		"KEEP" (both sides)		TODO(co)
					StencilOp::KEEP,		// stencilDepthFailOp (Rhi::StencilOp)		"KEEP"			"KEEP"		"KEEP" (both sides)		TODO(co)
					StencilOp::KEEP,		// stencilPassOp (Rhi::StencilOp)			"KEEP"			"KEEP"		"KEEP" (both sides)		TODO(co)
					ComparisonFunc::ALWAYS	// stencilFunc (Rhi::ComparisonFunc)		"ALWAYS"		"ALWAYS"	"ALWAYS" (both sides)	TODO(co)
				}
			};
			return DEPTH_STENCIL_STATE;
		}
	};




	//[-------------------------------------------------------]
	//[ Rhi/State/PipelineStateTypes.h                        ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Input-assembler (IA) stage: Primitive topology types
	*
	*  @note
	*    - These constants directly map to Direct3D 9 & 10 & 11 constants, do not change them
	*/
	enum class PrimitiveTopology
	{
		UNKNOWN        = 0,		///< Unknown primitive type
		POINT_LIST     = 1,		///< Point list, use "PATCH_LIST_1" for tessellation
		LINE_LIST      = 2,		///< Line list, use "PATCH_LIST_2" for tessellation
		LINE_STRIP     = 3,		///< Line strip
		TRIANGLE_LIST  = 4,		///< Triangle list, use "PATCH_LIST_3" for tessellation
		TRIANGLE_STRIP = 5,		///< Triangle strip
		PATCH_LIST_1   = 33,	///< Patch list with 1 vertex per patch (tessellation relevant topology type) - "POINT_LIST" used for tessellation
		PATCH_LIST_2   = 34,	///< Patch list with 2 vertices per patch (tessellation relevant topology type) - "LINE_LIST" used for tessellation
		PATCH_LIST_3   = 35,	///< Patch list with 3 vertices per patch (tessellation relevant topology type) - "TRIANGLE_LIST" used for tessellation
		PATCH_LIST_4   = 36,	///< Patch list with 4 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_5   = 37,	///< Patch list with 5 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_6   = 38,	///< Patch list with 6 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_7   = 39,	///< Patch list with 7 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_8   = 40,	///< Patch list with 8 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_9   = 41,	///< Patch list with 9 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_10  = 42,	///< Patch list with 10 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_11  = 43,	///< Patch list with 11 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_12  = 44,	///< Patch list with 12 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_13  = 45,	///< Patch list with 13 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_14  = 46,	///< Patch list with 14 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_15  = 47,	///< Patch list with 15 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_16  = 48,	///< Patch list with 16 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_17  = 49,	///< Patch list with 17 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_18  = 50,	///< Patch list with 18 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_19  = 51,	///< Patch list with 19 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_20  = 52,	///< Patch list with 20 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_21  = 53,	///< Patch list with 21 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_22  = 54,	///< Patch list with 22 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_23  = 55,	///< Patch list with 23 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_24  = 56,	///< Patch list with 24 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_25  = 57,	///< Patch list with 25 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_26  = 58,	///< Patch list with 26 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_27  = 59,	///< Patch list with 27 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_28  = 60,	///< Patch list with 28 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_29  = 61,	///< Patch list with 29 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_30  = 62,	///< Patch list with 30 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_31  = 63,	///< Patch list with 31 vertices per patch (tessellation relevant topology type)
		PATCH_LIST_32  = 64		///< Patch list with 32 vertices per patch (tessellation relevant topology type)
	};

	/**
	*  @brief
	*    Primitive topology type specifying how the graphics pipeline interprets geometry or hull shader input primitives
	*
	*  @note
	*    - These constants directly map to Direct3D 12 constants, do not change them
	*
	*  @see
	*    - "D3D12_PRIMITIVE_TOPOLOGY_TYPE"-documentation for details
	*/
	enum class PrimitiveTopologyType
	{
		UNDEFINED	= 0,	///< The shader has not been initialized with an input primitive type
		POINT		= 1,	///< Interpret the input primitive as a point
		LINE		= 2,	///< Interpret the input primitive as a line
		TRIANGLE	= 3,	///< Interpret the input primitive as a triangle
		PATCH		= 4		///< Interpret the input primitive as a control point patch
	};

	/**
	*  @brief
	*    Graphics pipeline state
	*/
	struct SerializedGraphicsPipelineState
	{
		PrimitiveTopology	  primitiveTopology;			///< Input-assembler (IA) stage: Primitive topology used for draw calls
		PrimitiveTopologyType primitiveTopologyType;		///< The primitive topology type specifies how the graphics pipeline interprets geometry or hull shader input primitives
		RasterizerState		  rasterizerState;				///< Rasterizer state
		DepthStencilState	  depthStencilState;			///< Depth stencil state
		BlendState			  blendState;					///< Blend state
		uint32_t			  numberOfRenderTargets;		///< Number of render targets
		TextureFormat::Enum	  renderTargetViewFormats[8];	///< Render target view formats
		TextureFormat::Enum	  depthStencilViewFormat;		///< Depth stencil view formats
	};
	struct GraphicsPipelineState : public SerializedGraphicsPipelineState
	{
		IRootSignature*   rootSignature;	///< Root signature (graphics pipeline state instances keep a reference to the root signature), must be valid
		IGraphicsProgram* graphicsProgram;	///< Graphics program used by the graphics pipeline state (graphics pipeline state instances keep a reference to the graphics program), must be valid
		VertexAttributes  vertexAttributes;	///< Vertex attributes, can be empty e.g. in case a task and mesh shader is used
		IRenderPass*	  renderPass;		///< Render pass, the graphics pipeline state keeps a reference, must be valid
	};
	struct GraphicsPipelineStateBuilder final : public GraphicsPipelineState
	{
		inline GraphicsPipelineStateBuilder()
		{
			// "GraphicsPipelineState"-part
			rootSignature						= nullptr;
			graphicsProgram						= nullptr;
			vertexAttributes.numberOfAttributes	= 0;
			vertexAttributes.attributes			= nullptr;
			renderPass							= nullptr;

			// "SerializedGraphicsPipelineState"-part
			primitiveTopology		   = PrimitiveTopology::TRIANGLE_LIST;
			primitiveTopologyType	   = PrimitiveTopologyType::TRIANGLE;
			rasterizerState			   = RasterizerStateBuilder::getDefaultRasterizerState();
			depthStencilState		   = DepthStencilStateBuilder::getDefaultDepthStencilState();
			blendState				   = BlendStateBuilder::getDefaultBlendState();
			numberOfRenderTargets	   = 1;
			renderTargetViewFormats[0] = TextureFormat::R8G8B8A8;
			renderTargetViewFormats[1] = TextureFormat::R8G8B8A8;
			renderTargetViewFormats[2] = TextureFormat::R8G8B8A8;
			renderTargetViewFormats[3] = TextureFormat::R8G8B8A8;
			renderTargetViewFormats[4] = TextureFormat::R8G8B8A8;
			renderTargetViewFormats[5] = TextureFormat::R8G8B8A8;
			renderTargetViewFormats[6] = TextureFormat::R8G8B8A8;
			renderTargetViewFormats[7] = TextureFormat::R8G8B8A8;
			depthStencilViewFormat	   = TextureFormat::D32_FLOAT;
		}

		inline GraphicsPipelineStateBuilder(IRootSignature* _rootSignature, IGraphicsProgram* _graphicsProgram, IRenderPass& _renderPass)
		{
			// "GraphicsPipelineState"-part which has no vertex attributes, e.g. because the graphics program is using a task and mesh shader
			rootSignature	= _rootSignature;
			graphicsProgram = _graphicsProgram;
			renderPass		= &_renderPass;

			// "SerializedGraphicsPipelineState"-part
			primitiveTopology			= PrimitiveTopology::TRIANGLE_LIST;
			primitiveTopologyType		= PrimitiveTopologyType::TRIANGLE;
			rasterizerState				= RasterizerStateBuilder::getDefaultRasterizerState();
			depthStencilState			= DepthStencilStateBuilder::getDefaultDepthStencilState();
			blendState					= BlendStateBuilder::getDefaultBlendState();
			numberOfRenderTargets		= 1;
			renderTargetViewFormats[0]	= TextureFormat::R8G8B8A8;
			renderTargetViewFormats[1]	= TextureFormat::R8G8B8A8;
			renderTargetViewFormats[2]	= TextureFormat::R8G8B8A8;
			renderTargetViewFormats[3]	= TextureFormat::R8G8B8A8;
			renderTargetViewFormats[4]	= TextureFormat::R8G8B8A8;
			renderTargetViewFormats[5]	= TextureFormat::R8G8B8A8;
			renderTargetViewFormats[6]	= TextureFormat::R8G8B8A8;
			renderTargetViewFormats[7]	= TextureFormat::R8G8B8A8;
			depthStencilViewFormat		= TextureFormat::D32_FLOAT;
		}

		inline GraphicsPipelineStateBuilder(IRootSignature* _rootSignature, IGraphicsProgram* _graphicsProgram, const VertexAttributes& _vertexAttributes, IRenderPass& _renderPass)
		{
			// "GraphicsPipelineState"-part
			rootSignature	 = _rootSignature;
			graphicsProgram	 = _graphicsProgram;
			vertexAttributes = _vertexAttributes;
			renderPass		 = &_renderPass;

			// "SerializedGraphicsPipelineState"-part
			primitiveTopology			= PrimitiveTopology::TRIANGLE_LIST;
			primitiveTopologyType		= PrimitiveTopologyType::TRIANGLE;
			rasterizerState				= RasterizerStateBuilder::getDefaultRasterizerState();
			depthStencilState			= DepthStencilStateBuilder::getDefaultDepthStencilState();
			blendState					= BlendStateBuilder::getDefaultBlendState();
			numberOfRenderTargets		= 1;
			renderTargetViewFormats[0]	= TextureFormat::R8G8B8A8;
			renderTargetViewFormats[1]	= TextureFormat::R8G8B8A8;
			renderTargetViewFormats[2]	= TextureFormat::R8G8B8A8;
			renderTargetViewFormats[3]	= TextureFormat::R8G8B8A8;
			renderTargetViewFormats[4]	= TextureFormat::R8G8B8A8;
			renderTargetViewFormats[5]	= TextureFormat::R8G8B8A8;
			renderTargetViewFormats[6]	= TextureFormat::R8G8B8A8;
			renderTargetViewFormats[7]	= TextureFormat::R8G8B8A8;
			depthStencilViewFormat		= TextureFormat::D32_FLOAT;
		}
	};




	//[-------------------------------------------------------]
	//[ Rhi/Shader/ShaderTypes.h                              ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Geometry shader (GS) input primitive topology
	*
	*  @note
	*    - These constants directly map to OpenGL constants, do not change them
	*/
	enum class GsInputPrimitiveTopology
	{
		POINTS			    = 0x0000,	///< List of point primitives
		LINES			    = 0x0001,	///< List of line or line strip primitives
		LINES_ADJACENCY	    = 0x000A,	///< List of line with adjacency or line strip with adjacency primitives
		TRIANGLES		    = 0x0004,	///< List of triangle or triangle strip primitives
		TRIANGLES_ADJACENCY = 0x000C	///< List of triangle with adjacency or triangle strip with adjacency primitives
	};

	/**
	*  @brief
	*    Geometry shader (GS) primitive topology
	*
	*  @note
	*    - These constants directly map to OpenGL constants, do not change them
	*/
	enum class GsOutputPrimitiveTopology
	{
		POINTS			=  0x0000,	///< A list of point primitives
		LINES			=  0x0001,	///< A list of line primitives
		TRIANGLES_STRIP	=  0x0005	///< A triangle strip primitives
	};

	/**
	*  @brief
	*    Shader bytecode (aka shader microcode, binary large object (BLOB))
	*/
	class ShaderBytecode final
	{

	// Public methods
	public:
		inline ShaderBytecode() :
			mNumberOfBytes(0),
			mBytecode(nullptr)
		{}

		inline ~ShaderBytecode()
		{
			delete [] mBytecode;
		}

		[[nodiscard]] inline uint32_t getNumberOfBytes() const
		{
			return mNumberOfBytes;
		}

		[[nodiscard]] inline const uint8_t* getBytecode() const
		{
			return mBytecode;
		}

		inline void setBytecodeCopy(uint32_t numberOfBytes, uint8_t* bytecode)
		{
			delete [] mBytecode;
			mNumberOfBytes = numberOfBytes;
			mBytecode = new uint8_t[mNumberOfBytes];
			memcpy(mBytecode, bytecode, mNumberOfBytes);
		}

	// Private data
	private:
		uint32_t mNumberOfBytes;	///< Number of bytes in the bytecode
		uint8_t* mBytecode;			///< Shader bytecode, can be a null pointer

	};

	/**
	*  @brief
	*    Shader source code
	*/
	struct ShaderSourceCode final
	{
		// Public data
		const char* sourceCode	= nullptr;	///< Shader ASCII source code, if null pointer or empty string a null pointer will be returned
		const char* profile		= nullptr;	///< Shader ASCII profile to use, if null pointer or empty string, a default profile will be used which usually tries to use the best available profile that runs on most hardware (Examples: "glslf", "arbfp1", "ps_3_0")
		const char* arguments	= nullptr;	///< Optional shader compiler ASCII arguments, can be a null pointer or empty string
		const char* entry		= nullptr;	///< ASCII entry point, if null pointer or empty string, "main" is used as default

		// Public methods
		inline ShaderSourceCode(const char* _sourceCode) :
			sourceCode(_sourceCode)
		{}
	};




	//[-------------------------------------------------------]
	//[ Rhi/QueryType.h                                       ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Asynchronous query type
	*/
	enum class QueryType
	{
		OCCLUSION			= 0,	///< Occlusion query, result is a "uint64_t" containing the number of passed samples
		PIPELINE_STATISTICS = 1,	///< Pipeline statistics query, result is a "Rhi::PipelineStatisticsQueryResult"
		TIMESTAMP			= 2		///< Timestamp query, result is a "uint64_t" containing a timestamp in nanosecond
	};

	/**
	*  @brief
	*    Asynchronous query control flags
	*/
	struct QueryControlFlags final
	{
		enum Enum
		{
			PRECISE = 1 << 0	///< Query control precise
		};
	};

	/**
	*  @brief
	*    Asynchronous query result flags
	*/
	struct QueryResultFlags final
	{
		enum Enum
		{
			WAIT = 1 << 0	///< Wait for the result
		};
	};

	/**
	*  @brief
	*    Asynchronous pipeline statistics query result
	*
	*  @note
	*    - This pipeline statistics structure maps directly to Direct3D 11 and Direct3D 12, do not change it
	*
	*  @see
	*    - "D3D11_QUERY_DATA_PIPELINE_STATISTICS"-documentation for details
	*/
	struct PipelineStatisticsQueryResult final
	{
		uint64_t numberOfInputAssemblerVertices;					///< Number of vertices read by input assembler
		uint64_t numberOfInputAssemblerPrimitives;					///< Number of primitives read by the input assembler
		uint64_t numberOfVertexShaderInvocations;					///< Number of times a vertex shader was invoked
		uint64_t numberOfGeometryShaderInvocations;					///< Number of times a geometry shader was invoked
		uint64_t numberOfGeometryShaderOutputPrimitives;			///< Number of primitives output by a geometry shader
		uint64_t numberOfClippingInputPrimitives;					///< Number of primitives that were sent to the rasterizer
		uint64_t numberOfClippingOutputPrimitives;					///< Number of primitives that were rendered
		uint64_t numberOfFragmentShaderInvocations;					///< Number of times a fragment shader (FS, "pixel shader" in Direct3D terminology) was invoked
		uint64_t numberOfTessellationControlShaderInvocations;		///< Number of times a hull shader (TCS, "hull shader" in Direct3D terminology) was invoked
		uint64_t numberOfTessellationEvaluationShaderInvocations;	///< Number of times a domain shader (TES, "domain shader" in Direct3D terminology) was invoked
		uint64_t numberOfComputeShaderInvocations;					///< Number of times a compute shader was invoked
	};




	//[-------------------------------------------------------]
	//[ Rhi/RefCount.h                                        ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Reference counter template
	*
	*  @note
	*    - Initially the reference counter is 0
	*/
	template <class AType>
	class RefCount
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		SE_FORCEINLINE RefCount() :
			mRefCount(0)
		{}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~RefCount()
		{}

		/**
		*  @brief
		*    Get a pointer to the object
		*
		*  @return
		*    Pointer to the reference counter's object, never a null pointer!
		*/
		[[nodiscard]] SE_FORCEINLINE const AType* getPointer() const
		{
			return static_cast<const AType*>(this);
		}

		/**
		*  @brief
		*    Get a pointer to the object
		*
		*  @return
		*    Pointer to the reference counter's object, never a null pointer!
		*/
		[[nodiscard]] SE_FORCEINLINE AType* getPointer()
		{
			return static_cast<AType*>(this);
		}

		/**
		*  @brief
		*    Increases the reference count
		*
		*  @return
		*    Current reference count
		*/
		SE_FORCEINLINE uint32_t addReference()
		{
			// Increment reference count
			++mRefCount;

			// Return current reference count
			return mRefCount;
		}

		/**
		*  @brief
		*    Decreases the reference count
		*
		*  @return
		*    Current reference count
		*
		*  @note
		*    - When the last reference was released, the instance is destroyed automatically
		*/
		SE_FORCEINLINE uint32_t releaseReference()
		{
			// Decrement reference count
			if (mRefCount > 1)
			{
				--mRefCount;

				// Return current reference count
				return mRefCount;
			}

			// Destroy object when no references are left
			else
			{
				selfDestruct();

				// This object is no longer
				return 0;
			}
		}

		/**
		*  @brief
		*    Gets the current reference count
		*
		*  @return
		*    Current reference count
		*/
		[[nodiscard]] SE_FORCEINLINE uint32_t getRefCount() const
		{
			// Return current reference count
			return mRefCount;
		}

	// Protected virtual Rhi::RefCount methods
	protected:
		/**
		*  @brief
		*    Destroy the instance
		*/
		virtual void selfDestruct() = 0;

	// Private data
	private:
		uint32_t mRefCount; ///< Reference count

	};




	//[-------------------------------------------------------]
	//[ Rhi/SmartRefCount.h                                   ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Smart reference counter template
	*/
	template <class AType>
	class SmartRefCount
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		SE_FORCEINLINE SmartRefCount() :
			mPtr(nullptr)
		{}

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] ptr
		*    Direct pointer to initialize with, can be a null pointer
		*/
		SE_FORCEINLINE SmartRefCount(AType* ptr) :
			mPtr(nullptr)
		{
			setPtr(ptr);
		}

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] ptr
		*    Smart pointer to initialize with
		*/
		SE_FORCEINLINE SmartRefCount(const SmartRefCount<AType>& ptr) :
			mPtr(nullptr)
		{
			setPtr(ptr.getPtr());
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline ~SmartRefCount()
		{
			setPtr(nullptr);
		}

		/**
		*  @brief
		*    Assign a pointer
		*
		*  @param[in] ptr
		*    Direct pointer to assign, can be a null pointer
		*
		*  @return
		*    Reference to the smart pointer
		*/
		SE_FORCEINLINE SmartRefCount<AType>& operator =(AType* ptr)
		{
			if (getPointer() != ptr)
			{
				setPtr(ptr);
			}
			return *this;
		}

		/**
		*  @brief
		*    Assign a smart pointer
		*
		*  @param[in] ptr
		*    Smart pointer to assign
		*
		*  @return
		*    Reference to the smart pointer
		*/
		SE_FORCEINLINE SmartRefCount<AType>& operator =(const SmartRefCount<AType>& ptr)
		{
			if (getPointer() != ptr.getPointer())
			{
				setPtr(ptr.getPtr());
			}
			return *this;
		}

		/**
		*  @brief
		*    Get a direct pointer to the object
		*
		*  @return
		*    Pointer to the object, can be a null pointer
		*/
		[[nodiscard]] SE_FORCEINLINE AType* getPointer() const
		{
			return (nullptr != mPtr) ? static_cast<AType*>(mPtr->getPointer()) : nullptr;
		}

		/**
		*  @brief
		*    Get a pointer to access the object
		*
		*  @return
		*    Pointer to the object, can be a null pointer
		*/
		[[nodiscard]] SE_FORCEINLINE AType* operator ->() const
		{
			return getPointer();
		}

		/**
		*  @brief
		*    Cast to a pointer to the object
		*
		*  @return
		*    Pointer to the object, can be a null pointer
		*/
		[[nodiscard]] SE_FORCEINLINE operator AType*() const
		{
			return getPointer();
		}

		/**
		*  @brief
		*    Check if the pointer is not a null pointer
		*
		*  @return
		*    "true" if the pointer is not a null pointer
		*/
		[[nodiscard]] SE_FORCEINLINE bool operator !() const
		{
			return (nullptr == getPointer());
		}

		/**
		*  @brief
		*    Check for equality
		*
		*  @param[in] ptr
		*    Direct pointer to compare with, can be a null pointer
		*
		*  @return
		*    "true" if the two pointers are equal
		*/
		[[nodiscard]] SE_FORCEINLINE bool operator ==(AType* ptr) const
		{
			return (getPointer() == ptr);
		}

		/**
		*  @brief
		*    Check for equality
		*
		*  @param[in] ptr
		*    Smart pointer to compare with
		*
		*  @return
		*    "true" if the two pointers are equal
		*/
		[[nodiscard]] SE_FORCEINLINE bool operator ==(const SmartRefCount<AType>& ptr) const
		{
			return (getPointer() == ptr.getPointer());
		}

		/**
		*  @brief
		*    Check for equality
		*
		*  @param[in] ptr
		*    Direct pointer to compare with, can be a null pointer
		*
		*  @return
		*    "true" if the two pointers are not equal
		*/
		[[nodiscard]] SE_FORCEINLINE bool operator !=(AType* ptr) const
		{
			return (getPointer() != ptr);
		}

		/**
		*  @brief
		*    Check for equality
		*
		*  @param[in] ptr
		*    Smart pointer to compare with
		*
		*  @return
		*    "true" if the two pointers are not equal
		*/
		[[nodiscard]] SE_FORCEINLINE bool operator !=(const SmartRefCount<AType>& ptr) const
		{
			return (getPointer() != ptr.getPointer());
		}

	// Private methods
	private:
		/**
		*  @brief
		*    Assign a pointer to an object that implements RefCount
		*
		*  @param[in] ptr
		*    Pointer to assign, can be a null pointer
		*/
		SE_FORCEINLINE void setPtr(AType* ptr)
		{
			// Release old pointer
			if (nullptr != mPtr)
			{
				mPtr->releaseReference();
			}

			// Assign new pointer
			if (nullptr != ptr)
			{
				ptr->addReference();
			}
			mPtr = ptr;
		}

		/**
		*  @brief
		*    Get pointer to the reference counted object
		*
		*  @return
		*    Pointer to the RefCount object, can be a null pointer
		*/
		[[nodiscard]] SE_FORCEINLINE AType* getPtr() const
		{
			// Return pointer
			return mPtr;
		}

	// Private data
	private:
		AType* mPtr; ///< Pointer to reference counter, can be a null pointer

	};




	//[-------------------------------------------------------]
	//[ Rhi/Capabilities.h                                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Capabilities class
	*
	*  @note
	*    - The data is public by intent in order to make it easier to use this class,
	*      no issues involved because the user only gets a constant instance
	*/
	class Capabilities final
	{

	// Public data
	public:
		char				deviceName[128];								///< UTF-8 device name of the used graphics card (e.g. "AMD Radeon R9 200 Series")
		TextureFormat::Enum preferredSwapChainColorTextureFormat;			///< Preferred swap chain color texture format
		TextureFormat::Enum preferredSwapChainDepthStencilTextureFormat;	///< Preferred swap chain depth stencil texture format
		uint32_t			maximumNumberOfViewports;						///< Maximum number of viewports (always at least 1)
		uint32_t			maximumNumberOfSimultaneousRenderTargets;		///< Maximum number of simultaneous render targets (if <1 render to texture is not supported)
		uint32_t			maximumTextureDimension;						///< Maximum texture dimension (usually 2048, 4096, 8192 or 16384)
		uint32_t			maximumNumberOf1DTextureArraySlices;			///< Maximum number of 1D texture array slices (usually 512 up to 8192, in case there's no support for 1D texture arrays it's 0)
		uint32_t			maximumNumberOf2DTextureArraySlices;			///< Maximum number of 2D texture array slices (usually 512 up to 8192, in case there's no support for 2D texture arrays it's 0)
		uint32_t			maximumNumberOfCubeTextureArraySlices;			///< Maximum number of cube texture array slices (usually 512 up to 8192, in case there's no support for cube texture arrays it's 0)
		uint32_t			maximumTextureBufferSize;						///< Maximum texture buffer (TBO) size in texel (>65536, typically much larger than that of one-dimensional texture, in case there's no support for texture buffer it's 0)
		uint32_t			maximumStructuredBufferSize;					///< Maximum structured buffer size in bytes (>65536, typically much larger than that of one-dimensional texture, in case there's no support for structured buffer it's 0)
		uint32_t			maximumIndirectBufferSize;						///< Maximum indirect buffer size in bytes
		uint32_t			maximumUniformBufferSize;						///< Maximum uniform buffer (UBO) size in bytes (usually at least 4096 *16 bytes, in case there's no support for uniform buffer it's 0)
		uint8_t				maximumNumberOfMultisamples;					///< Maximum number of multisamples (always at least 1, usually 8)
		uint8_t				maximumAnisotropy;								///< Maximum anisotropy (always at least 1, usually 16)
		bool				upperLeftOrigin;								///< Upper left origin (true for Vulkan, Direct3D, OpenGL with "GL_ARB_clip_control"-extension)
		bool				zeroToOneClipZ;									///< Zero-to-one clip Z (true for Vulkan, Direct3D, OpenGL with "GL_ARB_clip_control"-extension)
		bool				individualUniforms;								///< Individual uniforms ("constants" in Direct3D terminology) supported? If not, only uniform buffer objects are supported.
		bool				instancedArrays;								///< Instanced arrays supported? (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
		bool				drawInstanced;									///< Draw instanced supported? (shader model 4 feature, build in shader variable holding the current instance ID)
		bool				baseVertex;										///< Base vertex supported for draw calls?
		bool				nativeMultithreading;							///< Does the RHI support native multithreading? For example Direct3D 11 does meaning we can also create RHI resources asynchronous while for OpenGL we have to create an separate OpenGL context (less efficient, more complex to implement).
		bool				shaderBytecode;									///< Shader bytecode supported?
		// Graphics
		bool				vertexShader;									///< Is there support for vertex shaders (VS)?
		uint32_t			maximumNumberOfPatchVertices;					///< Tessellation-control-shader (TCS) stage and tessellation-evaluation-shader (TES) stage: Maximum number of vertices per patch (usually 0 for no tessellation support or 32 which is the maximum number of supported vertices per patch)
		uint32_t			maximumNumberOfGsOutputVertices;				///< Geometry-shader (GS) stage: Maximum number of vertices a geometry shader (GS) can emit (usually 0 for no geometry shader support or 1024)
		bool				fragmentShader;									///< Is there support for fragment shaders (FS)?
		bool				meshShader;										///< Is there support for task shaders (TS) and mesh shaders (MS)?
		// Compute
		bool				computeShader;									///< Is there support for compute shaders (CS)?

	// Public methods
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline Capabilities() :
			deviceName{},
			preferredSwapChainColorTextureFormat(TextureFormat::Enum::UNKNOWN),
			preferredSwapChainDepthStencilTextureFormat(TextureFormat::Enum::UNKNOWN),
			maximumNumberOfViewports(0),
			maximumNumberOfSimultaneousRenderTargets(0),
			maximumTextureDimension(0),
			maximumNumberOf1DTextureArraySlices(0),
			maximumNumberOf2DTextureArraySlices(0),
			maximumNumberOfCubeTextureArraySlices(0),
			maximumTextureBufferSize(0),
			maximumStructuredBufferSize(0),
			maximumIndirectBufferSize(0),
			maximumUniformBufferSize(0),
			maximumNumberOfMultisamples(1),
			maximumAnisotropy(1),
			upperLeftOrigin(true),
			zeroToOneClipZ(true),
			individualUniforms(false),
			instancedArrays(false),
			drawInstanced(false),
			baseVertex(false),
			nativeMultithreading(false),
			shaderBytecode(false),
			vertexShader(false),
			maximumNumberOfPatchVertices(0),
			maximumNumberOfGsOutputVertices(0),
			fragmentShader(false),
			meshShader(false),
			computeShader(false)
		{}

		/**
		*  @brief
		*    Destructor
		*/
		inline ~Capabilities()
		{}

	// Private methods
	private:
		explicit Capabilities(const Capabilities& source) = delete;
		Capabilities& operator =(const Capabilities& source) = delete;

	};




	//[-------------------------------------------------------]
	//[ Rhi/Statistics.h                                      ]
	//[-------------------------------------------------------]
	#ifdef SE_STATISTICS
		/**
		*  @brief
		*    Statistics class
		*
		*  @note
		*    - The data is public by intent in order to make it easier to use this class,
		*      no issues involved because the user only gets a constant instance
		*/
		class Statistics final
		{

		// Public data
		public:
			// Resources
			std::atomic<uint32_t> currentNumberOfRootSignatures;				///< Current number of root signature instances
			std::atomic<uint32_t> numberOfCreatedRootSignatures;				///< Number of created root signature instances
			std::atomic<uint32_t> currentNumberOfResourceGroups;				///< Current number of resource group instances
			std::atomic<uint32_t> numberOfCreatedResourceGroups;				///< Number of created resource group instances
			std::atomic<uint32_t> currentNumberOfGraphicsPrograms;				///< Current number of graphics program instances
			std::atomic<uint32_t> numberOfCreatedGraphicsPrograms;				///< Number of created graphics program instances
			std::atomic<uint32_t> currentNumberOfVertexArrays;					///< Current number of vertex array object (VAO, input-assembler (IA) stage) instances
			std::atomic<uint32_t> numberOfCreatedVertexArrays;					///< Number of created vertex array object (VAO, input-assembler (IA) stage) instances
			std::atomic<uint32_t> currentNumberOfRenderPasses;					///< Current number of render pass instances
			std::atomic<uint32_t> numberOfCreatedRenderPasses;					///< Number of created render pass instances
			std::atomic<uint32_t> currentNumberOfQueryPools;					///< Current number of asynchronous query pool instances
			std::atomic<uint32_t> numberOfCreatedQueryPools;					///< Number of created asynchronous query pool instances
			// IRenderTarget
			std::atomic<uint32_t> currentNumberOfSwapChains;					///< Current number of swap chain instances
			std::atomic<uint32_t> numberOfCreatedSwapChains;					///< Number of created swap chain instances
			std::atomic<uint32_t> currentNumberOfFramebuffers;					///< Current number of framebuffer object (FBO) instances
			std::atomic<uint32_t> numberOfCreatedFramebuffers;					///< Number of created framebuffer object (FBO) instances
			// IBuffer
			std::atomic<uint32_t> currentNumberOfVertexBuffers;					///< Current number of vertex buffer object (VBO, input-assembler (IA) stage) instances
			std::atomic<uint32_t> numberOfCreatedVertexBuffers;					///< Number of created vertex buffer object (VBO, input-assembler (IA) stage) instances
			std::atomic<uint32_t> currentNumberOfIndexBuffers;					///< Current number of index buffer object (IBO, input-assembler (IA) stage) instances
			std::atomic<uint32_t> numberOfCreatedIndexBuffers;					///< Number of created index buffer object (IBO, input-assembler (IA) stage) instances
			std::atomic<uint32_t> currentNumberOfTextureBuffers;				///< Current number of texture buffer object (TBO) instances
			std::atomic<uint32_t> numberOfCreatedTextureBuffers;				///< Number of created texture buffer object (TBO) instances
			std::atomic<uint32_t> currentNumberOfStructuredBuffers;				///< Current number of structured buffer object (SBO) instances
			std::atomic<uint32_t> numberOfCreatedStructuredBuffers;				///< Number of created structured buffer object (SBO) instances
			std::atomic<uint32_t> currentNumberOfIndirectBuffers;				///< Current number of indirect buffer object instances
			std::atomic<uint32_t> numberOfCreatedIndirectBuffers;				///< Number of created indirect buffer object instances
			std::atomic<uint32_t> currentNumberOfUniformBuffers;				///< Current number of uniform buffer object (UBO, "constant buffer" in Direct3D terminology) instances
			std::atomic<uint32_t> numberOfCreatedUniformBuffers;				///< Number of created uniform buffer object (UBO, "constant buffer" in Direct3D terminology) instances
			// ITexture
			std::atomic<uint32_t> currentNumberOfTexture1Ds;					///< Current number of texture 1D instances
			std::atomic<uint32_t> numberOfCreatedTexture1Ds;					///< Number of created texture 1D instances
			std::atomic<uint32_t> currentNumberOfTexture1DArrays;				///< Current number of texture 1D array instances
			std::atomic<uint32_t> numberOfCreatedTexture1DArrays;				///< Number of created texture 1D array instances
			std::atomic<uint32_t> currentNumberOfTexture2Ds;					///< Current number of texture 2D instances
			std::atomic<uint32_t> numberOfCreatedTexture2Ds;					///< Number of created texture 2D instances
			std::atomic<uint32_t> currentNumberOfTexture2DArrays;				///< Current number of texture 2D array instances
			std::atomic<uint32_t> numberOfCreatedTexture2DArrays;				///< Number of created texture 2D array instances
			std::atomic<uint32_t> currentNumberOfTexture3Ds;					///< Current number of texture 3D instances
			std::atomic<uint32_t> numberOfCreatedTexture3Ds;					///< Number of created texture 3D instances
			std::atomic<uint32_t> currentNumberOfTextureCubes;					///< Current number of texture cube instances
			std::atomic<uint32_t> numberOfCreatedTextureCubes;					///< Number of created texture cube instances
			std::atomic<uint32_t> currentNumberOfTextureCubeArrays;				///< Current number of texture cube array instances
			std::atomic<uint32_t> numberOfCreatedTextureCubeArrays;				///< Number of created texture cube array instances
			// IState
			std::atomic<uint32_t> currentNumberOfGraphicsPipelineStates;		///< Current number of graphics pipeline state (PSO) instances
			std::atomic<uint32_t> numberOfCreatedGraphicsPipelineStates;		///< Number of created graphics pipeline state (PSO) instances
			std::atomic<uint32_t> currentNumberOfComputePipelineStates;			///< Current number of compute pipeline state (PSO) instances
			std::atomic<uint32_t> numberOfCreatedComputePipelineStates;			///< Number of created compute pipeline state (PSO) instances
			std::atomic<uint32_t> currentNumberOfSamplerStates;					///< Current number of sampler state instances
			std::atomic<uint32_t> numberOfCreatedSamplerStates;					///< Number of created sampler state instances
			// IShader
			std::atomic<uint32_t> currentNumberOfVertexShaders;					///< Current number of vertex shader (VS) instances
			std::atomic<uint32_t> numberOfCreatedVertexShaders;					///< Number of created vertex shader (VS) instances
			std::atomic<uint32_t> currentNumberOfTessellationControlShaders;	///< Current number of tessellation control shader (TCS, "hull shader" in Direct3D terminology) instances
			std::atomic<uint32_t> numberOfCreatedTessellationControlShaders;	///< Number of created tessellation control shader (TCS, "hull shader" in Direct3D terminology) instances
			std::atomic<uint32_t> currentNumberOfTessellationEvaluationShaders;	///< Current number of tessellation evaluation shader (TES, "domain shader" in Direct3D terminology) instances
			std::atomic<uint32_t> numberOfCreatedTessellationEvaluationShaders;	///< Number of created tessellation evaluation shader (TES, "domain shader" in Direct3D terminology) instances
			std::atomic<uint32_t> currentNumberOfGeometryShaders;				///< Current number of geometry shader (GS) instances
			std::atomic<uint32_t> numberOfCreatedGeometryShaders;				///< Number of created geometry shader (GS) instances
			std::atomic<uint32_t> currentNumberOfFragmentShaders;				///< Current number of fragment shader (FS, "pixel shader" in Direct3D terminology) instances
			std::atomic<uint32_t> numberOfCreatedFragmentShaders;				///< Number of created fragment shader (FS, "pixel shader" in Direct3D terminology) instances
			std::atomic<uint32_t> currentNumberOfTaskShaders;					///< Current number of task shader (TS) instances
			std::atomic<uint32_t> numberOfCreatedTaskShaders;					///< Number of created task shader (TS) instances
			std::atomic<uint32_t> currentNumberOfMeshShaders;					///< Current number of mesh shader (MS) instances
			std::atomic<uint32_t> numberOfCreatedMeshShaders;					///< Number of created mesh shader (MS) instances
			std::atomic<uint32_t> currentNumberOfComputeShaders;				///< Current number of compute shader (CS) instances
			std::atomic<uint32_t> numberOfCreatedComputeShaders;				///< Number of created compute shader (CS) instances

		// Public methods
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			inline Statistics() :
				currentNumberOfRootSignatures(0),
				numberOfCreatedRootSignatures(0),
				currentNumberOfResourceGroups(0),
				numberOfCreatedResourceGroups(0),
				currentNumberOfGraphicsPrograms(0),
				numberOfCreatedGraphicsPrograms(0),
				currentNumberOfVertexArrays(0),
				numberOfCreatedVertexArrays(0),
				currentNumberOfRenderPasses(0),
				numberOfCreatedRenderPasses(0),
				currentNumberOfQueryPools(0),
				numberOfCreatedQueryPools(0),
				// IRenderTarget
				currentNumberOfSwapChains(0),
				numberOfCreatedSwapChains(0),
				currentNumberOfFramebuffers(0),
				numberOfCreatedFramebuffers(0),
				// IBuffer
				currentNumberOfVertexBuffers(0),
				numberOfCreatedVertexBuffers(0),
				currentNumberOfIndexBuffers(0),
				numberOfCreatedIndexBuffers(0),
				currentNumberOfTextureBuffers(0),
				numberOfCreatedTextureBuffers(0),
				currentNumberOfStructuredBuffers(0),
				numberOfCreatedStructuredBuffers(0),
				currentNumberOfIndirectBuffers(0),
				numberOfCreatedIndirectBuffers(0),
				currentNumberOfUniformBuffers(0),
				numberOfCreatedUniformBuffers(0),
				// ITexture
				currentNumberOfTexture1Ds(0),
				numberOfCreatedTexture1Ds(0),
				currentNumberOfTexture1DArrays(0),
				numberOfCreatedTexture1DArrays(0),
				currentNumberOfTexture2Ds(0),
				numberOfCreatedTexture2Ds(0),
				currentNumberOfTexture2DArrays(0),
				numberOfCreatedTexture2DArrays(0),
				currentNumberOfTexture3Ds(0),
				numberOfCreatedTexture3Ds(0),
				currentNumberOfTextureCubes(0),
				numberOfCreatedTextureCubes(0),
				currentNumberOfTextureCubeArrays(0),
				numberOfCreatedTextureCubeArrays(0),
				// IState
				currentNumberOfGraphicsPipelineStates(0),
				numberOfCreatedGraphicsPipelineStates(0),
				currentNumberOfComputePipelineStates(0),
				numberOfCreatedComputePipelineStates(0),
				currentNumberOfSamplerStates(0),
				numberOfCreatedSamplerStates(0),
				// IShader
				currentNumberOfVertexShaders(0),
				numberOfCreatedVertexShaders(0),
				currentNumberOfTessellationControlShaders(0),
				numberOfCreatedTessellationControlShaders(0),
				currentNumberOfTessellationEvaluationShaders(0),
				numberOfCreatedTessellationEvaluationShaders(0),
				currentNumberOfGeometryShaders(0),
				numberOfCreatedGeometryShaders(0),
				currentNumberOfFragmentShaders(0),
				numberOfCreatedFragmentShaders(0),
				currentNumberOfTaskShaders(0),
				numberOfCreatedTaskShaders(0),
				currentNumberOfMeshShaders(0),
				numberOfCreatedMeshShaders(0),
				currentNumberOfComputeShaders(0),
				numberOfCreatedComputeShaders(0)
			{}

			/**
			*  @brief
			*    Destructor
			*/
			inline ~Statistics()
			{}

			/**
			*  @brief
			*    Return the number of current resource instances
			*
			*  @return
			*    The number of current resource instances
			*
			*  @note
			*    - Primarily for debugging
			*    - The result is calculated by using the current statistics, do only call this method if you have to
			*/
			[[nodiscard]] inline uint32_t getNumberOfCurrentResources() const
			{
				// Calculate the current number of resource instances
				return	currentNumberOfRootSignatures +
						currentNumberOfResourceGroups +
						currentNumberOfGraphicsPrograms +
						currentNumberOfVertexArrays +
						currentNumberOfRenderPasses +
						currentNumberOfQueryPools +
						// IRenderTarget
						currentNumberOfSwapChains +
						currentNumberOfFramebuffers +
						// IBuffer
						currentNumberOfVertexBuffers +
						currentNumberOfIndexBuffers +
						currentNumberOfTextureBuffers +
						currentNumberOfStructuredBuffers +
						currentNumberOfIndirectBuffers +
						currentNumberOfUniformBuffers +
						// ITexture
						currentNumberOfTexture1Ds +
						currentNumberOfTexture1DArrays +
						currentNumberOfTexture2Ds +
						currentNumberOfTexture2DArrays +
						currentNumberOfTexture3Ds +
						currentNumberOfTextureCubes +
						currentNumberOfTextureCubeArrays +
						// IState
						currentNumberOfGraphicsPipelineStates +
						currentNumberOfComputePipelineStates +
						currentNumberOfSamplerStates +
						// IShader
						currentNumberOfVertexShaders +
						currentNumberOfTessellationControlShaders +
						currentNumberOfTessellationEvaluationShaders +
						currentNumberOfGeometryShaders +
						currentNumberOfFragmentShaders +
						currentNumberOfTaskShaders +
						currentNumberOfMeshShaders +
						currentNumberOfComputeShaders;
			}

			/**
			*  @brief
			*    Use debug output to show the current number of resource instances
			*
			*  @param[in] context
			*    The RHI context to use
			*
			*  @note
			*    - Primarily for debugging
			*/
			inline void debugOutputCurrentResouces(const Context& context) const
			{
				// Start
				RHI_LOG(INFORMATION, "** Number of current RHI resource instances **")

				// Misc
				RHI_LOG(INFORMATION, "Root signatures: %u", currentNumberOfRootSignatures.load())
				RHI_LOG(INFORMATION, "Resource groups: %u", currentNumberOfResourceGroups.load())
				RHI_LOG(INFORMATION, "Graphics programs: %u", currentNumberOfGraphicsPrograms.load())
				RHI_LOG(INFORMATION, "Vertex arrays: %u", currentNumberOfVertexArrays.load())
				RHI_LOG(INFORMATION, "Render passes: %u", currentNumberOfRenderPasses.load())
				RHI_LOG(INFORMATION, "Query pools: %u", currentNumberOfQueryPools.load())

				// IRenderTarget
				RHI_LOG( INFORMATION, "Swap chains: %u", currentNumberOfSwapChains.load())
				RHI_LOG( INFORMATION, "Framebuffers: %u", currentNumberOfFramebuffers.load())

				// IBuffer
				RHI_LOG( INFORMATION, "Vertex buffers: %u", currentNumberOfVertexBuffers.load())
				RHI_LOG( INFORMATION, "Index buffers: %u", currentNumberOfIndexBuffers.load())
				RHI_LOG( INFORMATION, "Texture buffers: %u", currentNumberOfTextureBuffers.load())
				RHI_LOG( INFORMATION, "Structured buffers: %u", currentNumberOfStructuredBuffers.load())
				RHI_LOG( INFORMATION, "Indirect buffers: %u", currentNumberOfIndirectBuffers.load())
				RHI_LOG( INFORMATION, "Uniform buffers: %u", currentNumberOfUniformBuffers.load())

				// ITexture
				RHI_LOG(INFORMATION, "1D textures: %u", currentNumberOfTexture1Ds.load())
				RHI_LOG( INFORMATION, "1D texture arrays: %u", currentNumberOfTexture1DArrays.load())
				RHI_LOG( INFORMATION, "2D textures: %u", currentNumberOfTexture2Ds.load())
				RHI_LOG( INFORMATION, "2D texture arrays: %u", currentNumberOfTexture2DArrays.load())
				RHI_LOG( INFORMATION, "3D textures: %u", currentNumberOfTexture3Ds.load())
				RHI_LOG( INFORMATION, "Cube textures: %u", currentNumberOfTextureCubes.load())
				RHI_LOG( INFORMATION, "Cube texture arrays: %u", currentNumberOfTextureCubeArrays.load())

				// IState
				RHI_LOG( INFORMATION, "Graphics pipeline states: %u", currentNumberOfGraphicsPipelineStates.load())
				RHI_LOG( INFORMATION, "Compute pipeline states: %u", currentNumberOfComputePipelineStates.load())
				RHI_LOG( INFORMATION, "Sampler states: %u", currentNumberOfSamplerStates.load())

				// IShader
				RHI_LOG( INFORMATION, "Vertex shaders: %u", currentNumberOfVertexShaders.load())
				RHI_LOG( INFORMATION, "Tessellation control shaders: %u", currentNumberOfTessellationControlShaders.load())
				RHI_LOG( INFORMATION, "Tessellation evaluation shaders: %u", currentNumberOfTessellationEvaluationShaders.load())
				RHI_LOG( INFORMATION, "Geometry shaders: %u", currentNumberOfGeometryShaders.load())
				RHI_LOG( INFORMATION, "Fragment shaders: %u", currentNumberOfFragmentShaders.load())
				RHI_LOG( INFORMATION, "Task shaders: %u", currentNumberOfTaskShaders.load())
				RHI_LOG( INFORMATION, "Mesh shaders: %u", currentNumberOfMeshShaders.load())
				RHI_LOG( INFORMATION, "Compute shaders: %u", currentNumberOfComputeShaders.load())

				// End
				RHI_LOG( INFORMATION, "***************************************************")
			}

		// Private methods
		private:
			explicit Statistics(const Statistics& source) = delete;
			Statistics& operator =(const Statistics& source) = delete;

		};
	#endif




	//[-------------------------------------------------------]
	//[ Rhi/IRhi.h                                            ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    RHI implementation name as ID
	*/
	enum class NameId : uint32_t
	{
		VULKAN		= 1646768219,	///< Vulkan RHI implementation, same value as renderer STRING_ID("Vulkan")
		DIRECT3D12	= 2152506057,	///< Direct3D 12 RHI implementation, same value as renderer STRING_ID("Direct3D12")
		DIRECT3D11	= 2102173200,	///< Direct3D 11 RHI implementation, same value as renderer STRING_ID("Direct3D11")
		OPENGL		= 1149085807,	///< OpenGL RHI implementation, same value as renderer STRING_ID("OpenGL")
		OPENGLES3	= 4137012044,	///< OpenGL ES 3 RHI implementation, same value as renderer STRING_ID("OpenGLES3")
		NULL_DUMMY	= 3816175889	///< Null RHI implementation, same value as renderer STRING_ID("Null")
	};

	#ifdef SE_DIRECT3D11
		static constexpr const char* DEFAULT_RHI_NAME = "Direct3D11";
	#elif defined(SE_OPENGL)
		static constexpr const char* DEFAULT_RHI_NAME = "OpenGL";
	#elif defined(SE_OPENGLES3)
		static constexpr const char* DEFAULT_RHI_NAME = "OpenGLES3";
	#elif defined(SE_VULKAN)
		static constexpr const char* DEFAULT_RHI_NAME = "Vulkan";
	#elif defined(SE_DIRECT3D12)
		static constexpr const char* DEFAULT_RHI_NAME = "Direct3D12";
	#elif defined(SE_RHINULL)
		static constexpr const char* DEFAULT_RHI_NAME = "Null";
	#endif

	/**
	*  @brief
	*    Abstract rendering hardware interface (RHI)
	*/
	class IRhi : public RefCount<IRhi>
	{

	// Friends for none constant statistics access
		friend class IRootSignature;
		friend class IResourceGroup;
		friend class IGraphicsProgram;
		friend class IVertexArray;
		friend class IRenderPass;
		friend class IQueryPool;
		friend class ISwapChain;
		friend class IFramebuffer;
		friend class IVertexBuffer;
		friend class IIndexBuffer;
		friend class ITextureBuffer;
		friend class IStructuredBuffer;
		friend class IIndirectBuffer;
		friend class IUniformBuffer;
		friend class ITexture1D;
		friend class ITexture1DArray;
		friend class ITexture2D;
		friend class ITexture2DArray;
		friend class ITexture3D;
		friend class ITextureCube;
		friend class ITextureCubeArray;
		friend class IGraphicsPipelineState;
		friend class IComputePipelineState;
		friend class ISamplerState;
		friend class IVertexShader;
		friend class ITessellationControlShader;
		friend class ITessellationEvaluationShader;
		friend class IGeometryShader;
		friend class IFragmentShader;
		friend class ITaskShader;
		friend class IMeshShader;
		friend class IComputeShader;

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IRhi() override
		{}

		/**
		*  @brief
		*    Return the RHI implementation name as ID
		*
		*  @return
		*    The RHI implementation name as ID
		*/
		[[nodiscard]] inline NameId getNameId() const
		{
			return mNameId;
		}

		/**
		*  @brief
		*    Return the context of the RHI instance
		*
		*  @return
		*    The context of the RHI instance, do not free the memory the returned reference is pointing to
		*/
		[[nodiscard]] inline const Context& getContext() const
		{
			return mContext;
		}

		/**
		*  @brief
		*    Return the capabilities of the RHI instance
		*
		*  @return
		*    The capabilities of the RHI instance, do not free the memory the returned reference is pointing to
		*/
		[[nodiscard]] inline const Capabilities& getCapabilities() const
		{
			return mCapabilities;
		}

		/**
		*  @brief
		*    Return default shader language instance
		*
		*  @return
		*    The default shader language instance, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline IShaderLanguage& getDefaultShaderLanguage()
		{
			IShaderLanguage* shaderLanguage = getShaderLanguage();
			RHI_ASSERT(nullptr != shaderLanguage, "There's no default shader language")
			return *shaderLanguage;
		}

		#ifdef SE_STATISTICS
			/**
			*  @brief
			*    Return the statistics of the RHI instance
			*
			*  @return
			*    The statistics of the RHI instance, do not free the memory the returned reference is pointing to
			*
			*  @note
			*    - It's possible that the statistics or part of it are disabled, e.g. due to hight performance constrains
			*/
			[[nodiscard]] inline const Statistics& getStatistics() const
			{
				return mStatistics;
			}
		#endif

	// Public virtual Rhi::IRhi methods
	public:
		/**
		*  @brief
		*    Return the name of the RHI instance
		*
		*  @return
		*    The ASCII name of the RHI instance, null pointer on error, do not free the memory the returned pointer is pointing to
		*/
		[[nodiscard]] virtual const char* getName() const = 0;

		/**
		*  @brief
		*    Return whether or not the RHI instance is properly initialized
		*
		*  @return
		*    "true" if the RHI instance is properly initialized, else "false"
		*
		*  @note
		*    - Do never ever use a not properly initialized RHI
		*/
		[[nodiscard]] virtual bool isInitialized() const = 0;

		/**
		*  @brief
		*    Return whether or not debug is enabled
		*
		*  @remarks
		*    By using
		*      "Rhi::IRhi::isDebugEnabled();"
		*    it is possible to check whether or not your application is currently running
		*    within a known debug/profile tool like e.g. Direct3D PIX (also works directly within VisualStudio
		*    2017 out-of-the-box). In case you want at least try to protect your asset, you might want to stop
		*    the execution of your application when a debug/profile tool is used which can e.g. record your data.
		*    Please be aware that this will only make it a little bit harder to debug and e.g. while doing so
		*    reading out your asset data. Public articles like
		*    "PIX: How to circumvent D3DPERF_SetOptions" at
		*      http://www.gamedev.net/blog/1323/entry-2250952-pix-how-to-circumvent-d3dperf-setoptions/
		*    describe how to "hack around" this security measurement, so, don't rely on it. Those debug
		*    methods work fine when using a Direct3D RHI implementation. OpenGL on the other hand
		*    has no Direct3D PIX like functions or extensions, use for instance "gDEBugger" (http://www.gremedy.com/)
		*    instead.
		*    -> When using Direct3D <11.1, those methods map to the Direct3D 9 PIX functions (D3DPERF_* functions)
		*    -> The Direct3D 9 PIX functions are also used for Direct3D 10 and Direct3D 11. Lookout! As soon as using
		*       the debug methods within this interface, the Direct3D 9 dll will be loaded.
		*    -> Starting with Direct3D 11.1, the Direct3D 9 PIX functions no longer work. Instead, the new
		*       "D3D11_CREATE_DEVICE_PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY"-flag (does not work with <Direct3D 11.1)
		*       is used when creating the device instance, then the "ID3DUserDefinedAnnotation"-API is used.
		*    -> Optimization: You might want to use those methods only via macros to make it easier to avoid using them
		*       within e.g. a final release build
		*
		*  @return
		*    "true" if debug is enabled, else "false"
		*/
		[[nodiscard]] virtual bool isDebugEnabled() = 0;

		//[-------------------------------------------------------]
		//[ Shader language                                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the number of supported shader languages
		*
		*  @return
		*    The number of supported shader languages
		*/
		[[nodiscard]] virtual uint32_t getNumberOfShaderLanguages() const = 0;

		/**
		*  @brief
		*    Return the name of a supported shader language at the provided index
		*
		*  @param[in] index
		*    Index of the supported shader language to return the name from ([0, getNumberOfShaderLanguages()-1])
		*
		*  @return
		*    The ASCII name (for example "GLSL" or "HLSL") of the supported shader language at the provided index, can be a null pointer
		*
		*  @note
		*    - Do not free the memory the returned pointer is pointing to
		*    - The default shader language is always at index 0
		*/
		[[nodiscard]] virtual const char* getShaderLanguageName(uint32_t index) const = 0;

		/**
		*  @brief
		*    Return a shader language instance
		*
		*  @param[in] shaderLanguageName
		*    The ASCII name of the shader language (for example "GLSL" or "HLSL"), if null pointer or empty string,
		*    the default RHI shader language is used (see "getShaderLanguageName(0)")
		*
		*  @return
		*    The shader language instance, a null pointer on error, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] virtual IShaderLanguage* getShaderLanguage(const char* shaderLanguageName = nullptr) = 0;

		//[-------------------------------------------------------]
		//[ Resource creation                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Create a render pass instance
		*
		*  @param[in] numberOfColorAttachments
		*    Number of color render target textures, must be <="Rhi::Capabilities::maximumNumberOfSimultaneousRenderTargets"
		*  @param[in] colorAttachmentTextureFormats
		*    The color render target texture formats, can be a null pointer or can contain null pointers, if not a null pointer there must be at
		*    least "numberOfColorAttachments" textures in the provided C-array of pointers
		*  @param[in] depthStencilAttachmentTextureFormat
		*    The optional depth stencil render target texture format, can be a "Rhi::TextureFormat::UNKNOWN" if there should be no depth buffer
		*  @param[in] numberOfMultisamples
		*    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
		*
		*  @return
		*    The created render pass instance, null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual IRenderPass* createRenderPass(uint32_t numberOfColorAttachments, const TextureFormat::Enum* colorAttachmentTextureFormats, TextureFormat::Enum depthStencilAttachmentTextureFormat = TextureFormat::UNKNOWN, uint8_t numberOfMultisamples = 1 RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a asynchronous query pool instance
		*
		*  @param[in] queryType
		*    Query type (e.g. "Rhi::QueryType::OCCLUSION")
		*  @param[in] numberOfQueries
		*    The number of queries the query pool contains (e.g. 1)
		*
		*  @return
		*    The created query pool instance, null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual IQueryPool* createQueryPool(QueryType queryType, uint32_t numberOfQueries = 1 RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a swap chain instance
		*
		*  @param[in] renderPass
		*    Render pass to use, the swap chain keeps a reference to the render pass
		*  @param[in] windowHandle
		*    Information about the window to render into
		*  @param[in] useExternalContext
		*    Indicates if an external RHI context is used; in this case the RHI itself has nothing to do with the creation/managing of an RHI context
		*
		*  @return
		*    The created swap chain instance, null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual ISwapChain* createSwapChain(IRenderPass& renderPass, WindowHandle windowHandle, bool useExternalContext = false RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a framebuffer object (FBO) instance
		*
		*  @param[in] renderPass
		*    Render pass to use, the framebuffer keeps a reference to the render pass
		*  @param[in] colorFramebufferAttachments
		*    The color render target textures, can be a null pointer or can contain null pointers, if not a null pointer there must be at
		*    least "Rhi::IRenderPass::getNumberOfColorAttachments()" textures in the provided C-array of pointers
		*  @param[in] depthStencilFramebufferAttachment
		*    The optional depth stencil render target texture, can be a null pointer
		*
		*  @return
		*    The created FBO instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumNumberOfSimultaneousRenderTargets" is not 0
		*    - The framebuffer keeps a reference to the provided texture instances
		*    - It's invalid to set the same color texture to multiple render targets at one and the same time
		*    - Depending on the used RHI implementation and feature set, there might be the requirement that all provided textures have the same size
		*      (in order to be on the save side, ensure that all provided textures have the same size and same MSAA sample count)
		*/
		[[nodiscard]] virtual IFramebuffer* createFramebuffer(IRenderPass& renderPass, const FramebufferAttachment* colorFramebufferAttachments, const FramebufferAttachment* depthStencilFramebufferAttachment = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a buffer manager instance
		*
		*  @return
		*    The created buffer manager instance, null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual IBufferManager* createBufferManager() = 0;

		/**
		*  @brief
		*    Create a texture manager instance
		*
		*  @return
		*    The created texture manager instance, null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual ITextureManager* createTextureManager() = 0;

		/**
		*  @brief
		*    Create a root signature instance
		*
		*  @param[in] rootSignature
		*    Root signature to use
		*
		*  @return
		*    The root signature instance, null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual IRootSignature* createRootSignature(const RootSignature& rootSignature RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a graphics pipeline state instance
		*
		*  @param[in] graphicsPipelineState
		*    Graphics pipeline state to use
		*
		*  @return
		*    The graphics pipeline state instance, null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual IGraphicsPipelineState* createGraphicsPipelineState(const GraphicsPipelineState& graphicsPipelineState RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a compute pipeline state instance
		*
		*  @param[in] rootSignature
		*    Root signature (compute pipeline state instances keep a reference to the root signature)
		*  @param[in] computeShader
		*    Compute shader used by the compute pipeline state (compute pipeline state instances keep a reference to the shader)
		*
		*  @return
		*    The compute pipeline state instance, null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual IComputePipelineState* createComputePipelineState(IRootSignature& rootSignature, IComputeShader& computeShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a sampler state instance
		*
		*  @param[in] samplerState
		*    Sampler state to use
		*
		*  @return
		*    The sampler state instance, null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual ISamplerState* createSamplerState(const SamplerState& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		//[-------------------------------------------------------]
		//[ Resource handling                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Map a resource
		*
		*  @param[in]  resource
		*    Resource to map, there's no internal resource validation, so, do only use valid resources in here!
		*  @param[in]  subresource
		*    Subresource
		*  @param[in]  mapType
		*    Map type
		*  @param[in]  mapFlags
		*    Map flags, see "Rhi::MapFlag"-flags
		*  @param[out] mappedSubresource
		*    Receives the mapped subresource information, do only use this data in case this method returns successfully
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		[[nodiscard]] virtual bool map(IResource& resource, uint32_t subresource, MapType mapType, uint32_t mapFlags, MappedSubresource& mappedSubresource) = 0;

		/**
		*  @brief
		*    Unmap a resource
		*
		*  @param[in] resource
		*    Resource to unmap, there's no internal resource validation, so, do only use valid resources in here!
		*  @param[in] subresource
		*    Subresource
		*/
		virtual void unmap(IResource& resource, uint32_t subresource) = 0;

		/**
		*  @brief
		*    Get asynchronous query pool results
		*
		*  @param[in] queryPool
		*    Query pool
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
		*  @param[in] queryResultFlags
		*    Query control flags (e.g. "Rhi::QueryResultFlags::WAIT")
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		[[nodiscard]] virtual bool getQueryPoolResults(IQueryPool& queryPool, uint32_t numberOfDataBytes, uint8_t* data, uint32_t firstQueryIndex = 0, uint32_t numberOfQueries = 1, uint32_t strideInBytes = 0, uint32_t queryResultFlags = QueryResultFlags::WAIT) = 0;

		//[-------------------------------------------------------]
		//[ Operations                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Begin scene rendering
		*
		*  @return
		*    "true" if all went fine, else "false" (In this case: Don't dare to render something)
		*
		*  @note
		*    - In order to be RHI implementation independent, call this method when starting to render something
		*/
		[[nodiscard]] virtual bool beginScene() = 0;

		/**
		*  @brief
		*    Submit command buffer to RHI
		*
		*  @param[in] commandBuffer
		*    Command buffer to submit
		*/
		virtual void submitCommandBuffer(const CommandBuffer& commandBuffer) = 0;

		/**
		*  @brief
		*    End scene rendering
		*
		*  @note
		*    - In order to be RHI implementation independent, call this method when you're done with rendering
		*/
		virtual void endScene() = 0;

		//[-------------------------------------------------------]
		//[ Synchronization                                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Force the execution of render commands in finite time (synchronization)
		*/
		virtual void flush() = 0;

		/**
		*  @brief
		*    Force the execution of render commands in finite time and wait until it's done (synchronization)
		*/
		virtual void finish() = 0;

		//[-------------------------------------------------------]
		//[ RHI implementation specific                           ]
		//[-------------------------------------------------------]
		[[nodiscard]] virtual void* getD3D11DevicePointer() const
		{
			return nullptr;
		}

		[[nodiscard]] virtual void* getD3D11ImmediateContextPointer() const
		{
			return nullptr;
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nameId
		*    RHI implementation name as ID
		*  @param[in] context
		*    RHI context, the RHI context instance must stay valid as long as the RHI instance exists
		*/
		inline IRhi(NameId nameId, const Context& context) :
			mNameId(nameId),
			mContext(context)
		{}

		explicit IRhi(const IRhi& source) = delete;
		IRhi& operator =(const IRhi& source) = delete;

		#ifdef SE_STATISTICS
			/**
			*  @brief
			*    Return the statistics of the RHI instance
			*
			*  @return
			*    The statistics of the RHI instance
			*
			*  @note
			*    - Do not free the memory the returned reference is pointing to
			*    - It's possible that the statistics or part of it are disabled, e.g. due to hight performance constrains
			*/
			[[nodiscard]] inline Statistics& getStatistics()
			{
				return mStatistics;
			}
		#endif

	// Protected data
	protected:
		NameId		   mNameId;			///< RHI implementation name as ID
		const Context& mContext;		///< Context
		Capabilities   mCapabilities;	///< Capabilities

	#ifdef SE_STATISTICS
		// Private data
		private:
			Statistics mStatistics;	///< Statistics
	#endif

	};

	typedef SmartRefCount<IRhi> IRhiPtr;




	//[-------------------------------------------------------]
	//[ RHI/Shader/IShaderLanguage.h                          ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract shader language interface
	*/
	class IShaderLanguage : public RefCount<IShaderLanguage>
	{

	// Public definitions
	public:
		/**
		*  @brief
		*    Optimization level
		*/
		enum class OptimizationLevel
		{
			DEBUG = 0,	///< No optimization and debug features enabled, usually only used for debugging
			NONE,		///< No optimization, usually only used for debugging
			LOW,		///< Low optimization
			MEDIUM,		///< Medium optimization
			HIGH,		///< High optimization
			ULTRA		///< Ultra optimization
		};

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IShaderLanguage() override
		{}

		/**
		*  @brief
		*    Return the owner RHI instance
		*
		*  @return
		*    The owner RHI instance, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline IRhi& getRhi() const
		{
			return *mRhi;
		}

		/**
		*  @brief
		*    Return the optimization level
		*
		*  @return
		*    The optimization level
		*/
		[[nodiscard]] inline OptimizationLevel getOptimizationLevel() const
		{
			return mOptimizationLevel;
		}

		/**
		*  @brief
		*    Set the optimization level
		*
		*  @param[in] optimizationLevel
		*    The optimization level
		*/
		inline void setOptimizationLevel(OptimizationLevel optimizationLevel)
		{
			mOptimizationLevel = optimizationLevel;
		}

		/**
		*  @brief
		*    Create a graphics program and assigns a vertex and fragment shader to it
		*
		*  @param[in] rootSignature
		*    Root signature
		*  @param[in] vertexAttributes
		*    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
		*  @param[in] vertexShader
		*    Vertex shader the graphics program is using, can be a null pointer, vertex shader and graphics program language must match
		*  @param[in] fragmentShader
		*    Fragment shader the graphics program is using, can be a null pointer, fragment shader and graphics program language must match
		*
		*  @return
		*    The created graphics program, a null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
		*    - It's valid that a graphics program implementation is adding a reference and releasing it again at once
		*      (this means that in the case of not having any more references, a shader might get destroyed when calling this method)
		*    - Comfort method
		*/
		[[nodiscard]] inline IGraphicsProgram* createGraphicsProgram(const IRootSignature& rootSignature, const VertexAttributes& vertexAttributes, IVertexShader* vertexShader, IFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER)
		{
			#if SE_DEBUG
				return createGraphicsProgram(rootSignature, vertexAttributes, vertexShader, nullptr, nullptr, nullptr, fragmentShader, debugName);
			#else
				return createGraphicsProgram(rootSignature, vertexAttributes, vertexShader, nullptr, nullptr, nullptr, fragmentShader);
			#endif
		}

		/**
		*  @brief
		*    Create a graphics program and assigns a vertex, geometry and fragment shader to it
		*
		*  @param[in] rootSignature
		*    Root signature
		*  @param[in] vertexAttributes
		*    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
		*  @param[in] vertexShader
		*    Vertex shader the graphics program is using, can be a null pointer, vertex shader and graphics program language must match
		*  @param[in] geometryShader
		*    Geometry shader the graphics program is using, can be a null pointer, geometry shader and graphics program language must match
		*  @param[in] fragmentShader
		*    Fragment shader the graphics program is using, can be a null pointer, fragment shader and graphics program language must match
		*
		*  @return
		*    The created graphics program, a null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
		*    - It's valid that a graphics program implementation is adding a reference and releasing it again at once
		*      (this means that in the case of not having any more references, a shader might get destroyed when calling this method)
		*    - Comfort method
		*/
		[[nodiscard]] inline IGraphicsProgram* createGraphicsProgram(const IRootSignature& rootSignature, const VertexAttributes& vertexAttributes, IVertexShader* vertexShader, IGeometryShader* geometryShader, IFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER)
		{
			#if SE_DEBUG
				return createGraphicsProgram(rootSignature, vertexAttributes, vertexShader, nullptr, nullptr, geometryShader, fragmentShader, debugName);
			#else
				return createGraphicsProgram(rootSignature, vertexAttributes, vertexShader, nullptr, nullptr, geometryShader, fragmentShader);
			#endif
		}

		/**
		*  @brief
		*    Create a graphics program and assigns a vertex, tessellation control, tessellation evaluation and fragment shader to it
		*
		*  @param[in] rootSignature
		*    Root signature
		*  @param[in] vertexAttributes
		*    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
		*  @param[in] vertexShader
		*    Vertex shader the graphics program is using, can be a null pointer, vertex shader and graphics program language must match
		*  @param[in] tessellationControlShader
		*    Tessellation control shader the graphics program is using, can be a null pointer, tessellation control shader and graphics program language must match
		*  @param[in] tessellationEvaluationShader
		*    Tessellation evaluation shader the graphics program is using, can be a null pointer, tessellation evaluation shader and graphics program language must match
		*  @param[in] fragmentShader
		*    Fragment shader the graphics program is using, can be a null pointer, fragment shader and graphics program language must match
		*
		*  @return
		*    The created graphics program, a null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
		*    - It's valid that a graphics program implementation is adding a reference and releasing it again at once
		*      (this means that in the case of not having any more references, a shader might get destroyed when calling this method)
		*    - Comfort method
		*/
		[[nodiscard]] inline IGraphicsProgram* createGraphicsProgram(const IRootSignature& rootSignature, const VertexAttributes& vertexAttributes, IVertexShader* vertexShader, ITessellationControlShader* tessellationControlShader, ITessellationEvaluationShader* tessellationEvaluationShader, IFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER)
		{
			#if SE_DEBUG
				return createGraphicsProgram(rootSignature, vertexAttributes, vertexShader, tessellationControlShader, tessellationEvaluationShader, nullptr, fragmentShader, debugName);
			#else
				return createGraphicsProgram(rootSignature, vertexAttributes, vertexShader, tessellationControlShader, tessellationEvaluationShader, nullptr, fragmentShader);
			#endif
		}

	// Public virtual Rhi::IShaderLanguage methods
	public:
		/**
		*  @brief
		*    Return the name of the shader language
		*
		*  @return
		*    The ASCII name of the shader language (for example "GLSL" or "HLSL"), never a null pointer
		*
		*  @note
		*    - Do not free the memory the returned pointer is pointing to
		*/
		[[nodiscard]] virtual const char* getShaderLanguageName() const = 0;

		/**
		*  @brief
		*    Create a vertex shader from shader bytecode
		*
		*  @param[in] vertexAttributes
		*    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*
		*  @return
		*    The created vertex shader, a null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::vertexShader" is "true"
		*    - The data the given pointers are pointing to is internally copied and you have to free your memory if you no longer need it
		*/
		[[nodiscard]] virtual IVertexShader* createVertexShaderFromBytecode(const VertexAttributes& vertexAttributes, const ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a vertex shader from shader source code
		*
		*  @param[in] vertexAttributes
		*    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
		*  @param[in] shaderSourceCode
		*    Shader source code
		*  @param[out] shaderBytecode
		*    If not a null pointer, receives the shader bytecode in case the used RHI implementation supports this feature
		*
		*  @return
		*    The created vertex shader, a null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @remarks
		*    "profile" is not supported by each shader-API and is in general shader-API dependent. GLSL doesn't have such
		*    profiles, just something named "version" - one has to directly write into the shader. But even when this information
		*    is not used for compiling the GLSL shader, we highly recommend to provide GLSL version information in the form of e.g.
		*    "130" for OpenGL 3.0 shaders ("#version 130").
		*    Please note that the profile is just a hint, if necessary, the implementation is free to choose another profile.
		*    In general, be carefully when explicitly setting a profile.
		*
		*   "entry" is not supported by each shader-API. GLSL doesn't have such an user defined entry point and the main
		*   function must always be "main".
		*
		*   Look out! When working with shaders you have to be prepared that a shader may work on one system, but fails to even
		*   compile on another one. Sadly, even if there are e.g. official GLSL specifications, you can't be sure that every
		*   GPU driver is implementing them in detail. Here are some pitfalls which already produced some headaches...
		*
		*   When using GLSL, don't forget to provide the #version directive! Quote from
		*     "The OpenGL� Shading Language - Language Version: 3.30 - Document Revision: 6 - 11-Mar-2010" Page 14
		*       "Version 1.10 of the language does not require shaders to include this directive, and shaders that do not include
		*        a #version directive will be treated as targeting version 1.10."
		*   It looks like that AMD/ATI drivers ("AMD Catalyst� 11.3") are NOT using just version 1.10 if there's no #version directive, but a higher
		*   version... so don't trust your GPU driver when your GLSL code, using modern language features, also works for you without
		*   #version directive, because it may not on other systems! OpenGL version and GLSL version can be a bit confusing, so
		*   here's a version table:
		*     GLSL #version    OpenGL version    Some comments
		*     110              2.0
		*     120              2.1
		*     130              3.0               Precision qualifiers added
		*                                        "attribute" deprecated; linkage between a vertex shader and OpenGL for per-vertex data -> use "in" instead
		*                                        "varying"/"centroid varying" deprecated; linkage between a vertex shader and a fragment shader for interpolated data -> use "in"/"out" instead
		*     140              3.1
		*     150              3.2               Almost feature-identical to Direct3D Shader Model 4.0 (Direct3D version 10), geometry shader added
		*     330              3.3               Equivalent to Direct3D Shader Model 4.0 (Direct3D version 10)
		*     400              4.0               Tessellation control ("Hull"-Shader in Direct3D 11) and evaluation ("Domain"-Shader in Direct3D 11) shaders added
		*     410              4.1
		*     420              4.2               Equivalent to Direct3D Shader Model 5.0 (Direct3D version 11)
		*  #version must occur before any other statement in the graphics program as stated within:
		*    "The OpenGL� Shading Language - Language Version: 3.30 - Document Revision: 6 - 11-Mar-2010" Page 15
		*      "The #version directive must occur in a shader before anything else, except for comments and white space."
		*  ... sadly, this time NVIDIA (driver: "266.58 WHQL") is not implementing the specification in detail and while on AMD/ATI drivers ("AMD Catalyst� 11.3")
		*  you get the error message "error(#105) #version must occur before any other statement in the program" when breaking specification,
		*  NVIDIA just accepts it without any error.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::vertexShader" is "true"
		*    - The data the given pointers are pointing to is internally copied and you have to free your memory if you no longer need it
		*/
		[[nodiscard]] virtual IVertexShader* createVertexShaderFromSourceCode(const VertexAttributes& vertexAttributes, const ShaderSourceCode& shaderSourceCode, ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a tessellation control shader ("hull shader" in Direct3D terminology) from shader bytecode
		*
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*
		*  @return
		*    The created tessellation control shader, a null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumNumberOfPatchVertices" is not 0
		*    - The data the given pointers are pointing to is internally copied and you have to free your memory if you no longer need it
		*/
		[[nodiscard]] virtual ITessellationControlShader* createTessellationControlShaderFromBytecode(const ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a tessellation control shader ("hull shader" in Direct3D terminology) from shader source code
		*
		*  @param[in] shaderSourceCode
		*    Shader source code
		*  @param[out] shaderBytecode
		*    If not a null pointer, receives the shader bytecode in case the used RHI implementation supports this feature
		*
		*  @return
		*    The created tessellation control shader, a null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumNumberOfPatchVertices" is not 0
		*    - The data the given pointers are pointing to is internally copied and you have to free your memory if you no longer need it
		*
		*  @see
		*    - "Rhi::IShaderLanguage::createVertexShader()" for more information
		*/
		[[nodiscard]] virtual ITessellationControlShader* createTessellationControlShaderFromSourceCode(const ShaderSourceCode& shaderSourceCode, ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a tessellation evaluation shader ("domain shader" in Direct3D terminology) from shader bytecode
		*
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*
		*  @return
		*    The created tessellation evaluation shader, a null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumNumberOfPatchVertices" is not 0
		*    - The data the given pointers are pointing to is internally copied and you have to free your memory if you no longer need it
		*/
		[[nodiscard]] virtual ITessellationEvaluationShader* createTessellationEvaluationShaderFromBytecode(const ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a tessellation evaluation shader ("domain shader" in Direct3D terminology) from shader source code
		*
		*  @param[in] shaderSourceCode
		*    Shader source code
		*  @param[out] shaderBytecode
		*    If not a null pointer, receives the shader bytecode in case the used RHI implementation supports this feature
		*
		*  @return
		*    The created tessellation evaluation shader, a null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumNumberOfPatchVertices" is not 0
		*    - The data the given pointers are pointing to is internally copied and you have to free your memory if you no longer need it
		*
		*  @see
		*    - "Rhi::IShaderLanguage::createVertexShader()" for more information
		*/
		[[nodiscard]] virtual ITessellationEvaluationShader* createTessellationEvaluationShaderFromSourceCode(const ShaderSourceCode& shaderSourceCode, ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a geometry shader from shader bytecode
		*
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*  @param[in] gsInputPrimitiveTopology
		*    Geometry shader input primitive topology
		*  @param[in] gsOutputPrimitiveTopology
		*    Geometry shader output primitive topology
		*  @param[in] numberOfOutputVertices
		*    Number of output vertices, maximum is "Rhi::Capabilities::maximumNumberOfGsOutputVertices"
		*
		*  @return
		*    The created geometry shader, a null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumNumberOfGsOutputVertices" is not 0
		*    - The data the given pointers are pointing to is internally copied and you have to free your memory if you no longer need it
		*    - Please note that not each internal implementation may actually need information like "gsInputPrimitiveTopology", but it's
		*      highly recommended to provide this information anyway to be able to switch the internal implementation (e.g. using
		*      OpenGL instead of Direct3D)
		*/
		[[nodiscard]] virtual IGeometryShader* createGeometryShaderFromBytecode(const ShaderBytecode& shaderBytecode, GsInputPrimitiveTopology gsInputPrimitiveTopology, GsOutputPrimitiveTopology gsOutputPrimitiveTopology, uint32_t numberOfOutputVertices RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a geometry shader from shader source code
		*
		*  @param[in] shaderSourceCode
		*    Shader source code
		*  @param[in] gsInputPrimitiveTopology
		*    Geometry shader input primitive topology
		*  @param[in] gsOutputPrimitiveTopology
		*    Geometry shader output primitive topology
		*  @param[in] numberOfOutputVertices
		*    Number of output vertices, maximum is "Rhi::Capabilities::maximumNumberOfGsOutputVertices"
		*  @param[out] shaderBytecode
		*    If not a null pointer, receives the shader bytecode in case the used RHI implementation supports this feature
		*
		*  @return
		*    The created geometry shader, a null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumNumberOfGsOutputVertices" is not 0
		*    - The data the given pointers are pointing to is internally copied and you have to free your memory if you no longer need it
		*    - Please note that not each internal implementation may actually need information like "gsInputPrimitiveTopology", but it's
		*      highly recommended to provide this information anyway to be able to switch the internal implementation (e.g. using
		*      OpenGL instead of Direct3D)
		*
		*  @see
		*    - "Rhi::IShaderLanguage::createVertexShader()" for more information
		*/
		[[nodiscard]] virtual IGeometryShader* createGeometryShaderFromSourceCode(const ShaderSourceCode& shaderSourceCode, GsInputPrimitiveTopology gsInputPrimitiveTopology, GsOutputPrimitiveTopology gsOutputPrimitiveTopology, uint32_t numberOfOutputVertices, ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a fragment shader from shader bytecode
		*
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*
		*  @return
		*    The created fragment shader, a null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::fragmentShader" is "true"
		*    - The data the given pointers are pointing to is internally copied and you have to free your memory if you no longer need it
		*/
		[[nodiscard]] virtual IFragmentShader* createFragmentShaderFromBytecode(const ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a fragment shader from shader source code
		*
		*  @param[in] shaderSourceCode
		*    Shader source code
		*  @param[out] shaderBytecode
		*    If not a null pointer, receives the shader bytecode in case the used RHI implementation supports this feature
		*
		*  @return
		*    The created fragment shader, a null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::fragmentShader" is "true"
		*    - The data the given pointers are pointing to is internally copied and you have to free your memory if you no longer need it
		*
		*  @see
		*    - "Rhi::IShaderLanguage::createVertexShader()" for more information
		*/
		[[nodiscard]] virtual IFragmentShader* createFragmentShaderFromSourceCode(const ShaderSourceCode& shaderSourceCode, ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a task shader from shader bytecode
		*
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*
		*  @return
		*    The created task shader, a null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::meshShader" is "true"
		*    - The data the given pointers are pointing to is internally copied and you have to free your memory if you no longer need it
		*/
		[[nodiscard]] virtual ITaskShader* createTaskShaderFromBytecode(const ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a task shader from shader source code
		*
		*  @param[in] shaderSourceCode
		*    Shader source code
		*  @param[out] shaderBytecode
		*    If not a null pointer, receives the shader bytecode in case the used RHI implementation supports this feature
		*
		*  @return
		*    The created task shader, a null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::meshShader" is "true"
		*    - The data the given pointers are pointing to is internally copied and you have to free your memory if you no longer need it
		*
		*  @see
		*    - "Rhi::IShaderLanguage::createVertexShader()" for more information
		*/
		[[nodiscard]] virtual ITaskShader* createTaskShaderFromSourceCode(const ShaderSourceCode& shaderSourceCode, ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a mesh shader from shader bytecode
		*
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*
		*  @return
		*    The created mesh shader, a null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::meshShader" is "true"
		*    - The data the given pointers are pointing to is internally copied and you have to free your memory if you no longer need it
		*/
		[[nodiscard]] virtual IMeshShader* createMeshShaderFromBytecode(const ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a mesh shader from shader source code
		*
		*  @param[in] shaderSourceCode
		*    Shader source code
		*  @param[out] shaderBytecode
		*    If not a null pointer, receives the shader bytecode in case the used RHI implementation supports this feature
		*
		*  @return
		*    The created mesh shader, a null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::meshShader" is "true"
		*    - The data the given pointers are pointing to is internally copied and you have to free your memory if you no longer need it
		*
		*  @see
		*    - "Rhi::IShaderLanguage::createVertexShader()" for more information
		*/
		[[nodiscard]] virtual IMeshShader* createMeshShaderFromSourceCode(const ShaderSourceCode& shaderSourceCode, ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a compute shader from shader bytecode
		*
		*  @param[in] shaderBytecode
		*    Shader bytecode
		*
		*  @return
		*    The created compute shader, a null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::computeShader" is "true"
		*    - The data the given pointers are pointing to is internally copied and you have to free your memory if you no longer need it
		*/
		[[nodiscard]] virtual IComputeShader* createComputeShaderFromBytecode(const ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a compute shader from shader source code
		*
		*  @param[in] shaderSourceCode
		*    Shader source code
		*  @param[out] shaderBytecode
		*    If not a null pointer, receives the shader bytecode in case the used RHI implementation supports this feature
		*
		*  @return
		*    The created compute shader, a null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::computeShader" is "true"
		*    - The data the given pointers are pointing to is internally copied and you have to free your memory if you no longer need it
		*
		*  @see
		*    - "Rhi::IShaderLanguage::createVertexShader()" for more information
		*/
		[[nodiscard]] virtual IComputeShader* createComputeShaderFromSourceCode(const ShaderSourceCode& shaderSourceCode, ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a graphics program and assigns a vertex, tessellation control, tessellation evaluation, geometry and fragment shader to it
		*
		*  @param[in] rootSignature
		*    Root signature
		*  @param[in] vertexAttributes
		*    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
		*  @param[in] vertexShader
		*    Vertex shader the graphics program is using, can be a null pointer, vertex shader and graphics program language must match
		*  @param[in] tessellationControlShader
		*    Tessellation control shader the graphics program is using, can be a null pointer, tessellation control shader and graphics program language must match
		*  @param[in] tessellationEvaluationShader
		*    Tessellation evaluation shader the graphics program is using, can be a null pointer, tessellation evaluation shader and graphics program language must match
		*  @param[in] geometryShader
		*    Geometry shader the graphics program is using, can be a null pointer, geometry shader and graphics program language must match
		*  @param[in] fragmentShader
		*    Fragment shader the graphics program is using, can be a null pointer, fragment shader and graphics program language must match
		*
		*  @return
		*    The created graphics program, a null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - The graphics program keeps a reference to the provided shaders and releases it when no longer required,
		*      so it's safe to directly hand over a fresh created resource without releasing it manually
		*    - It's valid that a graphics program implementation is adding a reference and releasing it again at once
		*      (this means that in the case of not having any more references, a shader might get destroyed when calling this method)
		*/
		[[nodiscard]] virtual IGraphicsProgram* createGraphicsProgram(const IRootSignature& rootSignature, const VertexAttributes& vertexAttributes, IVertexShader* vertexShader, ITessellationControlShader* tessellationControlShader, ITessellationEvaluationShader* tessellationEvaluationShader, IGeometryShader* geometryShader, IFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a graphics program and assigns a task and mesh shader to it
		*
		*  @param[in] rootSignature
		*    Root signature
		*  @param[in] taskShader
		*    Task shader the graphics program is using, can be a null pointer, task shader and graphics program language must match
		*  @param[in] meshShader
		*    Mesh shader the graphics program is using, mesh shader and graphics program language must match
		*  @param[in] fragmentShader
		*    Fragment shader the graphics program is using, can be a null pointer, fragment shader and graphics program language must match
		*
		*  @return
		*    The created graphics program, a null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
		*    - It's valid that a graphics program implementation is adding a reference and releasing it again at once
		*      (this means that in the case of not having any more references, a shader might get destroyed when calling this method)
		*/
		[[nodiscard]] virtual IGraphicsProgram* createGraphicsProgram(const IRootSignature& rootSignature, ITaskShader* taskShader, IMeshShader& meshShader, IFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit IShaderLanguage(IRhi& rhi) :
			mRhi(&rhi),
			mOptimizationLevel(OptimizationLevel::ULTRA)
		{}

		explicit IShaderLanguage(const IShaderLanguage& source) = delete;
		IShaderLanguage& operator =(const IShaderLanguage& source) = delete;

	// Private data
	private:
		IRhi*			  mRhi;					///< The owner RHI instance, always valid
		OptimizationLevel mOptimizationLevel;	///< Optimization level

	};

	typedef SmartRefCount<IShaderLanguage> IShaderLanguagePtr;




	//[-------------------------------------------------------]
	//[ Rhi/IResource.h                                       ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract resource interface
	*/
	class IResource : public RefCount<IResource>
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IResource() override
		{}

		#if SE_DEBUG
			/**
			*  @brief
			*    Return the resource debug name
			*
			*  @return
			*    The resource debug name, never a null pointer and at least an empty string
			*/
			[[nodiscard]] inline const char* getDebugName() const
			{
				return mDebugName;
			}
		#endif

		/**
		*  @brief
		*    Return the resource type
		*
		*  @return
		*    The resource type
		*/
		[[nodiscard]] inline ResourceType getResourceType() const
		{
			return mResourceType;
		}

		/**
		*  @brief
		*    Return the owner RHI instance
		*
		*  @return
		*    The owner RHI instance, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline IRhi& getRhi() const
		{
			return *mRhi;
		}

	// Public virtual Rhi::IResource methods
	public:
		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the RHI implementation specific resource handle (e.g. native Direct3D texture pointer or OpenGL texture ID)
		*
		*  @return
		*    The RHI implementation specific resource handle, can be a null pointer
		*
		*  @note
		*    - Don't use this RHI implementation specific method if you don't have to
		*/
		[[nodiscard]] inline virtual void* getInternalResourceHandle() const
		{
			return nullptr;
		}

	// Protected methods
	protected:
		#if SE_DEBUG
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] resourceType
			*    The resource type
			*
			*  @note
			*    - Only used for rare border cases, use the constructor with the RHI reference whenever possible
			*/
			inline IResource(ResourceType resourceType, const char debugName[]) :
				mResourceType(resourceType),
				mRhi(nullptr)	// Only used for rare border cases, use the constructor with the RHI reference whenever possible. Normally the RHI pointer should never ever be a null pointer. So if you're in here, you're considered to be evil.
			{
				ASSERT(strlen(debugName) < 256, "Resource debug name is not allowed to exceed 255 characters")
				strncpy(mDebugName, debugName, 256);
				mDebugName[255] = '\0';
			}

			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] resourceType
			*    The resource type
			*  @param[in] rhi
			*    Owner RHI instance
			*/
			inline IResource(ResourceType resourceType, IRhi& rhi, const char debugName[]) :
				mResourceType(resourceType),
				mRhi(&rhi)
			{
				RHI_ASSERT(strlen(debugName) < 256, "Resource debug name is not allowed to exceed 255 characters")
				strncpy(mDebugName, debugName, 256);
				mDebugName[255] = '\0';
			}
		#else
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] resourceType
			*    The resource type
			*
			*  @note
			*    - Only used for rare border cases, use the constructor with the RHI reference whenever possible
			*/
			inline explicit IResource(ResourceType resourceType) :
				mResourceType(resourceType),
				mRhi(nullptr)	// Only used for rare border cases, use the constructor with the RHI reference whenever possible. Normally the RHI pointer should never ever be a null pointer. So if you're in here, you're considered to be evil.
			{}

			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] resourceType
			*    The resource type
			*  @param[in] rhi
			*    Owner RHI instance
			*/
			inline IResource(ResourceType resourceType, IRhi& rhi) :
				mResourceType(resourceType),
				mRhi(&rhi)
			{}
		#endif

		explicit IResource(const IResource& source) = delete;
		IResource& operator =(const IResource& source) = delete;

	// Private data
	private:
		#if SE_DEBUG
			char mDebugName[256];	///< Debug name for easier resource identification when debugging, contains terminating zero, first member variable by intent to see it at once during introspection (debug memory layout change is no problem here)
		#endif
		ResourceType mResourceType;	///< The resource type
		IRhi*		 mRhi;			///< The owner RHI instance, always valid

	};

	typedef SmartRefCount<IResource> IResourcePtr;




	//[-------------------------------------------------------]
	//[ Rhi/RootSignature.h                                   ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract root signature ("pipeline layout" in Vulkan terminology) interface
	*
	*  @note
	*    - Overview of the binding models of explicit APIs: "Choosing a binding model" - https://github.com/gpuweb/gpuweb/issues/19
	*/
	class IRootSignature : public IResource
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IRootSignature() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfRootSignatures;
			#endif
		}

	// Public virtual Rhi::IRootSignature methods
	public:
		/**
		*  @brief
		*    Create a resource group instance
		*
		*  @param[in] rootParameterIndex
		*    The root parameter index number for binding
		*  @param[in] numberOfResources
		*    Number of resources, having no resources is invalid
		*  @param[in] resources
		*    At least "numberOfResources" resource pointers, must be valid, the resource group will keep a reference to the resources
		*  @param[in] samplerStates
		*    If not a null pointer at least "numberOfResources" sampler state pointers, must be valid if there's at least one texture resource, the resource group will keep a reference to the sampler states
		*
		*  @return
		*    The created resource group instance, a null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual IResourceGroup* createResourceGroup(uint32_t rootParameterIndex, uint32_t numberOfResources, IResource** resources, ISamplerState** samplerStates = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit IRootSignature(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(ResourceType::ROOT_SIGNATURE, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedRootSignatures;
				++rhi.getStatistics().currentNumberOfRootSignatures;
			#endif
		}

		explicit IRootSignature(const IRootSignature& source) = delete;
		IRootSignature& operator =(const IRootSignature& source) = delete;

	};

	typedef SmartRefCount<IRootSignature> IRootSignaturePtr;




	//[-------------------------------------------------------]
	//[ Rhi/ResourceGroup.h                                   ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract resource group interface
	*
	*  @note
	*    - A resource group groups resources (Vulkan descriptor set, Direct3D 12 descriptor table)
	*    - A resource group is an instance of a root descriptor table
	*    - Descriptor set comes from Vulkan while root descriptor table comes from Direct3D 12; both APIs have a similar but not identical binding model
	*    - Overview of the binding models of explicit APIs: "Choosing a binding model" - https://github.com/gpuweb/gpuweb/issues/19
	*    - Performance hint: Group resources by binding frequency and set resource groups with a low binding frequency at a low index (e.g. bind a per-pass resource group at index 0)
	*    - Compatibility hint: The number of simultaneous bound resource groups is rather low; try to stick to less or equal to four simultaneous bound resource groups, see http://vulkan.gpuinfo.org/listfeatures.php to check out GPU hardware capabilities
	*    - Compatibility hint: In Direct3D 12, samplers are not allowed in the same descriptor table as CBV/UAV/SRVs, put them into a sampler resource group
	*    - Compatibility hint: In Vulkan, one is usually using a combined image sampler, as a result a sampler resource group doesn't translate to a Vulkan sampler descriptor set.
	*                          Use sampler resource group at the highest binding indices to compensate for this.
	*/
	class IResourceGroup : public IResource
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IResourceGroup() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfResourceGroups;
			#endif
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit IResourceGroup(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(ResourceType::RESOURCE_GROUP, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedResourceGroups;
				++rhi.getStatistics().currentNumberOfResourceGroups;
			#endif
		}

		explicit IResourceGroup(const IResourceGroup& source) = delete;
		IResourceGroup& operator =(const IResourceGroup& source) = delete;

	};

	typedef SmartRefCount<IResourceGroup> IResourceGroupPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Shader/IGraphicsProgram.h                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract graphics program interface
	*/
	class IGraphicsProgram : public IResource
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IGraphicsProgram() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfGraphicsPrograms;
			#endif
		}

	// Public virtual Rhi::IGraphicsProgram methods
	public:
		// TODO(co) Cleanup
		[[nodiscard]] inline virtual handle getUniformHandle([[maybe_unused]] const char* uniformName)
		{
			return SE_NULL_HANDLE;
		}

		inline virtual void setUniform1i([[maybe_unused]] handle uniformHandle, [[maybe_unused]] int value)
		{}

		inline virtual void setUniform1f([[maybe_unused]] handle uniformHandle, [[maybe_unused]] float value)
		{}

		inline virtual void setUniform2fv([[maybe_unused]] handle uniformHandle, [[maybe_unused]] const float* value)
		{}

		inline virtual void setUniform3fv([[maybe_unused]] handle uniformHandle, [[maybe_unused]] const float* value)
		{}

		inline virtual void setUniform4fv([[maybe_unused]] handle uniformHandle, [[maybe_unused]] const float* value)
		{}

		inline virtual void setUniformMatrix3fv([[maybe_unused]] handle uniformHandle, [[maybe_unused]] const float* value)
		{}

		inline virtual void setUniformMatrix4fv([[maybe_unused]] handle uniformHandle, [[maybe_unused]] const float* value)
		{}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit IGraphicsProgram(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(ResourceType::GRAPHICS_PROGRAM, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedGraphicsPrograms;
				++rhi.getStatistics().currentNumberOfGraphicsPrograms;
			#endif
		}

		explicit IGraphicsProgram(const IGraphicsProgram& source) = delete;
		IGraphicsProgram& operator =(const IGraphicsProgram& source) = delete;

	};

	typedef SmartRefCount<IGraphicsProgram> IGraphicsProgramPtr;




	//[-------------------------------------------------------]
	//[ Rhi/RenderTarget/IRenderPass.h                        ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract render pass interface
	*/
	class IRenderPass : public IResource
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IRenderPass() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfRenderPasses;
			#endif
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner rhi instance
		*/
		inline explicit IRenderPass(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(ResourceType::RENDER_PASS, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedRenderPasses;
				++rhi.getStatistics().currentNumberOfRenderPasses;
			#endif
		}

		explicit IRenderPass(const IRenderPass& source) = delete;
		IRenderPass& operator =(const IRenderPass& source) = delete;

	};

	typedef SmartRefCount<IRenderPass> IRenderPassPtr;




	//[-------------------------------------------------------]
	//[ Rhi/RenderTarget/IQueryPool.h                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract asynchronous query pool interface
	*/
	class IQueryPool : public IResource
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IQueryPool() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfQueryPools;
			#endif
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner rhi instance
		*/
		inline explicit IQueryPool(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(ResourceType::QUERY_POOL, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedQueryPools;
				++rhi.getStatistics().currentNumberOfQueryPools;
			#endif
		}

		explicit IQueryPool(const IQueryPool& source) = delete;
		IQueryPool& operator =(const IQueryPool& source) = delete;

	};

	typedef SmartRefCount<IQueryPool> IQueryPoolPtr;




	//[-------------------------------------------------------]
	//[ Rhi/RenderTarget/IRenderTarget.h                      ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract render target interface
	*/
	class IRenderTarget : public IResource
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IRenderTarget() override
		{
			mRenderPass.releaseReference();
		}

		/**
		*  @brief
		*    Return the render pass
		*
		*  @return
		*    Render pass, don't release the reference unless you add an own reference
		*/
		[[nodiscard]] inline IRenderPass& getRenderPass() const
		{
			return mRenderPass;
		}

	// Public virtual Rhi::IRenderTarget methods
	public:
		/**
		*  @brief
		*    Return the width and height of the render target
		*
		*  @param[out] width
		*    Receives the width of the render target, guaranteed to be never ever zero
		*  @param[out] height
		*    Receives the height of the render target, guaranteed to be never ever zero
		*/
		virtual void getWidthAndHeight(uint32_t& width, uint32_t& height) const = 0;

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] resourceType
		*    The resource type
		*  @param[in] renderPass
		*    Render pass to use, the render target keeps a reference to the render pass
		*/
		inline IRenderTarget(ResourceType resourceType, IRenderPass& renderPass RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(resourceType, renderPass.getRhi() RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mRenderPass(renderPass)
		{
			mRenderPass.addReference();
		}

		explicit IRenderTarget(const IRenderTarget& source) = delete;
		IRenderTarget& operator =(const IRenderTarget& source) = delete;

	// Private data
	private:
		IRenderPass& mRenderPass;	///< Render pass to use, the render target keeps a reference to the render pass

	};

	typedef SmartRefCount<IRenderTarget> IRenderTargetPtr;




	//[-------------------------------------------------------]
	//[ Rhi/RenderTarget/ISwapChain.h                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract render window interface which is used to implement some platform specific functionality regarding render window needed by the swap chain implementation
	*
	*  @remarks
	*    This interface can be used to implement the needed platform specific functionality for a platform which isn't known by the RHI implementation
	*    e.g. the user uses a windowing library (e.g. SDL2) which abstracts the window handling on different windowing platforms (e.g. Win32 or Linux/Wayland)
	*    and the application should run on a windowing platform which isn't supported by the swap chain implementation itself.
	*/
	class IRenderWindow
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IRenderWindow()
		{}

	// Public virtual Rhi::IRendererWindow methods
	public:
		/**
		*  @brief
		*    Return the width and height of the render window
		*
		*  @param[out] width
		*    Width of the render window
		*  @param[out] height
		*    Height of the render window
		*/
		virtual void getWidthAndHeight(uint32_t& width, uint32_t& height) const = 0;

		/**
		*  @brief
		*    Present the content of the current back buffer
		*
		*  @note
		*    - Swap of front and back buffer
		*/
		virtual void present() = 0;

	// Protected methods
	protected:
		inline IRenderWindow()
		{}

		explicit IRenderWindow(const IRenderWindow& source) = delete;
		IRenderWindow& operator =(const IRenderWindow& source) = delete;

	};

	/**
	*  @brief
	*    Abstract swap chain interface
	*/
	class ISwapChain : public IRenderTarget
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ISwapChain() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfSwapChains;
			#endif
		}

	// Public virtual Rhi::ISwapChain methods
	public:
		/**
		*  @brief
		*    Return the native window handle
		*
		*  @return
		*    Native window handle the swap chain is using as output window, can be a null handle
		*/
		[[nodiscard]] virtual handle getNativeWindowHandle() const = 0;

		/**
		*  @brief
		*    Set vertical synchronization interval
		*
		*  @param[in] synchronizationInterval
		*    Synchronization interval, >0 if vertical synchronization should be used, else zero
		*/
		virtual void setVerticalSynchronizationInterval(uint32_t synchronizationInterval) = 0;

		/**
		*  @brief
		*    Present the content of the current back buffer
		*
		*  @note
		*    - Swap of front and back buffer
		*/
		virtual void present() = 0;

		/**
		*  @brief
		*    Call this method whenever the size of the native window was changed
		*/
		virtual void resizeBuffers() = 0;

		/**
		*  @brief
		*    Return the current fullscreen state
		*
		*  @return
		*    "true" if fullscreen, else "false"
		*/
		[[nodiscard]] virtual bool getFullscreenState() const = 0;

		/**
		*  @brief
		*    Set the current fullscreen state
		*
		*  @param[in] fullscreen
		*    "true" if fullscreen, else "false"
		*/
		virtual void setFullscreenState(bool fullscreen) = 0;

		/**
		*  @brief
		*    Set an render window instance
		*
		*  @param[in] renderWindow
		*    The render window interface instance, can be a null pointer, if valid the instance must stay valid as long as it's connected to the swap chain instance
		*
		*  @remarks
		*    This method can be used to override the platform specific handling for retrieving window size and doing an buffer swap on the render window (aka present).
		*    An instance can be set when the user don't want that the swap chain itself tempers with the given window handle (the handle might be invalid but non zero)
		*    e.g. the user uses a windowing library (e.g. SDL2) which abstracts the window handling on different windowing platforms (e.g. Win32 or Linux/Wayland) and
		*    the application should run on a windowing platform which isn't supported by the swap chain implementation itself.
		*/
		virtual void setRenderWindow(IRenderWindow* renderWindow) = 0;

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] renderPass
		*    Render pass to use, the swap chain keeps a reference to the render pass
		*/
		inline explicit ISwapChain(IRenderPass& renderPass RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IRenderTarget(ResourceType::SWAP_CHAIN, renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++getRhi().getStatistics().numberOfCreatedSwapChains;
				++getRhi().getStatistics().currentNumberOfSwapChains;
			#endif
		}

		explicit ISwapChain(const ISwapChain& source) = delete;
		ISwapChain& operator =(const ISwapChain& source) = delete;

	};

	typedef SmartRefCount<ISwapChain> ISwapChainPtr;




	//[-------------------------------------------------------]
	//[ Rhi/RenderTarget/IFramebuffer.h                       ]
	//[-------------------------------------------------------]
	struct FramebufferAttachment final
	{
		ITexture* texture;
		uint32_t  mipmapIndex;
		uint32_t  layerIndex;	///< "slice" in Direct3D terminology, depending on the texture type it's a 2D texture array layer, 3D texture slice or cube map face
		inline FramebufferAttachment() :
			texture(nullptr),
			mipmapIndex(0),
			layerIndex(0)
		{}
		inline FramebufferAttachment(ITexture* _texture, uint32_t _mipmapIndex = 0, uint32_t _layerIndex = 0) :
			texture(_texture),
			mipmapIndex(_mipmapIndex),
			layerIndex(_layerIndex)
		{}
	};

	/**
	*  @brief
	*    Abstract framebuffer (FBO) interface
	*/
	class IFramebuffer : public IRenderTarget
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IFramebuffer() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfFramebuffers;
			#endif
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] renderPass
		*    Render pass to use, the framebuffer a reference to the render pass
		*/
		inline explicit IFramebuffer(IRenderPass& renderPass RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IRenderTarget(ResourceType::FRAMEBUFFER, renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++getRhi().getStatistics().numberOfCreatedFramebuffers;
				++getRhi().getStatistics().currentNumberOfFramebuffers;
			#endif
		}

		explicit IFramebuffer(const IFramebuffer& source) = delete;
		IFramebuffer& operator =(const IFramebuffer& source) = delete;

	};

	typedef SmartRefCount<IFramebuffer> IFramebufferPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/IBufferManager.h                           ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract buffer manager interface
	*
	*  @remarks
	*    The buffer manager is responsible for managing fine granular instances of
	*    - Vertex buffer object ("Rhi::IVertexBuffer")
	*    - Index buffer object ("Rhi::IIndexBuffer")
	*    - Vertex array object ("Rhi::IVertexArray")
	*    - Texture buffer object ("Rhi::ITextureBuffer")
	*    - Structured buffer object ("Rhi::IStructuredBuffer")
	*    - Indirect buffer object ("Rhi::IIndirectBuffer")
	*    - Uniform buffer object ("Rhi::IUniformBuffer")
	*
	*    Implementations are free to implement a naive 1:1 mapping of a resource to an RHI implementation resource.
	*    For AZDO ("Almost Zero Driver Overhead") implementations might allocate a few big RHI implementation resources
	*    and manage the granular instances internally. Modern RHI implementations like Vulkan or DirectX 12 are build
	*    around the concept that the user just allocates a few big memory heaps and then managing them by their own.
	*/
	class IBufferManager : public RefCount<IBufferManager>
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IBufferManager() override
		{}

		/**
		*  @brief
		*    Return the owner RHI instance
		*
		*  @return
		*    The owner RHI instance, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline IRhi& getRhi() const
		{
			return mRhi;
		}

	// Public virtual Rhi::IBufferManager methods
	public:
		//[-------------------------------------------------------]
		//[ Resource creation                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Create a vertex buffer object (VBO, "array buffer" in OpenGL terminology) instance
		*
		*  @param[in] numberOfBytes
		*    Number of bytes within the vertex buffer, must be valid
		*  @param[in] data
		*    Vertex buffer data, can be a null pointer (empty buffer), the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] bufferFlags
		*    Buffer flags, see "Rhi::BufferFlag"
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*
		*  @return
		*    The created VBO instance, null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual IVertexBuffer* createVertexBuffer(uint32_t numberOfBytes, const void* data = nullptr, uint32_t bufferFlags = 0, BufferUsage bufferUsage = BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create an index buffer object (IBO, "element array buffer" in OpenGL terminology) instance
		*
		*  @param[in] numberOfBytes
		*    Number of bytes within the index buffer, must be valid
		*  @param[in] data
		*    Index buffer data, can be a null pointer (empty buffer), the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] bufferFlags
		*    Buffer flags, see "Rhi::BufferFlag"
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*  @param[in] indexBufferFormat
		*    Index buffer data format
		*
		*  @return
		*    The created IBO instance, null pointer on error. Release the returned instance if you no longer need it.
		*/
		[[nodiscard]] virtual IIndexBuffer* createIndexBuffer(uint32_t numberOfBytes, const void* data = nullptr, uint32_t bufferFlags = 0, BufferUsage bufferUsage = BufferUsage::STATIC_DRAW, IndexBufferFormat::Enum indexBufferFormat = IndexBufferFormat::UNSIGNED_SHORT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a vertex array instance
		*
		*  @param[in] vertexAttributes
		*    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
		*  @param[in] numberOfVertexBuffers
		*    Number of vertex buffers, having zero vertex buffers is valid
		*  @param[in] vertexBuffers
		*    At least "numberOfVertexBuffers" instances of vertex array vertex buffers, can be a null pointer in case there are zero vertex buffers, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] indexBuffer
		*    Optional index buffer to use, can be a null pointer, the vertex array instance keeps a reference to the index buffer
		*
		*  @return
		*    The created vertex array instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - The created vertex array instance keeps a reference to the vertex buffers used by the vertex array attributes
		*    - It's valid that a vertex array implementation is adding a reference and releasing it again at once
		*      (this means that in the case of not having any more references, a vertex buffer might get destroyed when calling this method)
		*/
		[[nodiscard]] virtual IVertexArray* createVertexArray(const VertexAttributes& vertexAttributes, uint32_t numberOfVertexBuffers, const VertexArrayVertexBuffer* vertexBuffers, IIndexBuffer* indexBuffer = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create an texture buffer object (TBO) instance
		*
		*  @param[in] numberOfBytes
		*    Number of bytes within the texture buffer, must be valid
		*  @param[in] data
		*    Texture buffer data, can be a null pointer (empty buffer), the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] bufferFlags
		*    Buffer flags, see "Rhi::BufferFlag"
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*  @param[in] textureFormat
		*    Texture buffer data format
		*
		*  @return
		*    The created TBO instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumTextureBufferSize" is not 0
		*/
		[[nodiscard]] virtual ITextureBuffer* createTextureBuffer(uint32_t numberOfBytes, const void* data = nullptr, uint32_t bufferFlags = BufferFlag::SHADER_RESOURCE, BufferUsage bufferUsage = BufferUsage::STATIC_DRAW, TextureFormat::Enum textureFormat = TextureFormat::R32G32B32A32F RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create an structured buffer object (SBO) instance
		*
		*  @param[in] numberOfBytes
		*    Number of bytes within the structured buffer, must be valid
		*  @param[in] data
		*    Structured buffer data, can be a null pointer (empty buffer), the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] bufferFlags
		*    Buffer flags, see "Rhi::BufferFlag"
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*  @param[in] numberOfStructureBytes
		*    Number of structure bytes
		*
		*  @return
		*    The created structured buffer instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumStructuredBufferSize" is not 0
		*/
		[[nodiscard]] virtual IStructuredBuffer* createStructuredBuffer(uint32_t numberOfBytes, const void* data, uint32_t bufferFlags, BufferUsage bufferUsage, uint32_t numberOfStructureBytes RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create an indirect buffer object instance
		*
		*  @param[in] numberOfBytes
		*    Number of bytes within the indirect buffer, must be valid
		*  @param[in] data
		*    Indirect buffer data, can be a null pointer (empty buffer), the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] indirectBufferFlags
		*    Indirect buffer flags, see "Rhi::IndirectBufferFlag"
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*
		*  @return
		*    The created indirect buffer instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumIndirectBufferSize" is >0
		*/
		[[nodiscard]] virtual IIndirectBuffer* createIndirectBuffer(uint32_t numberOfBytes, const void* data = nullptr, uint32_t indirectBufferFlags = 0, BufferUsage bufferUsage = BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create an uniform buffer object (UBO, "constant buffer" in Direct3D terminology) instance
		*
		*  @param[in] numberOfBytes
		*    Number of bytes within the uniform buffer, must be valid
		*  @param[in] data
		*    Uniform buffer data, can be a null pointer (empty buffer), the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] bufferUsage
		*    Indication of the buffer usage
		*
		*  @return
		*    The created UBO instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumUniformBufferSize" is >0
		*    - There are no buffer flags by intent since an uniform buffer can't be used for unordered access and as a consequence an uniform buffer must always used as shader resource to not be pointless
		*/
		[[nodiscard]] virtual IUniformBuffer* createUniformBuffer(uint32_t numberOfBytes, const void* data = nullptr, BufferUsage bufferUsage = BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner rhi instance
		*/
		inline explicit IBufferManager(IRhi& rhi) :
			mRhi(rhi)
		{}

		explicit IBufferManager(const IBufferManager& source) = delete;
		IBufferManager& operator =(const IBufferManager& source) = delete;

	// Private data
	private:
		IRhi& mRhi;	///< The owner RHI instance

	};

	typedef SmartRefCount<IBufferManager> IBufferManagerPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/IVertexArray.h                             ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract vertex array object (VAO) interface
	*
	*  @remarks
	*    Encapsulates all the data that is associated with the vertex processor.
	*
	*    Direct3D specifies input stream slot indices within "vertex declaration"/"input layout" without binding them to a particular
	*    vertex buffer object (VBO). Vertex buffers are bound to particular input stream slots. OpenGL "vertex array object" (VAO) has no
	*    concept of input stream slot indices, vertex attributes are directly bound to a particular vertex buffer to feed the data into
	*    the input assembly (IA). To be able to map this interface efficiently to Direct3D as well as OpenGL, this interface has to stick
	*    to the OpenGL "vertex array object"-concept. As a result, we have to directly define vertex buffer objects as data source.
	*/
	class IVertexArray : public IResource
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IVertexArray() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfVertexArrays;
			#endif
		}

		/**
		*  @brief
		*    Return the unique compact vertex array ID
		*
		*  @return
		*    The unique compact vertex array ID
		*/
		inline uint16_t getId() const
		{
			return mId;
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] id
		*    The unique compact vertex array ID
		*/
		inline IVertexArray(IRhi& rhi, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(ResourceType::VERTEX_ARRAY, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mId(id)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedVertexArrays;
				++rhi.getStatistics().currentNumberOfVertexArrays;
			#endif
		}

		explicit IVertexArray(const IVertexArray& source) = delete;
		IVertexArray& operator =(const IVertexArray& source) = delete;

	// Private data
	private:
		uint16_t mId;
	};

	typedef SmartRefCount<IVertexArray> IVertexArrayPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/IBuffer.h                                  ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract buffer interface
	*/
	class IBuffer : public IResource
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IBuffer() override
		{}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] resourceType
		*    The resource type
		*
		*  @note
		*    - Only used for rare border cases, use the constructor with the RHI reference whenever possible
		*/
		inline explicit IBuffer(ResourceType resourceType RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(resourceType RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{}

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] resourceType
		*    The resource type
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline IBuffer(ResourceType resourceType, IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(resourceType, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{}

		explicit IBuffer(const IBuffer& source) = delete;
		IBuffer& operator =(const IBuffer& source) = delete;

	};

	typedef SmartRefCount<IBuffer> IBufferPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/IVertexBuffer.h                            ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract vertex buffer object (VBO, "array buffer" in OpenGL terminology) interface
	*/
	class IVertexBuffer : public IBuffer
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IVertexBuffer() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfVertexBuffers;
			#endif
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit IVertexBuffer(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IBuffer(ResourceType::VERTEX_BUFFER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedVertexBuffers;
				++rhi.getStatistics().currentNumberOfVertexBuffers;
			#endif
		}

		explicit IVertexBuffer(const IVertexBuffer& source) = delete;
		IVertexBuffer& operator =(const IVertexBuffer& source) = delete;

	};

	typedef SmartRefCount<IVertexBuffer> IVertexBufferPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/IIndexBuffer.h                             ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract index buffer object (IBO, "element array buffer" in OpenGL terminology) interface
	*/
	class IIndexBuffer : public IBuffer
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IIndexBuffer() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfIndexBuffers;
			#endif
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit IIndexBuffer(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IBuffer(ResourceType::INDEX_BUFFER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedIndexBuffers;
				++rhi.getStatistics().currentNumberOfIndexBuffers;
			#endif
		}

		explicit IIndexBuffer(const IIndexBuffer& source) = delete;
		IIndexBuffer& operator =(const IIndexBuffer& source) = delete;

	};

	typedef SmartRefCount<IIndexBuffer> IIndexBufferPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/ITextureBuffer.h                           ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract texture buffer object (TBO) interface; array of homogeneous data
	*
	*  @remarks
	*    General usage hint
	*      - Maximum size:                 128MByte (or more)
	*      - Memory access pattern:        Random access
	*      - Memory storage:               Global texture memory which is considered to be slower than local memory
	*    OpenGL - http://www.opengl.org/wiki/Buffer_Texture
	*      - Core in version:              4.2
	*      - Adopted into core in version: 3.0
	*      - ARB extension:                GL_ARB_texture_buffer_object
	*      - EXT extension:                GL_EXT_texture_buffer_object
	*    Direct3D - "Shader Constants"-documentation - http://msdn.microsoft.com/en-us/library/windows/desktop/bb509581%28v=vs.85%29.aspx
	*      - Direct3D version:             10 and 11
	*      - Shader model:                 4
	*/
	class ITextureBuffer : public IBuffer
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITextureBuffer() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfTextureBuffers;
			#endif
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit ITextureBuffer(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IBuffer(ResourceType::TEXTURE_BUFFER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedTextureBuffers;
				++rhi.getStatistics().currentNumberOfTextureBuffers;
			#endif
		}

		explicit ITextureBuffer(const ITextureBuffer& source) = delete;
		ITextureBuffer& operator =(const ITextureBuffer& source) = delete;

	};

	typedef SmartRefCount<ITextureBuffer> ITextureBufferPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/IStructuredBuffer.h                        ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract structured buffer object (SBO) interface; array of structured data
	*
	*  @remarks
	*    General usage hint
	*      - Maximum size:                 128MByte (or more)
	*      - Memory access pattern:        Random access
	*      - Memory storage:               Global texture memory which is considered to be slower than local memory
	*    OpenGL - Shader Storage Buffer Object (SSBO) - https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object
	*      - Core in version:              4.6
	*      - Adopted into core in version: 4.3
	*      - ARB extension:                GL_ARB_shader_storage_buffer_object
	*    Direct3D - "New Resource Types"-documentation - https://docs.microsoft.com/en-gb/windows/desktop/direct3d11/direct3d-11-advanced-stages-cs-resources
	*      - Direct3D version:             11
	*      - Shader model:                 5
	*
	*  @note
	*    - There's no support for Direct3D 11 style "append" or "counter" structured buffer by intent since neither OpenGL nor Vulkan support something similar, use atomic shader intrinsics instead
	*    - Structured performance tip: "Structured buffer, 128-bit stride, coherent access patterns, and under 64 KB of data = Use uniform buffer instead" - "How About Constant Buffers?" by Evan Hart, posted May 04 2015 at 11:34AM - https://developer.nvidia.com/content/how-about-constant-buffers
	*    - Structured performance tip: "Use 128-bit stride" -- "Understanding Structured Buffer Performance" by Evan Hart, posted Apr 17 2015 at 11:33AM - https://developer.nvidia.com/content/understanding-structured-buffer-performance
	*    - Structured performance tip: "Redundancy and Latency in Structured Buffer Use" by Evan Hart, posted Apr 30 2015 at 06:15PM - https://developer.nvidia.com/content/redundancy-and-latency-structured-buffer-use
	*/
	class IStructuredBuffer : public IBuffer
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IStructuredBuffer() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfStructuredBuffers;
			#endif
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit IStructuredBuffer(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IBuffer(ResourceType::STRUCTURED_BUFFER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedStructuredBuffers;
				++rhi.getStatistics().currentNumberOfStructuredBuffers;
			#endif
		}

		explicit IStructuredBuffer(const IStructuredBuffer& source) = delete;
		IStructuredBuffer& operator =(const IStructuredBuffer& source) = delete;

	};

	typedef SmartRefCount<IStructuredBuffer> IStructuredBufferPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/IIndirectBuffer.h                          ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract indirect buffer object interface; array of draw call data
	*
	*  @note
	*    - Contains instances of "Rhi::DrawArguments" and "Rhi::DrawIndexedArguments"
	*    - Indirect buffers where originally introduced to be able to let the GPU have some more control over draw commands,
	*      but with the introduction of multi indirect draw it became also interesting for reducing RHI implementation overhead (AZDO ("Almost Zero Driver Overhead"))
	*/
	class IIndirectBuffer : public IBuffer
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IIndirectBuffer() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfIndirectBuffers;
			#endif
		}

	// Public virtual Rhi::IIndirectBuffer methods
	public:
		/**
		*  @brief
		*    Return indirect buffer emulation data pointer
		*
		*  @return
		*    Indirect buffer emulation data pointer, can be a null pointer, don't destroy the returned instance
		*/
		[[nodiscard]] virtual const uint8_t* getEmulationData() const = 0;

	// Protected methods
	protected:
		#if SE_DEBUG
			/**
			*  @brief
			*    Default constructor
			*
			*  @note
			*    - Only used for rare border cases, use the constructor with the RHI reference whenever possible
			*/
			inline explicit IIndirectBuffer(const char debugName[]) :
				IBuffer(ResourceType::INDIRECT_BUFFER, debugName)
			{}
		#else
			/**
			*  @brief
			*    Default constructor
			*
			*  @note
			*    - Only used for rare border cases, use the constructor with the RHI reference whenever possible
			*/
			inline IIndirectBuffer() :
				IBuffer(ResourceType::INDIRECT_BUFFER)
			{}
		#endif

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit IIndirectBuffer(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IBuffer(ResourceType::INDIRECT_BUFFER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedIndirectBuffers;
				++rhi.getStatistics().currentNumberOfIndirectBuffers;
			#endif
		}

		explicit IIndirectBuffer(const IIndirectBuffer& source) = delete;
		IIndirectBuffer& operator =(const IIndirectBuffer& source) = delete;

	};

	typedef SmartRefCount<IIndirectBuffer> IIndirectBufferPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/IUniformBuffer.h                           ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract uniform buffer object (UBO, "constant buffer" in Direct3D terminology) interface; read only structured buffer
	*
	*  @remarks
	*    General usage hint
	*      - Maximum size:                 64KByte (or more)
	*      - Memory access pattern:        Coherent access
	*      - Memory storage:               Usually local memory which is considered to be faster than global memory
	*    OpenGL - http://www.opengl.org/wiki/Uniform_Buffer_Object
	*      - Core in version:              4.2
	*      - Adopted into core in version: 3.1
	*      - ARB extension:                GL_ARB_Uniform_Buffer_Object
	*    Direct3D - "Shader Constants"-documentation - http://msdn.microsoft.com/en-us/library/windows/desktop/bb509581%28v=vs.85%29.aspx
	*      - Direct3D version:             10 and 11
	*      - Shader model:                 4
	*
	*  @note
	*    - Structured performance tip: "Constant Buffers without Constant Pain" by Holger Gruen, posted Jan 14 2015 at 10:38AM - https://developer.nvidia.com/content/constant-buffers-without-constant-pain-0
	*/
	class IUniformBuffer : public IBuffer
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IUniformBuffer() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfUniformBuffers;
			#endif
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit IUniformBuffer(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IBuffer(ResourceType::UNIFORM_BUFFER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedUniformBuffers;
				++rhi.getStatistics().currentNumberOfUniformBuffers;
			#endif
		}

		explicit IUniformBuffer(const IUniformBuffer& source) = delete;
		IUniformBuffer& operator =(const IUniformBuffer& source) = delete;

	};

	typedef SmartRefCount<IUniformBuffer> IUniformBufferPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Texture/ITextureManager.h                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract texture manager interface
	*
	*  @remarks
	*    The texture manager is responsible for managing fine granular instances of
	*    - 1D texture ("Rhi::ITexture1D")
	*    - 1D texture array ("Rhi::ITexture1DArray")
	*    - 2D texture ("Rhi::ITexture2D")
	*    - 2D texture array ("Rhi::ITexture2DArray")
	*    - 3D texture ("Rhi::ITexture3D")
	*    - Cube texture ("Rhi::ITextureCube")
	*    - Cube texture array ("Rhi::ITextureCubeArray")
	*/
	class ITextureManager : public RefCount<ITextureManager>
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITextureManager() override
		{}

		/**
		*  @brief
		*    Return the owner RHI instance
		*
		*  @return
		*    The owner RHI instance, do not release the returned instance unless you added an own reference to it
		*/
		[[nodiscard]] inline IRhi& getRhi() const
		{
			return mRhi;
		}

	// Public virtual Rhi::ITextureManager methods
	public:
		//[-------------------------------------------------------]
		//[ Resource creation                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Create a 1D texture instance
		*
		*  @param[in] width
		*    Texture width, must be >0 else a null pointer is returned
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*
		*  @return
		*    The created 1D texture instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - The following texture data layout is expected: Mip0, Mip1, Mip2, Mip3 ...
		*/
		[[nodiscard]] virtual ITexture1D* createTexture1D(uint32_t width, TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a 1D array texture instance
		*
		*  @param[in] width
		*    Texture width, must be >0 else a null pointer is returned
		*  @param[in] numberOfSlices
		*    Number of slices, must be >0 else a null pointer is returned
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*
		*  @return
		*    The created 1D array texture instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @remarks
		*    The texture array data consists of a sequence of texture slices. Each the texture slice data of a single texture slice has to
		*    be in CRN-texture layout, which means organized in mip-major order, like this:
		*    - Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
		*    - Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
		*    (DDS-texture layout is using face-major order)
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumNumberOf1DTextureArraySlices" is not 0
		*/
		[[nodiscard]] virtual ITexture1DArray* createTexture1DArray(uint32_t width, uint32_t numberOfSlices, TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a 2D texture instance
		*
		*  @param[in] width
		*    Texture width, must be >0 else a null pointer is returned
		*  @param[in] height
		*    Texture height, must be >0 else a null pointer is returned
		*  @param[in] textureFormat
		*    Texture data format
		*  @param[in] data
		*    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*  @param[in] numberOfMultisamples
		*    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
		*  @param[in] optimizedTextureClearValue
		*    Optional optimized texture clear value
		*
		*  @return
		*    The created 2D texture instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @note
		*    - The following texture data layout is expected: Mip0, Mip1, Mip2, Mip3 ...
		*/
		[[nodiscard]] virtual ITexture2D* createTexture2D(uint32_t width, uint32_t height, TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT, uint8_t numberOfMultisamples = 1, const OptimizedTextureClearValue* optimizedTextureClearValue = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a 2D array texture instance
		*
		*  @param[in] width
		*    Texture width, must be >0 else a null pointer is returned
		*  @param[in] height
		*    Texture height, must be >0 else a null pointer is returned
		*  @param[in] numberOfSlices
		*    Number of slices, must be >0 else a null pointer is returned
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*
		*  @return
		*    The created 2D array texture instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @remarks
		*    The texture array data consists of a sequence of texture slices. Each the texture slice data of a single texture slice has to
		*    be in CRN-texture layout, which means organized in mip-major order, like this:
		*    - Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5 (= positive X, negative X, positive Y, negative Y, positive Z, negative Z)
		*    - Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
		*    (DDS-texture layout is using face-major order)
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumNumberOf2DTextureArraySlices" is not 0
		*/
		[[nodiscard]] virtual ITexture2DArray* createTexture2DArray(uint32_t width, uint32_t height, uint32_t numberOfSlices, TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a 3D texture instance
		*
		*  @param[in] width
		*    Texture width, must be >0 else a null pointer is returned
		*  @param[in] height
		*    Texture height, must be >0 else a null pointer is returned
		*  @param[in] depth
		*    Texture depth, must be >0 else a null pointer is returned
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*
		*  @return
		*    The created 3D texture instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @remarks
		*    The texture data has to be in CRN-texture layout, which means organized in mip-major order, like this:
		*    - Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5 (= positive X, negative X, positive Y, negative Y, positive Z, negative Z)
		*    - Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
		*    (DDS-texture layout is using face-major order)
		*/
		[[nodiscard]] virtual ITexture3D* createTexture3D(uint32_t width, uint32_t height, uint32_t depth, TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a cube texture instance
		*
		*  @param[in] width
		*    Texture width, must be >0 else a null pointer is returned
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*
		*  @return
		*    The created cube texture instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @remarks
		*    The texture data has to be in CRN-texture layout, which means organized in mip-major order, like this:
		*    - Mip0: Face0, Face1, Face2, Face3, Face4, Face5
		*    - Mip1: Face0, Face1, Face2, Face3, Face4, Face5
		*    (DDS-texture layout is using face-major order)
		*/
		[[nodiscard]] virtual ITextureCube* createTextureCube(uint32_t width, TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

		/**
		*  @brief
		*    Create a cube texture array instance
		*
		*  @param[in] width
		*    Texture width, must be >0 else a null pointer is returned
		*  @param[in] numberOfSlices
		*    Number of slices, must be >0 else a null pointer is returned
		*  @param[in] textureFormat
		*    Texture format
		*  @param[in] data
		*    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
		*  @param[in] textureFlags
		*    Texture flags, see "Rhi::TextureFlag::Enum"
		*  @param[in] textureUsage
		*    Indication of the texture usage
		*
		*  @return
		*    The created cube texture array instance, null pointer on error. Release the returned instance if you no longer need it.
		*
		*  @remarks
		*    The texture data has to be in CRN-texture layout, which means organized in mip-major order, like this:
		*    - Mip0: Face0, Face1, Face2, Face3, Face4, Face5
		*    - Mip1: Face0, Face1, Face2, Face3, Face4, Face5
		*    (DDS-texture layout is using face-major order)
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::maximumNumberOfCubeTextureArraySlices" is not 0
		*/
		[[nodiscard]] virtual ITextureCubeArray* createTextureCubeArray(uint32_t width, uint32_t numberOfSlices, TextureFormat::Enum textureFormat, const void* data = nullptr, uint32_t textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit ITextureManager(IRhi& rhi) :
			mRhi(rhi)
		{}

		explicit ITextureManager(const ITextureManager& source) = delete;
		ITextureManager& operator =(const ITextureManager& source) = delete;

	// Private data
	private:
		IRhi& mRhi;	///< The owner RHI instance

	};

	typedef SmartRefCount<ITextureManager> ITextureManagerPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Texture/ITexture.h                                ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract texture interface
	*/
	class ITexture : public IResource
	{

	// Public static methods
	public:
		/**
		*  @brief
		*    Calculate the number of mipmaps for a 1D texture
		*
		*  @param[in] width
		*    Texture width
		*
		*  @return
		*    Number of mipmaps
		*/
		[[nodiscard]] static inline uint32_t getNumberOfMipmaps(uint32_t width)
		{
			// If "std::log2()" wouldn't be supported, we could write the following: "return static_cast<uint32_t>(1 + std::floor(std::log(width) / 0.69314718055994529));"
			// -> log2(x) = log(x) / log(2)
			// -> log(2) = 0.69314718055994529
			return static_cast<uint32_t>(1 + std::floor(std::log2(width)));
		}

		/**
		*  @brief
		*    Calculate the number of mipmaps for a 2D texture
		*
		*  @param[in] width
		*    Texture width
		*  @param[in] height
		*    Texture height
		*
		*  @return
		*    Number of mipmaps
		*/
		[[nodiscard]] static inline uint32_t getNumberOfMipmaps(uint32_t width, uint32_t height)
		{
			return getNumberOfMipmaps((width > height) ? width : height);
		}

		/**
		*  @brief
		*    Calculate the number of mipmaps for a 3D texture
		*
		*  @param[in] width
		*    Texture width
		*  @param[in] height
		*    Texture height
		*  @param[in] depth
		*    Texture depth
		*
		*  @return
		*    Number of mipmaps
		*/
		[[nodiscard]] static inline uint32_t getNumberOfMipmaps(uint32_t width, uint32_t height, uint32_t depth)
		{
			return getNumberOfMipmaps(width, (height > depth) ? height : depth);
		}

		/**
		*  @brief
		*    Calculate the half size
		*
		*  @param[in] size
		*    Size
		*
		*  @return
		*    Half size, 1 as minimum
		*/
		[[nodiscard]] static inline uint32_t getHalfSize(uint32_t size)
		{
			size = (size >> 1);	// /= 2
			return (0u == size) ? 1u : size;
		}

		/**
		*  @brief
		*    Calculate the mipmap size at the given mipmap index
		*
		*  @param[in] mipmapIndex
		*    Mipmap index
		*  @param[in,out] width
		*    Texture width
		*/
		static inline void getMipmapSize(uint32_t mipmapIndex, uint32_t& width)
		{
			if (0u != mipmapIndex)
			{
				width = static_cast<uint32_t>(static_cast<float>(width) / std::exp2f(static_cast<float>(mipmapIndex)));
				if (0u == width)
				{
					width = 1u;
				}
			}
		}

		/**
		*  @brief
		*    Calculate the mipmap size at the given mipmap index
		*
		*  @param[in] mipmapIndex
		*    Mipmap index
		*  @param[in,out] width
		*    Texture width
		*  @param[in,out] height
		*    Texture height
		*/
		static inline void getMipmapSize(uint32_t mipmapIndex, uint32_t& width, uint32_t& height)
		{
			getMipmapSize(mipmapIndex, width);
			getMipmapSize(mipmapIndex, height);
		}

		/**
		*  @brief
		*    Calculate the mipmap size at the given mipmap index
		*
		*  @param[in] mipmapIndex
		*    Mipmap index
		*  @param[in,out] width
		*    Texture width
		*  @param[in,out] height
		*    Texture height
		*  @param[in,out] depth
		*    Texture depth
		*/
		static inline void getMipmapSize(uint32_t mipmapIndex, uint32_t& width, uint32_t& height, uint32_t& depth)
		{
			getMipmapSize(mipmapIndex, width);
			getMipmapSize(mipmapIndex, height);
			getMipmapSize(mipmapIndex, depth);
		}

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITexture() override
		{}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] resourceType
		*    The resource type
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline ITexture(ResourceType resourceType, IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(resourceType, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{}

		explicit ITexture(const ITexture& source) = delete;
		ITexture& operator =(const ITexture& source) = delete;

	};

	typedef SmartRefCount<ITexture> ITexturePtr;




	//[-------------------------------------------------------]
	//[ Rhi/Texture/ITexture1D.h                              ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract 1D texture interface
	*
	*  @remarks
	*    Common use cases for 1D textures:
	*    - Illuminating Engineering Society (IES) light profile as 1D texture
	*    - 1D lens color map for screen space lens flares
	*    - 1D screen space ambient occlusion sample kernel map
	*    - 1D light gradient map for cel�shading (a style of cartoon rendering)
	*/
	class ITexture1D : public ITexture
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITexture1D() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfTexture1Ds;
			#endif
		}

		/**
		*  @brief
		*    Return the width of the texture
		*
		*  @return
		*    The width of the texture
		*/
		[[nodiscard]] inline uint32_t getWidth() const
		{
			return mWidth;
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] width
		*    The width of the texture
		*/
		inline ITexture1D(IRhi& rhi, uint32_t width RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture(ResourceType::TEXTURE_1D, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mWidth(width)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedTexture1Ds;
				++rhi.getStatistics().currentNumberOfTexture1Ds;
			#endif
		}

		explicit ITexture1D(const ITexture1D& source) = delete;
		ITexture1D& operator =(const ITexture1D& source) = delete;

	// Private data
	private:
		uint32_t mWidth;	///< The width of the texture

	};

	typedef SmartRefCount<ITexture1D> ITexture1DPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Texture/ITexture1DArray.h                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract 1D array texture interface
	*
	*  @note
	*    - Common use case for 1D texture array: An 1D array texture is for example useful for storing multiple 1D Illuminating Engineering Society (IES) light profiles packed together in a single texture in order to avoid RHI state changes
	*/
	class ITexture1DArray : public ITexture
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITexture1DArray() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfTexture1DArrays;
			#endif
		}

		/**
		*  @brief
		*    Return the width of the texture
		*
		*  @return
		*    The width of the texture
		*/
		[[nodiscard]] inline uint32_t getWidth() const
		{
			return mWidth;
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

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] width
		*    The width of the texture
		*  @param[in] numberOfSlices
		*    The number of slices
		*/
		inline ITexture1DArray(IRhi& rhi, uint32_t width, uint32_t numberOfSlices RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture(ResourceType::TEXTURE_1D_ARRAY, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mWidth(width),
			mNumberOfSlices(numberOfSlices)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedTexture1DArrays;
				++rhi.getStatistics().currentNumberOfTexture1DArrays;
			#endif
		}

		explicit ITexture1DArray(const ITexture1DArray& source) = delete;
		ITexture1DArray& operator =(const ITexture1DArray& source) = delete;

	// Private data
	private:
		uint32_t mWidth;			///< The width of the texture
		uint32_t mNumberOfSlices;	///< The number of slices

	};

	typedef SmartRefCount<ITexture1DArray> ITexture1DArrayPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Texture/ITexture2D.h                              ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract 2D texture interface
	*
	*  @note
	*    - Common use case for 2D texture: Pretty much every standard 2D texture
	*/
	class ITexture2D : public ITexture
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITexture2D() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfTexture2Ds;
			#endif
		}

		/**
		*  @brief
		*    Return the width of the texture
		*
		*  @return
		*    The width of the texture
		*/
		[[nodiscard]] inline uint32_t getWidth() const
		{
			return mWidth;
		}

		/**
		*  @brief
		*    Return the height of the texture
		*
		*  @return
		*    The height of the texture
		*/
		[[nodiscard]] inline uint32_t getHeight() const
		{
			return mHeight;
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] width
		*    The width of the texture
		*  @param[in] height
		*    The height of the texture
		*/
		inline ITexture2D(IRhi& rhi, uint32_t width, uint32_t height RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture(ResourceType::TEXTURE_2D, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mWidth(width),
			mHeight(height)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedTexture2Ds;
				++rhi.getStatistics().currentNumberOfTexture2Ds;
			#endif
		}

		explicit ITexture2D(const ITexture2D& source) = delete;
		ITexture2D& operator =(const ITexture2D& source) = delete;

	// Private data
	private:
		uint32_t mWidth;	///< The width of the texture
		uint32_t mHeight;	///< The height of the texture

	};

	typedef SmartRefCount<ITexture2D> ITexture2DPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Texture/ITexture2DArray.h                         ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract 2D array texture interface
	*
	*  @remarks
	*    Common use cases for 2D texture arrays:
	*    - The cascade 2D textures of shadow mapping
	*    - The detail layer 2D textures (grass, stone etc.) of a terrain
	*    - Decal 2D textures
	*/
	class ITexture2DArray : public ITexture
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITexture2DArray() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfTexture2DArrays;
			#endif
		}

		/**
		*  @brief
		*    Return the width of the texture
		*
		*  @return
		*    The width of the texture
		*/
		[[nodiscard]] inline uint32_t getWidth() const
		{
			return mWidth;
		}

		/**
		*  @brief
		*    Return the height of the texture
		*
		*  @return
		*    The height of the texture
		*/
		[[nodiscard]] inline uint32_t getHeight() const
		{
			return mHeight;
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

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] width
		*    The width of the texture
		*  @param[in] height
		*    The height of the texture
		*  @param[in] numberOfSlices
		*    The number of slices
		*/
		inline ITexture2DArray(IRhi& rhi, uint32_t width, uint32_t height, uint32_t numberOfSlices RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture(ResourceType::TEXTURE_2D_ARRAY, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mWidth(width),
			mHeight(height),
			mNumberOfSlices(numberOfSlices)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedTexture2DArrays;
				++rhi.getStatistics().currentNumberOfTexture2DArrays;
			#endif
		}

		explicit ITexture2DArray(const ITexture2DArray& source) = delete;
		ITexture2DArray& operator =(const ITexture2DArray& source) = delete;

	// Private data
	private:
		uint32_t mWidth;			///< The width of the texture
		uint32_t mHeight;			///< The height of the texture
		uint32_t mNumberOfSlices;	///< The number of slices

	};

	typedef SmartRefCount<ITexture2DArray> ITexture2DArrayPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Texture/ITexture3D.h                              ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract 3D texture interface
	*
	*  @remarks
	*    Common use cases for 3D textures:
	*    - 3D color correction lookup table (LUT)
	*    - Volume rendering (medical area and nowadays in games as well)
	*    - Light clusters 3D map
	*/
	class ITexture3D : public ITexture
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITexture3D() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfTexture3Ds;
			#endif
		}

		/**
		*  @brief
		*    Return the width of the texture
		*
		*  @return
		*    The width of the texture
		*/
		[[nodiscard]] inline uint32_t getWidth() const
		{
			return mWidth;
		}

		/**
		*  @brief
		*    Return the height of the texture
		*
		*  @return
		*    The height of the texture
		*/
		[[nodiscard]] inline uint32_t getHeight() const
		{
			return mHeight;
		}

		/**
		*  @brief
		*    Return the depth of the texture
		*
		*  @return
		*    The depth of the texture
		*/
		[[nodiscard]] inline uint32_t getDepth() const
		{
			return mDepth;
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] width
		*    The width of the texture
		*  @param[in] height
		*    The height of the texture
		*  @param[in] depth
		*    The depth of the texture
		*/
		inline ITexture3D(IRhi& rhi, uint32_t width, uint32_t height, uint32_t depth RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture(ResourceType::TEXTURE_3D, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mWidth(width),
			mHeight(height),
			mDepth(depth)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedTexture3Ds;
				++rhi.getStatistics().currentNumberOfTexture3Ds;
			#endif
		}

		explicit ITexture3D(const ITexture3D& source) = delete;
		ITexture3D& operator =(const ITexture3D& source) = delete;

	// Private data
	private:
		uint32_t mWidth;	///< The width of the texture
		uint32_t mHeight;	///< The height of the texture
		uint32_t mDepth;	///< The depth of the texture

	};

	typedef SmartRefCount<ITexture3D> ITexture3DPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Texture/ITextureCube.h                            ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract cube texture interface
	*
	*  @note
	*    - Common use case for cube texture: Environment cube maps
	*/
	class ITextureCube : public ITexture
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITextureCube() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfTextureCubes;
			#endif
		}

		/**
		*  @brief
		*    Return the width of the texture
		*
		*  @return
		*    The width of the texture
		*/
		[[nodiscard]] inline uint32_t getWidth() const
		{
			return mWidth;
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] width
		*    The width of the texture
		*/
		inline ITextureCube(IRhi& rhi, uint32_t width RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture(ResourceType::TEXTURE_CUBE, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mWidth(width)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedTextureCubes;
				++rhi.getStatistics().currentNumberOfTextureCubes;
			#endif
		}

		explicit ITextureCube(const ITextureCube& source) = delete;
		ITextureCube& operator =(const ITextureCube& source) = delete;

	// Private data
	private:
		uint32_t mWidth;	///< The width of the texture

	};

	typedef SmartRefCount<ITextureCube> ITextureCubePtr;




	//[-------------------------------------------------------]
	//[ Rhi/Texture/ITextureCubeArray.h                       ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract cube texture array interface
	*
	*  @note
	*    - Common use case for cube texture array: Multiple environment cube maps (created e.g. via environment probes)
	*/
	class ITextureCubeArray : public ITexture
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITextureCubeArray() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfTextureCubeArrays;
			#endif
		}

		/**
		*  @brief
		*    Return the width of the texture
		*
		*  @return
		*    The width of the texture
		*/
		[[nodiscard]] inline uint32_t getWidth() const
		{
			return mWidth;
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

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] width
		*    The width of the texture
		*  @param[in] numberOfSlices
		*    The number of slices
		*/
		inline ITextureCubeArray(IRhi& rhi, uint32_t width, uint32_t numberOfSlices RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			ITexture(ResourceType::TEXTURE_CUBE_ARRAY, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mWidth(width),
			mNumberOfSlices(numberOfSlices)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedTextureCubeArrays;
				++rhi.getStatistics().currentNumberOfTextureCubeArrays;
			#endif
		}

		explicit ITextureCubeArray(const ITextureCubeArray& source) = delete;
		ITextureCubeArray& operator =(const ITextureCubeArray& source) = delete;

	// Private data
	private:
		uint32_t mWidth;			///< The width of the texture
		uint32_t mNumberOfSlices;	///< The number of slices

	};

	typedef SmartRefCount<ITextureCubeArray> ITextureCubeArrayPtr;




	//[-------------------------------------------------------]
	//[ Rhi/State/IState.h                                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract state interface
	*/
	class IState : public IResource
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IState() override
		{}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] resourceType
		*    The resource type
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline IState(ResourceType resourceType, IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(resourceType, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{}

		explicit IState(const IState& source) = delete;
		IState& operator =(const IState& source) = delete;

	};

	typedef SmartRefCount<IState> IStatePtr;




	//[-------------------------------------------------------]
	//[ Rhi/State/IPipelineState.h                            ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract pipeline state interface
	*/
	class IPipelineState : public IState
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IPipelineState() override
		{}

		/**
		*  @brief
		*    Return the unique compact graphics or compute pipeline state ID; one internal ID pool per concrete pipeline state type
		*
		*  @return
		*    The unique compact graphics or compute pipeline state ID
		*/
		inline uint16_t getId() const
		{
			return mId;
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] resourceType
		*    The resource type
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] id
		*    The unique compact graphics or compute pipeline state ID
		*/
		inline IPipelineState(ResourceType resourceType, IRhi& rhi, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IState(resourceType, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mId(id)
		{}

		explicit IPipelineState(const IPipelineState& source) = delete;
		IPipelineState& operator =(const IPipelineState& source) = delete;

	// Private data
	private:
		uint16_t mId;
	};

	typedef SmartRefCount<IPipelineState> IPipelineStatePtr;




	//[-------------------------------------------------------]
	//[ Rhi/State/IGraphicsPipelineState.h                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract graphics pipeline state interface
	*/
	class IGraphicsPipelineState : public IPipelineState
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IGraphicsPipelineState() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfGraphicsPipelineStates;
			#endif
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] id
		*    The unique compact graphics or compute pipeline state ID
		*/
		inline IGraphicsPipelineState(IRhi& rhi, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IPipelineState(ResourceType::GRAPHICS_PIPELINE_STATE, rhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedGraphicsPipelineStates;
				++rhi.getStatistics().currentNumberOfGraphicsPipelineStates;
			#endif
		}

		explicit IGraphicsPipelineState(const IGraphicsPipelineState& source) = delete;
		IGraphicsPipelineState& operator =(const IGraphicsPipelineState& source) = delete;

	};

	typedef SmartRefCount<IGraphicsPipelineState> IGraphicsPipelineStatePtr;




	//[-------------------------------------------------------]
	//[ Rhi/State/IComputePipelineState.h                     ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract compute pipeline state interface
	*/
	class IComputePipelineState : public IPipelineState
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IComputePipelineState() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfComputePipelineStates;
			#endif
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] id
		*    The unique compact graphics or compute pipeline state ID
		*/
		inline IComputePipelineState(IRhi& rhi, uint16_t id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IPipelineState(ResourceType::COMPUTE_PIPELINE_STATE, rhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedComputePipelineStates;
				++rhi.getStatistics().currentNumberOfComputePipelineStates;
			#endif
		}

		explicit IComputePipelineState(const IComputePipelineState& source) = delete;
		IGraphicsPipelineState& operator =(const IComputePipelineState& source) = delete;

	};

	typedef SmartRefCount<IComputePipelineState> IComputePipelineStatePtr;




	//[-------------------------------------------------------]
	//[ Rhi/State/ISamplerState.h                             ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract sampler state interface
	*/
	class ISamplerState : public IState
	{

	// Public static methods
	public:
		/**
		*  @brief
		*    Return the default sampler state
		*
		*  @return
		*    The default sampler state, see "Rhi::SamplerState" for the default values
		*/
		[[nodiscard]] static inline const SamplerState& getDefaultSamplerState()
		{
			// As default values, the one of Direct3D 11 and Direct 10 were chosen in order to make it easier for those RHI implementations
			// (choosing OpenGL default values would bring no benefit due to the design of the OpenGL API)

			// Direct3D 11 "D3D11_SAMPLER_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/ff476207%28v=vs.85%29.aspx

			// The Direct3D 10 documentation is buggy: (online and within the "Microsoft DirectX SDK (June 2010)"-SDK, checked it)
			//   - "D3D10_SAMPLER_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb172415%28v=vs.85%29.aspx
			//     -> Says "Default filter is Min_Mag_Mip_Point"
			//   - "ID3D10Device::VSSetSamplers method"-documentation at MSDN: msdn.microsoft.com/en-us/library/windows/desktop/bb173627(v=vs.85).aspx
			//     -> Says "Default filter is Min_Mag_Mip_Linear"
			//   -> When testing the behaviour, it "looks like" Min_Mag_Mip_Linear is used

			// Direct3D 9 "D3DSAMPLERSTATETYPE enumeration"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb172602%28v=vs.85%29.aspx

			// OpenGL & OpenGL ES 3: The official specifications (unlike Direct3D, OpenGL versions are more compatible to each other)

			// Return default values
			static constexpr SamplerState SAMPLER_STATE =
			{																				//	Direct3D 11					Direct3D 10						Direct3D 9				OpenGL
				FilterMode::MIN_MAG_MIP_LINEAR,	// filter (Rhi::FilterMode)					"MIN_MAG_MIP_LINEAR"			"MIN_MAG_MIP_LINEAR"			"MIN_MAG_MIP_POINT"		"MIN_POINT_MAG_MIP_LINEAR"
				TextureAddressMode::CLAMP,		// addressU (Rhi::TextureAddressMode)		"CLAMP"							"CLAMP"							"WRAP"					"WRAP"
				TextureAddressMode::CLAMP,		// addressV (Rhi::TextureAddressMode)		"CLAMP"							"CLAMP"							"WRAP"					"WRAP"
				TextureAddressMode::CLAMP,		// addressW (Rhi::TextureAddressMode)		"CLAMP"							"CLAMP"							"WRAP"					"WRAP"
				0.0f,							// mipLodBias (float)						"0.0f"							"0.0f"							"0.0f"					"0.0f"
				16,								// maxAnisotropy (uint32_t)					"16"							"16"							"1"						"1"
				ComparisonFunc::NEVER,			// comparisonFunc (Rhi::ComparisonFunc)		"NEVER"							"NEVER"							<unsupported>			"LESS_EQUAL"
				{
					0.0f,						// borderColor[0] (float)					"0.0f"							"0.0f"							"0.0f"					"0.0f"
					0.0f,						// borderColor[1] (float)					"0.0f"							"0.0f"							"0.0f"					"0.0f"
					0.0f,						// borderColor[2] (float)					"0.0f"							"0.0f"							"0.0f"					"0.0f"
					0.0f						// borderColor[3] (float)					"0.0f"							"0.0f"							"0.0f"					"0.0f"
				},
				-3.402823466e+38f,				// minLod (float) - Default: -FLT_MAX		"-3.402823466e+38F (-FLT_MAX)"	"-3.402823466e+38F (-FLT_MAX)"	<unsupported>			"-1000.0f"
				3.402823466e+38f				// maxLod (float) - Default: FLT_MAX		"3.402823466e+38F (FLT_MAX)"	"3.402823466e+38F (FLT_MAX)"	"0.0f"					"1000.0f"
			};
			return SAMPLER_STATE;
		}

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ISamplerState() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfSamplerStates;
			#endif
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit ISamplerState(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IState(ResourceType::SAMPLER_STATE, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedSamplerStates;
				++rhi.getStatistics().currentNumberOfSamplerStates;
			#endif
		}

		explicit ISamplerState(const ISamplerState& source) = delete;
		ISamplerState& operator =(const ISamplerState& source) = delete;

	};

	typedef SmartRefCount<ISamplerState> ISamplerStatePtr;




	//[-------------------------------------------------------]
	//[ Rhi/Shader/IShader.h                                  ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract shader interface
	*/
	class IShader : public IResource
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IShader() override
		{}

	// Public virtual Rhi::IShader methods
	public:
		/**
		*  @brief
		*    Return the name of the shader language the shader is using
		*
		*  @return
		*    The ASCII name of the shader language the shader is using (for example "GLSL" or "HLSL"), always valid
		*
		*  @note
		*    - Do not free the memory the returned pointer is pointing to
		*/
		[[nodiscard]] virtual const char* getShaderLanguageName() const = 0;

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] resourceType
		*    Resource type
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline IShader(ResourceType resourceType, IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(resourceType, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{}

		explicit IShader(const IShader& source) = delete;
		IShader& operator =(const IShader& source) = delete;

	};

	typedef SmartRefCount<IShader> IShaderPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Shader/IVertexShader.h                            ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract vertex shader (VS) interface
	*/
	class IVertexShader : public IShader
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IVertexShader() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfVertexShaders;
			#endif
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit IVertexShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::VERTEX_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedVertexShaders;
				++rhi.getStatistics().currentNumberOfVertexShaders;
			#endif
		}

		explicit IVertexShader(const IVertexShader& source) = delete;
		IVertexShader& operator =(const IVertexShader& source) = delete;

	};

	typedef SmartRefCount<IVertexShader> IVertexShaderPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Shader/ITessellationControlShader.h               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract tessellation control shader (TCS, "hull shader" in Direct3D terminology) interface
	*/
	class ITessellationControlShader : public IShader
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITessellationControlShader() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfTessellationControlShaders;
			#endif
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit ITessellationControlShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::TESSELLATION_CONTROL_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedTessellationControlShaders;
				++rhi.getStatistics().currentNumberOfTessellationControlShaders;
			#endif
		}

		explicit ITessellationControlShader(const ITessellationControlShader& source) = delete;
		ITessellationControlShader& operator =(const ITessellationControlShader& source) = delete;

	};

	typedef SmartRefCount<ITessellationControlShader> ITessellationControlShaderPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Shader/ITessellationEvaluationShader.h            ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract tessellation evaluation shader (TES, "domain shader" in Direct3D terminology) interface
	*/
	class ITessellationEvaluationShader : public IShader
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITessellationEvaluationShader() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfTessellationEvaluationShaders;
			#endif
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit ITessellationEvaluationShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::TESSELLATION_EVALUATION_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedTessellationEvaluationShaders;
				++rhi.getStatistics().currentNumberOfTessellationEvaluationShaders;
			#endif
		}

		explicit ITessellationEvaluationShader(const ITessellationEvaluationShader& source) = delete;
		ITessellationEvaluationShader& operator =(const ITessellationEvaluationShader& source) = delete;

	};

	typedef SmartRefCount<ITessellationEvaluationShader> ITessellationEvaluationShaderPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Shader/IGeometryShader.h                          ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract geometry shader (GS) interface
	*/
	class IGeometryShader : public IShader
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IGeometryShader() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfGeometryShaders;
			#endif
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit IGeometryShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::GEOMETRY_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedGeometryShaders;
				++rhi.getStatistics().currentNumberOfGeometryShaders;
			#endif
		}

		explicit IGeometryShader(const IGeometryShader& source) = delete;
		IGeometryShader& operator =(const IGeometryShader& source) = delete;

	};

	typedef SmartRefCount<IGeometryShader> IGeometryShaderPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Shader/IFragmentShader.h                          ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract fragment shader (FS, "pixel shader" in Direct3D terminology) interface
	*/
	class IFragmentShader : public IShader
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IFragmentShader() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfFragmentShaders;
			#endif
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit IFragmentShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::FRAGMENT_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedFragmentShaders;
				++rhi.getStatistics().currentNumberOfFragmentShaders;
			#endif
		}

		explicit IFragmentShader(const IFragmentShader& source) = delete;
		IFragmentShader& operator =(const IFragmentShader& source) = delete;

	};

	typedef SmartRefCount<IFragmentShader> IFragmentShaderPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Shader/ITaskShader.h                              ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract task shader (TS) interface
	*/
	class ITaskShader : public IShader
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ITaskShader() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfTaskShaders;
			#endif
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit ITaskShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::TASK_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedTaskShaders;
				++rhi.getStatistics().currentNumberOfTaskShaders;
			#endif
		}

		explicit ITaskShader(const ITaskShader& source) = delete;
		ITaskShader& operator =(const ITaskShader& source) = delete;

	};

	typedef SmartRefCount<ITaskShader> ITaskShaderPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Shader/IMeshShader.h                              ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract mesh shader (MS) interface
	*/
	class IMeshShader : public IShader
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IMeshShader() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfMeshShaders;
			#endif
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit IMeshShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::MESH_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedMeshShaders;
				++rhi.getStatistics().currentNumberOfMeshShaders;
			#endif
		}

		explicit IMeshShader(const IMeshShader& source) = delete;
		IMeshShader& operator =(const IMeshShader& source) = delete;

	};

	typedef SmartRefCount<IMeshShader> IMeshShaderPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Shader/IComputeShader.h                           ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract compute shader (CS) interface
	*/
	class IComputeShader : public IShader
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IComputeShader() override
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				--getRhi().getStatistics().currentNumberOfComputeShaders;
			#endif
		}

	// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rhi
		*    Owner RHI instance
		*/
		inline explicit IComputeShader(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IShader(ResourceType::COMPUTE_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
			#ifdef SE_STATISTICS
				// Update the statistics
				++rhi.getStatistics().numberOfCreatedComputeShaders;
				++rhi.getStatistics().currentNumberOfComputeShaders;
			#endif
		}

		explicit IComputeShader(const IComputeShader& source) = delete;
		IComputeShader& operator =(const IComputeShader& source) = delete;

	};

	typedef SmartRefCount<IComputeShader> IComputeShaderPtr;




	//[-------------------------------------------------------]
	//[ Rhi/Buffer/CommandBuffer.h                            ]
	//[-------------------------------------------------------]
	enum class CommandDispatchFunctionIndex : uint8_t
	{
		// Command buffer
		EXECUTE_COMMAND_BUFFER = 0,
		// Graphics
		SET_GRAPHICS_ROOT_SIGNATURE,
		SET_GRAPHICS_PIPELINE_STATE,
		SET_GRAPHICS_RESOURCE_GROUP,
		SET_GRAPHICS_VERTEX_ARRAY,			// Input-assembler (IA) stage
		SET_GRAPHICS_VIEWPORTS,				// Rasterizer (RS) stage
		SET_GRAPHICS_SCISSOR_RECTANGLES,	// Rasterizer (RS) stage
		SET_GRAPHICS_RENDER_TARGET,			// Output-merger (OM) stage
		CLEAR_GRAPHICS,
		DRAW_GRAPHICS,
		DRAW_INDEXED_GRAPHICS,
		DRAW_MESH_TASKS,
		// Compute
		SET_COMPUTE_ROOT_SIGNATURE,
		SET_COMPUTE_PIPELINE_STATE,
		SET_COMPUTE_RESOURCE_GROUP,
		DISPATCH_COMPUTE,
		// Resource
		SET_TEXTURE_MINIMUM_MAXIMUM_MIPMAP_INDEX,
		RESOLVE_MULTISAMPLE_FRAMEBUFFER,
		COPY_RESOURCE,
		GENERATE_MIPMAPS,
		// Query
		RESET_QUERY_POOL,
		BEGIN_QUERY,
		END_QUERY,
		WRITE_TIMESTAMP_QUERY,
		// Debug
		SET_DEBUG_MARKER,
		BEGIN_DEBUG_EVENT,
		END_DEBUG_EVENT,
		// Done
		NUMBER_OF_FUNCTIONS
	};

	typedef void (*ImplementationDispatchFunction)(const void*, IRhi& rhi);
	typedef void* CommandPacket;
	typedef const void* ConstCommandPacket;

	// Global functions
	namespace CommandPacketHelper
	{
		static constexpr uint32_t OFFSET_NEXT_COMMAND_PACKET_BYTE_INDEX		= 0u;
		static constexpr uint32_t OFFSET_IMPLEMENTATION_DISPATCH_FUNCTION	= OFFSET_NEXT_COMMAND_PACKET_BYTE_INDEX + sizeof(uint32_t);
		static constexpr uint32_t OFFSET_COMMAND							= OFFSET_IMPLEMENTATION_DISPATCH_FUNCTION + sizeof(uint32_t);	// Don't use "sizeof(CommandDispatchFunctionIndex)" instead of "sizeof(uint32_t)" so we have a known alignment

		template <typename T>
		[[nodiscard]] inline uint32_t getNumberOfBytes(uint32_t numberOfAuxiliaryBytes)
		{
			return OFFSET_COMMAND + sizeof(T) + numberOfAuxiliaryBytes;
		}

		[[nodiscard]] inline uint32_t getNextCommandPacketByteIndex(const CommandPacket commandPacket)
		{
			return *reinterpret_cast<const uint32_t*>(reinterpret_cast<const uint8_t*>(commandPacket) + OFFSET_NEXT_COMMAND_PACKET_BYTE_INDEX);
		}

		[[nodiscard]] inline uint32_t getNextCommandPacketByteIndex(const ConstCommandPacket constCommandPacket)
		{
			return *reinterpret_cast<const uint32_t*>(reinterpret_cast<const uint8_t*>(constCommandPacket) + OFFSET_NEXT_COMMAND_PACKET_BYTE_INDEX);
		}

		inline void storeNextCommandPacketByteIndex(const CommandPacket commandPacket, uint32_t nextPacketByteIndex)
		{
			*reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(commandPacket) + OFFSET_NEXT_COMMAND_PACKET_BYTE_INDEX) = nextPacketByteIndex;
		}

		[[nodiscard]] inline CommandDispatchFunctionIndex* getCommandDispatchFunctionIndex(const CommandPacket commandPacket)
		{
			return reinterpret_cast<CommandDispatchFunctionIndex*>(reinterpret_cast<uint8_t*>(commandPacket) + OFFSET_IMPLEMENTATION_DISPATCH_FUNCTION);
		}

		[[nodiscard]] inline const CommandDispatchFunctionIndex* getCommandDispatchFunctionIndex(const ConstCommandPacket constCommandPacket)
		{
			return reinterpret_cast<const CommandDispatchFunctionIndex*>(reinterpret_cast<const uint8_t*>(constCommandPacket) + OFFSET_IMPLEMENTATION_DISPATCH_FUNCTION);
		}

		inline void storeImplementationDispatchFunctionIndex(const CommandPacket commandPacket, CommandDispatchFunctionIndex commandDispatchFunctionIndex)
		{
			*getCommandDispatchFunctionIndex(commandPacket) = commandDispatchFunctionIndex;
		}

		[[nodiscard]] inline CommandDispatchFunctionIndex loadCommandDispatchFunctionIndex(const CommandPacket commandPacket)
		{
			return *getCommandDispatchFunctionIndex(commandPacket);
		}

		[[nodiscard]] inline CommandDispatchFunctionIndex loadCommandDispatchFunctionIndex(const ConstCommandPacket constCommandPacket)
		{
			return *getCommandDispatchFunctionIndex(constCommandPacket);
		}

		template <typename T>
		[[nodiscard]] inline T* getCommand(const CommandPacket commandPacket)
		{
			return reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(commandPacket) + OFFSET_COMMAND);
		}

		[[nodiscard]] inline const void* loadCommand(const CommandPacket commandPacket)
		{
			return reinterpret_cast<uint8_t*>(commandPacket) + OFFSET_COMMAND;
		}

		[[nodiscard]] inline const void* loadCommand(const ConstCommandPacket constCommandPacket)
		{
			return reinterpret_cast<const uint8_t*>(constCommandPacket) + OFFSET_COMMAND;
		}

		/**
		*  @brief
		*    Return auxiliary memory address of the given command; returned memory address is considered unstable and might change as soon as another command is added
		*/
		template <typename T>
		[[nodiscard]] inline uint8_t* getAuxiliaryMemory(T* command)
		{
			return reinterpret_cast<uint8_t*>(command) + sizeof(T);
		}

		/**
		*  @brief
		*    Return auxiliary memory address of the given command; returned memory address is considered unstable and might change as soon as another command is added
		*/
		template <typename T>
		[[nodiscard]] inline const uint8_t* getAuxiliaryMemory(const T* command)
		{
			return reinterpret_cast<const uint8_t*>(command) + sizeof(T);
		}

	}

	/**
	*  @brief
	*    Command buffer
	*
	*  @remarks
	*    Basing on
	*    - "Molecular Musings" - "Stateless, layered, multi-threaded rendering � Part 1" by Stefan Reinalter from November 6, 2014 - https://blog.molecular-matters.com/2014/11/06/stateless-layered-multi-threaded-rendering-part-1/
	*    - "Molecular Musings" - "Stateless, layered, multi-threaded rendering � Part 2" by Stefan Reinalter from November 13, 2014 - http://molecularmusings.wordpress.com/2014/11/13/stateless-layered-multi-threaded-rendering-part-2-stateless-api-design/
	*    - "Molecular Musings" - "Stateless, layered, multi-threaded rendering � Part 3" by Stefan Reinalter from December 16, 2014 - http://molecularmusings.wordpress.com/2014/12/16/stateless-layered-multi-threaded-rendering-part-3-api-design-details/
	*    - "realtimecollisiondetection.net � the blog" - "Order your graphics draw calls around!" by Christer Ericson from October 3, 2008 - http://realtimecollisiondetection.net/blog/?p=86
	*    but without a key inside the more general command buffer. Sorting is a job of a more high level construct like a render queue which also automatically will perform
	*    batching and instancing. Also the memory management is much simplified to be cache friendly.
	*
	*  @note
	*    - The commands are stored as a flat contiguous array to be cache friendly
	*    - Each command can have an additional auxiliary buffer, e.g. to store uniform buffer data to submit to the RHI
	*    - It's valid to record a command buffer only once, and submit it multiple times to the RHI
	*/
	class CommandBuffer final
	{

	// Public methods
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline CommandBuffer() :
			mCommandPacketBufferNumberOfBytes(0),
			mCommandPacketBuffer(nullptr),
			mPreviousCommandPacketByteIndex(~0u),
			mCurrentCommandPacketByteIndex(0)
			#ifdef SE_STATISTICS
				, mNumberOfCommands(0)
			#endif
		{}

		/**
		*  @brief
		*    Destructor
		*/
		inline ~CommandBuffer()
		{
			delete [] mCommandPacketBuffer;
		}

		/**
		*  @brief
		*    Return whether or not the command buffer is empty
		*
		*  @return
		*    "true" if the command buffer is empty, else "false"
		*/
		[[nodiscard]] inline bool isEmpty() const
		{
			return (~0u == mPreviousCommandPacketByteIndex);
		}

		#ifdef SE_STATISTICS
			/**
			*  @brief
			*    Return the number of commands inside the command buffer
			*
			*  @return
			*    The number of commands inside the command buffer
			*
			*  @note
			*    - Counting the number of commands inside the command buffer is only a debugging feature and not used in optimized builds
			*/
			[[nodiscard]] inline uint32_t getNumberOfCommands() const
			{
				return mNumberOfCommands;
			}
		#endif

		/**
		*  @brief
		*    Return the command packet buffer
		*
		*  @return
		*    The command packet buffer, can be a null pointer, don't destroy the instance
		*
		*  @note
		*    - Internal, don't access the method if you don't have to
		*/
		[[nodiscard]] inline const uint8_t* getCommandPacketBuffer() const
		{
			return (~0u != mPreviousCommandPacketByteIndex) ? mCommandPacketBuffer : nullptr;
		}

		/**
		*  @brief
		*    Clear the command buffer
		*/
		inline void clear()
		{
			mPreviousCommandPacketByteIndex = ~0u;
			mCurrentCommandPacketByteIndex = 0;
			#ifdef SE_STATISTICS
				mNumberOfCommands = 0;
			#endif
		}

		/**
		*  @brief
		*    Add command
		*
		*  @param[in] numberOfAuxiliaryBytes
		*    Optional number of auxiliary bytes, e.g. to store uniform buffer data to submit to the RHI
		*
		*  @return
		*    Pointer to the added command, only null pointer in case of apocalypse, don't destroy the memory
		*/
		template <typename U>
		[[nodiscard]] U* addCommand(uint32_t numberOfAuxiliaryBytes = 0)
		{
			// How many command package buffer bytes are consumed by the command to add?
			const uint32_t numberOfCommandBytes = CommandPacketHelper::getNumberOfBytes<U>(numberOfAuxiliaryBytes);

			// 4294967295 is the maximum value of an "uint32_t"-type: Check for overflow
			// -> We use the magic number here to avoid "std::numeric_limits::max()" usage
			ASSERT((static_cast<uint64_t>(mCurrentCommandPacketByteIndex) + numberOfCommandBytes) < 4294967295u, "Invalid current command packet byte index")

			// Grow command packet buffer, if required
			if (mCommandPacketBufferNumberOfBytes < mCurrentCommandPacketByteIndex + numberOfCommandBytes)
			{
				// Allocate new memory, grow using a known value but do also add the number of bytes consumed by the current command to add (many auxiliary bytes might be requested)
				const uint32_t newCommandPacketBufferNumberOfBytes = mCommandPacketBufferNumberOfBytes + NUMBER_OF_BYTES_TO_GROW + numberOfCommandBytes;
				uint8_t* newCommandPacketBuffer = new uint8_t[newCommandPacketBufferNumberOfBytes];

				// Copy over current command package buffer content and free it, if required
				if (nullptr != mCommandPacketBuffer)
				{
					memcpy(newCommandPacketBuffer, mCommandPacketBuffer, mCommandPacketBufferNumberOfBytes);
					delete [] mCommandPacketBuffer;
				}

				// Finalize
				mCommandPacketBuffer = newCommandPacketBuffer;
				mCommandPacketBufferNumberOfBytes = newCommandPacketBufferNumberOfBytes;
			}

			// Get command package for the new command
			CommandPacket commandPacket = &mCommandPacketBuffer[mCurrentCommandPacketByteIndex];

			// Setup previous and current command package
			if (~0u != mPreviousCommandPacketByteIndex)
			{
				CommandPacketHelper::storeNextCommandPacketByteIndex(&mCommandPacketBuffer[mPreviousCommandPacketByteIndex], mCurrentCommandPacketByteIndex);
			}
			CommandPacketHelper::storeNextCommandPacketByteIndex(commandPacket, ~0u);
			CommandPacketHelper::storeImplementationDispatchFunctionIndex(commandPacket, U::COMMAND_DISPATCH_FUNCTION_INDEX);
			mPreviousCommandPacketByteIndex = mCurrentCommandPacketByteIndex;
			mCurrentCommandPacketByteIndex += numberOfCommandBytes;

			// Done
			#ifdef SE_STATISTICS
				++mNumberOfCommands;
			#endif
			return CommandPacketHelper::getCommand<U>(commandPacket);
		}

		/**
		*  @brief
		*    Submit the command buffer to the RHI without flushing; use this for recording command buffers once and submit them multiple times
		*
		*  @param[in] rhi
		*    RHI to submit the command buffer to
		*/
		inline void submitToRhi(IRhi& rhi) const
		{
			rhi.submitCommandBuffer(*this);
		}

		/**
		*  @brief
		*    Submit the command buffer to the RHI and clear so the command buffer is empty again
		*
		*  @param[in] rhi
		*    RHI to submit the command buffer to
		*/
		inline void submitToRhiAndClear(IRhi& rhi)
		{
			rhi.submitCommandBuffer(*this);
			clear();
		}

		/**
		*  @brief
		*    Submit the command buffer to another command buffer without flushing; use this for recording command buffers once and submit them multiple times
		*
		*  @param[in] commandBuffer
		*    Command buffer to submit the command buffer to
		*/
		inline void submitToCommandBuffer(CommandBuffer& commandBuffer) const
		{
			// Sanity checks
			ASSERT(this != &commandBuffer, "Can't submit a command buffer to itself")
			ASSERT(!isEmpty(), "Can't submit empty command buffers")

			// How many command package buffer bytes are consumed by the command to add?
			const uint32_t numberOfCommandBytes = mCurrentCommandPacketByteIndex;

			// 4294967295 is the maximum value of an "uint32_t"-type: Check for overflow
			// -> We use the magic number here to avoid "std::numeric_limits::max()" usage
			ASSERT((static_cast<uint64_t>(commandBuffer.mCurrentCommandPacketByteIndex) + numberOfCommandBytes) < 4294967295u, "Invalid current command packet byte index")

			// Grow command packet buffer, if required
			if (commandBuffer.mCommandPacketBufferNumberOfBytes < commandBuffer.mCurrentCommandPacketByteIndex + numberOfCommandBytes)
			{
				// Allocate new memory, grow using a known value but do also add the number of bytes consumed by the current command to add (many auxiliary bytes might be requested)
				const uint32_t newCommandPacketBufferNumberOfBytes = commandBuffer.mCommandPacketBufferNumberOfBytes + NUMBER_OF_BYTES_TO_GROW + numberOfCommandBytes;
				uint8_t* newCommandPacketBuffer = new uint8_t[newCommandPacketBufferNumberOfBytes];

				// Copy over current command package buffer content and free it, if required
				if (nullptr != commandBuffer.mCommandPacketBuffer)
				{
					memcpy(newCommandPacketBuffer, commandBuffer.mCommandPacketBuffer, commandBuffer.mCommandPacketBufferNumberOfBytes);
					delete [] commandBuffer.mCommandPacketBuffer;
				}

				// Finalize
				commandBuffer.mCommandPacketBuffer = newCommandPacketBuffer;
				commandBuffer.mCommandPacketBufferNumberOfBytes = newCommandPacketBufferNumberOfBytes;
			}

			// Copy over the command buffer in one burst
			memcpy(&commandBuffer.mCommandPacketBuffer[commandBuffer.mCurrentCommandPacketByteIndex], mCommandPacketBuffer, mCurrentCommandPacketByteIndex);

			// Setup previous command package
			if (~0u != commandBuffer.mPreviousCommandPacketByteIndex)
			{
				CommandPacketHelper::storeNextCommandPacketByteIndex(&commandBuffer.mCommandPacketBuffer[commandBuffer.mPreviousCommandPacketByteIndex], commandBuffer.mCurrentCommandPacketByteIndex);
			}

			// Update command package indices
			CommandPacket commandPacket = &commandBuffer.mCommandPacketBuffer[commandBuffer.mCurrentCommandPacketByteIndex];
			uint32_t nextCommandPacketByteIndex = CommandPacketHelper::getNextCommandPacketByteIndex(commandPacket);
			while (~0u != nextCommandPacketByteIndex)
			{
				nextCommandPacketByteIndex = commandBuffer.mCurrentCommandPacketByteIndex + nextCommandPacketByteIndex;
				CommandPacketHelper::storeNextCommandPacketByteIndex(commandPacket, nextCommandPacketByteIndex);
				commandPacket = &commandBuffer.mCommandPacketBuffer[nextCommandPacketByteIndex];
				nextCommandPacketByteIndex = CommandPacketHelper::getNextCommandPacketByteIndex(commandPacket);
			}

			// Finalize
			commandBuffer.mPreviousCommandPacketByteIndex = commandBuffer.mCurrentCommandPacketByteIndex + mPreviousCommandPacketByteIndex;
			commandBuffer.mCurrentCommandPacketByteIndex += mCurrentCommandPacketByteIndex;
			#ifdef SE_STATISTICS
				commandBuffer.mNumberOfCommands += mNumberOfCommands;
			#endif
		}

		/**
		*  @brief
		*    Submit the command buffer to another command buffer and clear so the command buffer is empty again
		*
		*  @param[in] commandBuffer
		*    Command buffer to submit the command buffer to
		*/
		inline void submitToCommandBufferAndClear(CommandBuffer& commandBuffer)
		{
			submitToCommandBuffer(commandBuffer);
			clear();
		}

	// Private definitions
	private:
		static constexpr uint32_t NUMBER_OF_BYTES_TO_GROW = 8192;

	// Private data
	private:
		// Memory
		uint32_t mCommandPacketBufferNumberOfBytes;
		uint8_t* mCommandPacketBuffer;
		// Current state
		uint32_t mPreviousCommandPacketByteIndex;
		uint32_t mCurrentCommandPacketByteIndex;
		#ifdef SE_STATISTICS
			uint32_t mNumberOfCommands;
		#endif

	};

	// Concrete commands
	namespace Command
	{

		//[-------------------------------------------------------]
		//[ Command buffer                                        ]
		//[-------------------------------------------------------]
		struct ExecuteCommandBuffer final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, CommandBuffer* commandBufferToExecute)
			{
				ASSERT(nullptr != commandBufferToExecute, "Invalid command buffer to execute")
				*commandBuffer.addCommand<ExecuteCommandBuffer>() = ExecuteCommandBuffer(commandBufferToExecute);
			}
			// Constructor
			inline explicit ExecuteCommandBuffer(CommandBuffer* _commandBufferToExecute) :
				commandBufferToExecute(_commandBufferToExecute)
			{}
			// Data
			CommandBuffer* commandBufferToExecute;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::EXECUTE_COMMAND_BUFFER;
		};

		//[-------------------------------------------------------]
		//[ Graphics                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set the used graphics root signature
		*
		*  @param[in] rootSignature
		*    Graphics root signature to use, can be an null pointer (default: "nullptr")
		*/
		struct SetGraphicsRootSignature final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IRootSignature* rootSignature)
			{
				*commandBuffer.addCommand<SetGraphicsRootSignature>() = SetGraphicsRootSignature(rootSignature);
			}
			// Constructor
			inline explicit SetGraphicsRootSignature(IRootSignature* _rootSignature) :
				rootSignature(_rootSignature)
			{}
			// Data
			IRootSignature* rootSignature;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_ROOT_SIGNATURE;
		};

		/**
		*  @brief
		*    Set the used graphics pipeline state
		*
		*  @param[in] graphicsPipelineState
		*    Graphics pipeline state to use, can be an null pointer (default: "nullptr")
		*/
		struct SetGraphicsPipelineState final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IGraphicsPipelineState* graphicsPipelineState)
			{
				*commandBuffer.addCommand<SetGraphicsPipelineState>() = SetGraphicsPipelineState(graphicsPipelineState);
			}
			// Constructor
			inline explicit SetGraphicsPipelineState(IGraphicsPipelineState* _graphicsPipelineState) :
				graphicsPipelineState(_graphicsPipelineState)
			{}
			// Data
			IGraphicsPipelineState* graphicsPipelineState;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_PIPELINE_STATE;
		};

		/**
		*  @brief
		*    Set a graphics resource group
		*
		*  @param[in] rootParameterIndex
		*    The root parameter index number for binding
		*  @param[in] resourceGroup
		*    Resource group to set
		*/
		struct SetGraphicsResourceGroup final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, uint32_t rootParameterIndex, IResourceGroup* resourceGroup)
			{
				*commandBuffer.addCommand<SetGraphicsResourceGroup>() = SetGraphicsResourceGroup(rootParameterIndex, resourceGroup);
			}
			// Constructor
			inline SetGraphicsResourceGroup(uint32_t _rootParameterIndex, IResourceGroup* _resourceGroup) :
				rootParameterIndex(_rootParameterIndex),
				resourceGroup(_resourceGroup)
			{}
			// Data
			uint32_t		rootParameterIndex;
			IResourceGroup*	resourceGroup;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_RESOURCE_GROUP;
		};

		/**
		*  @brief
		*    Set the used vertex array, input-assembler (IA) stage
		*
		*  @param[in] vertexArray
		*    Vertex array to use, can be an null pointer (default: "nullptr")
		*/
		struct SetGraphicsVertexArray final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IVertexArray* vertexArray)
			{
				*commandBuffer.addCommand<SetGraphicsVertexArray>() = SetGraphicsVertexArray(vertexArray);
			}
			// Constructor
			inline explicit SetGraphicsVertexArray(IVertexArray* _vertexArray) :
				vertexArray(_vertexArray)
			{}
			// Data
			IVertexArray* vertexArray;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_VERTEX_ARRAY;
		};

		/**
		*  @brief
		*    Set the graphics viewports, rasterizer (RS) stage
		*
		*  @param[in] numberOfViewports
		*    Number of viewports, if <1 nothing happens, must be <="Rhi::Capabilities::maximumNumberOfViewports"
		*  @param[in] viewports
		*    C-array of viewports, there must be at least "numberOfViewports"-viewports, in case of a null pointer nothing happens
		*
		*  @note
		*    - The current viewport(s) does not affect the clear operation
		*    - Lookout! In Direct3D 12 the scissor test can't be deactivated and hence one always needs to set a valid scissor rectangle.
		*      Use the convenience "Rhi::Command::SetGraphicsViewportAndScissorRectangle"-command if possible to not walk into this Direct3D 12 trap.
		*/
		struct SetGraphicsViewports final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, uint32_t numberOfViewports, const Viewport* viewports)
			{
				*commandBuffer.addCommand<SetGraphicsViewports>() = SetGraphicsViewports(numberOfViewports, viewports);
			}
			static inline void create(CommandBuffer& commandBuffer, uint32_t topLeftX, uint32_t topLeftY, uint32_t width, uint32_t height, float minimumDepth = 0.0f, float maximumDepth = 1.0f)
			{
				SetGraphicsViewports* setGraphicsViewportsCommand = commandBuffer.addCommand<SetGraphicsViewports>(sizeof(Viewport));

				// Set command data
				Viewport* viewport = reinterpret_cast<Viewport*>(CommandPacketHelper::getAuxiliaryMemory(setGraphicsViewportsCommand));
				viewport->topLeftX = static_cast<float>(topLeftX);
				viewport->topLeftY = static_cast<float>(topLeftY);
				viewport->width	   = static_cast<float>(width);
				viewport->height   = static_cast<float>(height);
				viewport->minDepth = minimumDepth;
				viewport->maxDepth = maximumDepth;

				// Finalize command
				setGraphicsViewportsCommand->numberOfViewports = 1;
				setGraphicsViewportsCommand->viewports		   = nullptr;
			}
			// Constructor
			inline SetGraphicsViewports(uint32_t _numberOfViewports, const Viewport* _viewports) :
				numberOfViewports(_numberOfViewports),
				viewports(_viewports)
			{}
			// Data
			uint32_t		numberOfViewports;
			const Viewport* viewports;	///< If null pointer, command auxiliary memory is used instead
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_VIEWPORTS;
		};

		/**
		*  @brief
		*    Set the graphics scissor rectangles, rasterizer (RS) stage
		*
		*  @param[in] numberOfScissorRectangles
		*    Number of scissor rectangles, if <1 nothing happens, must be <="Rhi::Capabilities::maximumNumberOfViewports"
		*  @param[in] dcissorRectangles
		*    C-array of scissor rectangles, there must be at least "numberOfScissorRectangles" scissor rectangles, in case of a null pointer nothing happens
		*
		*  @note
		*    - Scissor rectangles are only used when "Rhi::RasterizerState::scissorEnable" is true
		*    - The current scissor rectangle(s) does not affect the clear operation
		*/
		struct SetGraphicsScissorRectangles final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, uint32_t numberOfScissorRectangles, const ScissorRectangle* scissorRectangles)
			{
				*commandBuffer.addCommand<SetGraphicsScissorRectangles>() = SetGraphicsScissorRectangles(numberOfScissorRectangles, scissorRectangles);
			}
			static inline void create(CommandBuffer& commandBuffer, long topLeftX, long topLeftY, long bottomRightX, long bottomRightY)
			{
				SetGraphicsScissorRectangles* setGraphicsScissorRectanglesCommand = commandBuffer.addCommand<SetGraphicsScissorRectangles>(sizeof(ScissorRectangle));

				// Set command data
				ScissorRectangle* scissorRectangle = reinterpret_cast<ScissorRectangle*>(CommandPacketHelper::getAuxiliaryMemory(setGraphicsScissorRectanglesCommand));
				scissorRectangle->topLeftX	   = topLeftX;
				scissorRectangle->topLeftY	   = topLeftY;
				scissorRectangle->bottomRightX = bottomRightX;
				scissorRectangle->bottomRightY = bottomRightY;

				// Finalize command
				setGraphicsScissorRectanglesCommand->numberOfScissorRectangles = 1;
				setGraphicsScissorRectanglesCommand->scissorRectangles		   = nullptr;
			}
			// Constructor
			inline SetGraphicsScissorRectangles(uint32_t _numberOfScissorRectangles, const ScissorRectangle* _scissorRectangles) :
				numberOfScissorRectangles(_numberOfScissorRectangles),
				scissorRectangles(_scissorRectangles)
			{}
			// Data
			uint32_t				numberOfScissorRectangles;
			const ScissorRectangle* scissorRectangles;	///< If null pointer, command auxiliary memory is used instead
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_SCISSOR_RECTANGLES;
		};

		/**
		*  @brief
		*    Set graphics viewport and scissor rectangle (convenience method)
		*
		*  @param[in] topLeftX
		*    Top left x
		*  @param[in] topLeftY
		*    Top left y
		*  @param[in] width
		*    Width
		*  @param[in] height
		*    Height
		*  @param[in] minimumDepth
		*    Minimum depth
		*  @param[in] maximumDepth
		*    Maximum depth
		*
		*  @note
		*    - Lookout! In Direct3D 12 the scissor test can't be deactivated and hence one always needs to set a valid scissor rectangle.
		*      Use the convenience "Rhi::Command::SetGraphicsViewportAndScissorRectangle"-command if possible to not walk into this Direct3D 12 trap.
		*/
		struct SetGraphicsViewportAndScissorRectangle final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, uint32_t topLeftX, uint32_t topLeftY, uint32_t width, uint32_t height, float minimumDepth = 0.0f, float maximumDepth = 1.0f)
			{
				// Set the graphics viewport
				SetGraphicsViewports::create(commandBuffer, topLeftX, topLeftY, width, height, minimumDepth, maximumDepth);

				// Set the graphics scissor rectangle
				SetGraphicsScissorRectangles::create(commandBuffer, static_cast<long>(topLeftX), static_cast<long>(topLeftY), static_cast<long>(topLeftX + width), static_cast<long>(topLeftY + height));
			}
		};

		/**
		*  @brief
		*    Set the graphics render target to render into, output-merger (OM) stage
		*
		*  @param[in] renderTarget
		*    Render target to render into by binding it to the output-merger state, can be an null pointer to render into the primary window
		*/
		struct SetGraphicsRenderTarget final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IRenderTarget* renderTarget)
			{
				*commandBuffer.addCommand<SetGraphicsRenderTarget>() = SetGraphicsRenderTarget(renderTarget);
			}
			// Constructor
			inline explicit SetGraphicsRenderTarget(IRenderTarget* _renderTarget) :
				renderTarget(_renderTarget)
			{}
			// Data
			IRenderTarget* renderTarget;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_RENDER_TARGET;
		};

		/**
		*  @brief
		*    Clears the graphics viewport to a specified RGBA color, clears the depth buffer,
		*    and erases the stencil buffer
		*
		*  @param[in] clearFlags
		*    Flags that indicate what should be cleared. This parameter can be any
		*    combination of the following flags, but at least one flag must be used:
		*    "Rhi::ClearFlag::COLOR", "Rhi::ClearFlag::DEPTH" and "Rhi::ClearFlag::STENCIL, see "Rhi::ClearFlag"-flags
		*  @param[in] color
		*    RGBA clear color (used if "Rhi::ClearFlag::COLOR" is set)
		*  @param[in] z
		*    Z clear value. (if "Rhi::ClearFlag::DEPTH" is set)
		*    This parameter can be in the range from 0.0 through 1.0. A value of 0.0
		*    represents the nearest distance to the viewer, and 1.0 the farthest distance.
		*  @param[in] stencil
		*    Value to clear the stencil-buffer with. This parameter can be in the range from
		*    0 through 2^n�1, where n is the bit depth of the stencil buffer.
		*
		*  @note
		*    - The current viewport(s) (see "Rhi::Command::SetGraphicsViewports()") does not affect the clear operation
		*    - The current scissor rectangle(s) (see "Rhi::Command::SetGraphicsScissorRectangles()") does not affect the clear operation
		*    - In case there are multiple active render targets, all render targets are cleared
		*/
		struct ClearGraphics final
		{
			// Static methods
			// -> z = 0 instead of 1 due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
			static inline void create(CommandBuffer& commandBuffer, uint32_t clearFlags, const float color[4], float z = 0.0f, uint32_t stencil = 0)
			{
				*commandBuffer.addCommand<ClearGraphics>() = ClearGraphics(clearFlags, color, z, stencil);
			}
			// Constructor
			inline ClearGraphics(uint32_t _clearFlags, const float _color[4], float _z, uint32_t _stencil) :
				clearFlags(_clearFlags),
				color{_color[0], _color[1], _color[2], _color[3]},
				z(_z),
				stencil(_stencil)
			{}
			// Data
			uint32_t clearFlags;
			float	 color[4];
			float	 z;
			uint32_t stencil;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::CLEAR_GRAPHICS;
		};

		/**
		*  @brief
		*    Render the specified geometric primitive, based on an array of vertices instancing and indirect draw
		*
		*  @param[in] indirectBuffer
		*    Indirect buffer to use, the indirect buffer must contain at least "numberOfDraws" instances of "Rhi::DrawArguments" starting at "indirectBufferOffset"
		*  @param[in] indirectBufferOffset
		*    Indirect buffer offset
		*  @param[in] numberOfDraws
		*    Number of draws, can be 0
		*
		*  @note
		*    - Draw instanced is a shader model 4 feature, only supported if "Rhi::Capabilities::drawInstanced" is true
		*    - In Direct3D 9, instanced arrays with hardware support is only possible when drawing indexed primitives, see
		*      "Efficiently Drawing Multiple Instances of Geometry (Direct3D 9)"-article at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb173349%28v=vs.85%29.aspx#Drawing_Non_Indexed_Geometry
		*    - Fails if no vertex array is set
		*    - If the multi-draw indirect feature is not supported this parameter, multiple draw calls are emitted
		*    - If the draw indirect feature is not supported, a software indirect buffer is used and multiple draw calls are emitted
		*/
		struct DrawGraphics final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, const IIndirectBuffer& indirectBuffer, uint32_t indirectBufferOffset = 0, uint32_t numberOfDraws = 1)
			{
				*commandBuffer.addCommand<DrawGraphics>() = DrawGraphics(indirectBuffer, indirectBufferOffset, numberOfDraws);
			}
			static inline void create(CommandBuffer& commandBuffer, uint32_t vertexCountPerInstance, uint32_t instanceCount = 1, uint32_t startVertexLocation = 0, uint32_t startInstanceLocation = 0)
			{
				DrawGraphics* drawCommand = commandBuffer.addCommand<DrawGraphics>(sizeof(DrawArguments));

				// Set command data: The command packet auxiliary memory contains an "Rhi::DrawArguments"-instance
				const DrawArguments drawArguments(vertexCountPerInstance, instanceCount, startVertexLocation, startInstanceLocation);
				memcpy(CommandPacketHelper::getAuxiliaryMemory(drawCommand), &drawArguments, sizeof(DrawArguments));

				// Finalize command
				drawCommand->indirectBuffer		  = nullptr;
				drawCommand->indirectBufferOffset = 0;
				drawCommand->numberOfDraws		  = 1;
			}
			// Constructor
			inline DrawGraphics(const IIndirectBuffer& _indirectBuffer, uint32_t _indirectBufferOffset, uint32_t _numberOfDraws) :
				indirectBuffer(&_indirectBuffer),
				indirectBufferOffset(_indirectBufferOffset),
				numberOfDraws(_numberOfDraws)
			{}
			// Data
			const IIndirectBuffer* indirectBuffer;	///< If null pointer, command auxiliary memory is used instead
			uint32_t			   indirectBufferOffset;
			uint32_t			   numberOfDraws;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::DRAW_GRAPHICS;
		};

		/**
		*  @brief
		*    Render the specified geometric primitive, based on indexing into an array of vertices, instancing and indirect draw
		*
		*  @param[in] indirectBuffer
		*    Indirect buffer to use, the indirect buffer must contain at least "numberOfDraws" instances of "Rhi::DrawIndexedArguments" starting at bindirectBufferOffset"
		*  @param[in] indirectBufferOffset
		*    Indirect buffer offset
		*  @param[in] numberOfDraws
		*    Number of draws, can be 0
		*
		*  @note
		*    - Instanced arrays is a shader model 3 feature, only supported if "Rhi::Capabilities::instancedArrays" is true
		*    - Draw instanced is a shader model 4 feature, only supported if "Rhi::Capabilities::drawInstanced" is true
		*    - This method draws indexed primitives from the current set of data input streams
		*    - Fails if no index and/or vertex array is set
		*    - If the multi-draw indirect feature is not supported this parameter, multiple draw calls are emitted
		*    - If the draw indirect feature is not supported, a software indirect buffer is used and multiple draw calls are emitted
		*/
		struct DrawIndexedGraphics final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, const IIndirectBuffer& indirectBuffer, uint32_t indirectBufferOffset = 0, uint32_t numberOfDraws = 1)
			{
				*commandBuffer.addCommand<DrawIndexedGraphics>() = DrawIndexedGraphics(indirectBuffer, indirectBufferOffset, numberOfDraws);
			}
			static inline void create(CommandBuffer& commandBuffer, uint32_t indexCountPerInstance, uint32_t instanceCount = 1, uint32_t startIndexLocation = 0, int32_t baseVertexLocation = 0, uint32_t startInstanceLocation = 0)
			{
				DrawIndexedGraphics* drawCommand = commandBuffer.addCommand<DrawIndexedGraphics>(sizeof(DrawIndexedArguments));

				// Set command data: The command packet auxiliary memory contains an "Rhi::DrawIndexedArguments"-instance
				const DrawIndexedArguments drawIndexedArguments(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
				memcpy(CommandPacketHelper::getAuxiliaryMemory(drawCommand), &drawIndexedArguments, sizeof(DrawIndexedArguments));

				// Finalize command
				drawCommand->indirectBuffer		  = nullptr;
				drawCommand->indirectBufferOffset = 0;
				drawCommand->numberOfDraws		  = 1;
			}
			// Constructor
			inline DrawIndexedGraphics(const IIndirectBuffer& _indirectBuffer, uint32_t _indirectBufferOffset, uint32_t _numberOfDraws) :
				indirectBuffer(&_indirectBuffer),
				indirectBufferOffset(_indirectBufferOffset),
				numberOfDraws(_numberOfDraws)
			{}
			// Data
			const IIndirectBuffer* indirectBuffer;	///< If null pointer, command auxiliary memory is used instead
			uint32_t			   indirectBufferOffset;
			uint32_t			   numberOfDraws;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::DRAW_INDEXED_GRAPHICS;
		};

		/**
		*  @brief
		*    Render the specified geometric primitive, based on a task and mesh shader and indirect draw
		*
		*  @param[in] indirectBuffer
		*    Indirect buffer to use, the indirect buffer must contain at least "numberOfDraws" instances of "Rhi::DrawMeshTasksArguments" starting at "indirectBufferOffset"
		*  @param[in] indirectBufferOffset
		*    Indirect buffer offset
		*  @param[in] numberOfDraws
		*    Number of draws, can be 0
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::meshShader" is "true"
		*/
		struct DrawMeshTasks final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, const IIndirectBuffer& indirectBuffer, uint32_t indirectBufferOffset = 0, uint32_t numberOfDraws = 1)
			{
				*commandBuffer.addCommand<DrawMeshTasks>() = DrawMeshTasks(indirectBuffer, indirectBufferOffset, numberOfDraws);
			}
			static inline void create(CommandBuffer& commandBuffer, uint32_t numberOfTasks, uint32_t firstTask = 0)
			{
				DrawMeshTasks* drawCommand = commandBuffer.addCommand<DrawMeshTasks>(sizeof(DrawMeshTasksArguments));

				// Set command data: The command packet auxiliary memory contains an "Rhi::DrawMeshTasksArguments"-instance
				const DrawMeshTasksArguments drawArguments(numberOfTasks, firstTask);
				memcpy(CommandPacketHelper::getAuxiliaryMemory(drawCommand), &drawArguments, sizeof(DrawMeshTasksArguments));

				// Finalize command
				drawCommand->indirectBuffer		  = nullptr;
				drawCommand->indirectBufferOffset = 0;
				drawCommand->numberOfDraws		  = 1;
			}
			// Constructor
			inline DrawMeshTasks(const IIndirectBuffer& _indirectBuffer, uint32_t _indirectBufferOffset, uint32_t _numberOfDraws) :
				indirectBuffer(&_indirectBuffer),
				indirectBufferOffset(_indirectBufferOffset),
				numberOfDraws(_numberOfDraws)
			{}
			// Data
			const IIndirectBuffer* indirectBuffer;	///< If null pointer, command auxiliary memory is used instead
			uint32_t			   indirectBufferOffset;
			uint32_t			   numberOfDraws;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::DRAW_MESH_TASKS;
		};

		//[-------------------------------------------------------]
		//[ Compute                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set the used compute root signature
		*
		*  @param[in] rootSignature
		*    Compute root signature to use, can be an null pointer (default: "nullptr")
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::computeShader" is true
		*/
		struct SetComputeRootSignature final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IRootSignature* rootSignature)
			{
				*commandBuffer.addCommand<SetComputeRootSignature>() = SetComputeRootSignature(rootSignature);
			}
			// Constructor
			inline explicit SetComputeRootSignature(IRootSignature* _rootSignature) :
				rootSignature(_rootSignature)
			{}
			// Data
			IRootSignature* rootSignature;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_COMPUTE_ROOT_SIGNATURE;
		};

		/**
		*  @brief
		*    Set the used compute pipeline state
		*
		*  @param[in] computePipelineState
		*    Compute pipeline state to use, can be an null pointer (default: "nullptr")
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::computeShader" is true
		*/
		struct SetComputePipelineState final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IComputePipelineState* computePipelineState)
			{
				*commandBuffer.addCommand<SetComputePipelineState>() = SetComputePipelineState(computePipelineState);
			}
			// Constructor
			inline explicit SetComputePipelineState(IComputePipelineState* _computePipelineState) :
				computePipelineState(_computePipelineState)
			{}
			// Data
			IComputePipelineState* computePipelineState;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_COMPUTE_PIPELINE_STATE;
		};

		/**
		*  @brief
		*    Set a compute resource group
		*
		*  @param[in] rootParameterIndex
		*    The root parameter index number for binding
		*  @param[in] resourceGroup
		*    Resource group to set
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::computeShader" is true
		*/
		struct SetComputeResourceGroup final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, uint32_t rootParameterIndex, IResourceGroup* resourceGroup)
			{
				*commandBuffer.addCommand<SetComputeResourceGroup>() = SetComputeResourceGroup(rootParameterIndex, resourceGroup);
			}
			// Constructor
			inline SetComputeResourceGroup(uint32_t _rootParameterIndex, IResourceGroup* _resourceGroup) :
				rootParameterIndex(_rootParameterIndex),
				resourceGroup(_resourceGroup)
			{}
			// Data
			uint32_t		rootParameterIndex;
			IResourceGroup*	resourceGroup;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_COMPUTE_RESOURCE_GROUP;
		};

		/**
		*  @brief
		*    Compute dispatch call
		*
		*  @param[in] groupCountX
		*    Number of local workgroups to dispatch in the X dimension
		*  @param[in] groupCountY
		*    Number of local workgroups to dispatch in the Y dimension
		*  @param[in] groupCountZ
		*    Number of local workgroups to dispatch in the Z dimension
		*
		*  @note
		*    - Only supported if "Rhi::Capabilities::computeShader" is true
		*/
		struct DispatchCompute final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
			{
				*commandBuffer.addCommand<DispatchCompute>() = DispatchCompute(groupCountX, groupCountY, groupCountZ);
			}
			// Constructor
			inline DispatchCompute(uint32_t _groupCountX, uint32_t _groupCountY, uint32_t _groupCountZ) :
				groupCountX(_groupCountX),
				groupCountY(_groupCountY),
				groupCountZ(_groupCountZ)
			{}
			// Data
			uint32_t groupCountX;
			uint32_t groupCountY;
			uint32_t groupCountZ;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::DISPATCH_COMPUTE;
		};

		//[-------------------------------------------------------]
		//[ Resource                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set texture minimum maximum mipmap index
		*
		*  @param[in] texture
		*    Texture to set the minimum maximum mipmap index of
		*  @param[in] minimumMipmapIndex
		*    Minimum mipmap index, the most detailed mipmap, also known as base mipmap, 0 by default
		*  @param[in] maximumMipmapIndex
		*    Maximum mipmap index, the least detailed mipmap, <number of mipmaps> by default
		*/
		struct SetTextureMinimumMaximumMipmapIndex final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, ITexture& texture, uint32_t minimumMipmapIndex, uint32_t maximumMipmapIndex)
			{
				*commandBuffer.addCommand<SetTextureMinimumMaximumMipmapIndex>() = SetTextureMinimumMaximumMipmapIndex(texture, minimumMipmapIndex, maximumMipmapIndex);
			}
			// Constructor
			inline SetTextureMinimumMaximumMipmapIndex(ITexture& _texture, uint32_t _minimumMipmapIndex, uint32_t _maximumMipmapIndex) :
				texture(&_texture),
				minimumMipmapIndex(_minimumMipmapIndex),
				maximumMipmapIndex(_maximumMipmapIndex)
			{}
			// Data
			ITexture* texture;
			uint32_t  minimumMipmapIndex;
			uint32_t  maximumMipmapIndex;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_TEXTURE_MINIMUM_MAXIMUM_MIPMAP_INDEX;
		};

		/**
		*  @brief
		*    Resolve multisample framebuffer
		*
		*  @param[in] destinationRenderTarget
		*    None multisample destination render target
		*  @param[in] sourceMultisampleFramebuffer
		*    Source multisample framebuffer
		*/
		struct ResolveMultisampleFramebuffer final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IRenderTarget& destinationRenderTarget, IFramebuffer& sourceMultisampleFramebuffer)
			{
				*commandBuffer.addCommand<ResolveMultisampleFramebuffer>() = ResolveMultisampleFramebuffer(destinationRenderTarget, sourceMultisampleFramebuffer);
			}
			// Constructor
			inline ResolveMultisampleFramebuffer(IRenderTarget& _destinationRenderTarget, IFramebuffer& _sourceMultisampleFramebuffer) :
				destinationRenderTarget(&_destinationRenderTarget),
				sourceMultisampleFramebuffer(&_sourceMultisampleFramebuffer)
			{}
			// Data
			IRenderTarget* destinationRenderTarget;
			IFramebuffer* sourceMultisampleFramebuffer;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::RESOLVE_MULTISAMPLE_FRAMEBUFFER;
		};

		/**
		*  @brief
		*    Copy resource
		*
		*  @param[in] destinationResource
		*    Destination resource
		*  @param[in] sourceResource
		*    Source Resource
		*/
		struct CopyResource final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IResource& destinationResource, IResource& sourceResource)
			{
				*commandBuffer.addCommand<CopyResource>() = CopyResource(destinationResource, sourceResource);
			}
			// Constructor
			inline CopyResource(IResource& _destinationResource, IResource& _sourceResource) :
				destinationResource(&_destinationResource),
				sourceResource(&_sourceResource)
			{}
			// Data
			IResource* destinationResource;
			IResource* sourceResource;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::COPY_RESOURCE;
		};

		/**
		*  @brief
		*    Generate resource mipmaps
		*
		*  @param[in] resource
		*    Resource
		*/
		struct GenerateMipmaps final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IResource& resource)
			{
				*commandBuffer.addCommand<GenerateMipmaps>() = GenerateMipmaps(resource);
			}
			// Constructor
			inline explicit GenerateMipmaps(IResource& _resource) :
				resource(&_resource)
			{}
			// Data
			IResource* resource;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::GENERATE_MIPMAPS;
		};

		//[-------------------------------------------------------]
		//[ Query                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Reset asynchronous query pool
		*
		*  @param[in] queryPool
		*    Query pool
		*  @param[in] firstQueryIndex
		*    First query index (e.g. 0)
		*  @param[in] numberOfQueries
		*    Number of queries (e.g. 1)
		*/
		struct ResetQueryPool final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IQueryPool& queryPool, uint32_t firstQueryIndex = 0, uint32_t numberOfQueries = 1)
			{
				*commandBuffer.addCommand<ResetQueryPool>() = ResetQueryPool(queryPool, firstQueryIndex, numberOfQueries);
			}
			// Constructor
			inline ResetQueryPool(IQueryPool& _queryPool, uint32_t _firstQueryIndex, uint32_t _numberOfQueries) :
				queryPool(&_queryPool),
				firstQueryIndex(_firstQueryIndex),
				numberOfQueries(_numberOfQueries)
			{}
			// Data
			IQueryPool* queryPool;
			uint32_t	firstQueryIndex;
			uint32_t	numberOfQueries;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::RESET_QUERY_POOL;
		};

		/**
		*  @brief
		*    Begin asynchronous query
		*
		*  @param[in] queryPool
		*    Query pool
		*  @param[in] queryIndex
		*    Query index (e.g. 0)
		*  @param[in] queryControlFlags
		*    Query control flags (e.g. "Rhi::QueryControlFlags::PRECISE")
		*/
		struct BeginQuery final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IQueryPool& queryPool, uint32_t queryIndex = 0, uint32_t queryControlFlags = 0)
			{
				*commandBuffer.addCommand<BeginQuery>() = BeginQuery(queryPool, queryIndex, queryControlFlags);
			}
			// Constructor
			inline BeginQuery(IQueryPool& _queryPool, uint32_t _queryIndex, uint32_t _queryControlFlags) :
				queryPool(&_queryPool),
				queryIndex(_queryIndex),
				queryControlFlags(_queryControlFlags)
			{}
			// Data
			IQueryPool* queryPool;
			uint32_t	queryIndex;
			uint32_t	queryControlFlags;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::BEGIN_QUERY;
		};

		/**
		*  @brief
		*    Reset and begin asynchronous query; ease-of-use function in case there's just a single query inside the query pool
		*
		*  @param[in] queryPool
		*    Query pool
		*  @param[in] queryIndex
		*    Query index (e.g. 0)
		*  @param[in] queryControlFlags
		*    Query control flags (e.g. "Rhi::QueryControlFlags::PRECISE")
		*/
		struct ResetAndBeginQuery final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IQueryPool& queryPool, uint32_t queryIndex = 0, uint32_t queryControlFlags = 0)
			{
				*commandBuffer.addCommand<ResetQueryPool>() = ResetQueryPool(queryPool, queryIndex, 1);
				*commandBuffer.addCommand<BeginQuery>() = BeginQuery(queryPool, queryIndex, queryControlFlags);
			}
		};

		/**
		*  @brief
		*    End asynchronous query
		*
		*  @param[in] queryPool
		*    Query pool
		*  @param[in] queryIndex
		*    Query index (e.g. 0)
		*/
		struct EndQuery final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IQueryPool& queryPool, uint32_t queryIndex = 0)
			{
				*commandBuffer.addCommand<EndQuery>() = EndQuery(queryPool, queryIndex);
			}
			// Constructor
			inline EndQuery(IQueryPool& _queryPool, uint32_t _queryIndex) :
				queryPool(&_queryPool),
				queryIndex(_queryIndex)
			{}
			// Data
			IQueryPool* queryPool;
			uint32_t	queryIndex;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::END_QUERY;
		};

		/**
		*  @brief
		*    Write asynchronous timestamp query
		*
		*  @param[in] queryPool
		*    Query pool
		*  @param[in] queryIndex
		*    Query index (e.g. 0)
		*/
		struct WriteTimestampQuery final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, IQueryPool& queryPool, uint32_t queryIndex = 0)
			{
				*commandBuffer.addCommand<WriteTimestampQuery>() = WriteTimestampQuery(queryPool, queryIndex);
			}
			// Constructor
			inline WriteTimestampQuery(IQueryPool& _queryPool, uint32_t _queryIndex) :
				queryPool(&_queryPool),
				queryIndex(_queryIndex)
			{}
			// Data
			IQueryPool* queryPool;
			uint32_t	queryIndex;
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::WRITE_TIMESTAMP_QUERY;
		};

		//[-------------------------------------------------------]
		//[ Debug                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set a debug marker
		*
		*  @param[in] name
		*    ASCII name of the debug marker, must be valid (there's no internal null pointer test)
		*
		*  @see
		*    - "isDebugEnabled()"
		*/
		struct SetDebugMarker final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, const char* name)
			{
				*commandBuffer.addCommand<SetDebugMarker>() = SetDebugMarker(name);
			}
			// Constructor
			inline explicit SetDebugMarker(const char* _name)
			{
				ASSERT(strlen(_name) < 128, "Invalid name")
				strncpy(name, _name, 128);
				name[127] = '\0';
			}
			// Data
			char name[128];
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_DEBUG_MARKER;
		};

		/**
		*  @brief
		*    Begin debug event
		*
		*  @param[in] name
		*    ASCII name of the debug event, must be valid (there's no internal null pointer test)
		*
		*  @see
		*    - "isDebugEnabled()"
		*/
		struct BeginDebugEvent final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer, const char* name)
			{
				*commandBuffer.addCommand<BeginDebugEvent>() = BeginDebugEvent(name);
			}
			// Constructor
			inline explicit BeginDebugEvent(const char* _name)
			{
				ASSERT(strlen(_name) < 128, "Invalid name")
				strncpy(name, _name, 128);
				name[127] = '\0';
			}
			// Data
			char name[128];
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::BEGIN_DEBUG_EVENT;
		};

		/**
		*  @brief
		*    End the last started debug event
		*
		*  @see
		*    - "isDebugEnabled()"
		*/
		struct EndDebugEvent final
		{
			// Static methods
			static inline void create(CommandBuffer& commandBuffer)
			{
				*commandBuffer.addCommand<EndDebugEvent>() = EndDebugEvent();
			}
			// Static data
			static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::END_DEBUG_EVENT;
		};

	}

	// Debug macros
	#if SE_DEBUG
		/**
		*  @brief
		*    Set a debug marker
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*  @param[in] name
		*    ASCII name of the debug marker
		*/
		#define COMMAND_SET_DEBUG_MARKER(commandBuffer, name) Rhi::Command::SetDebugMarker::create(commandBuffer, name);

		/**
		*  @brief
		*    Set a debug marker by using the current function name ("__FUNCTION__") as marker name
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_SET_DEBUG_MARKER_FUNCTION(commandBuffer) Rhi::Command::SetDebugMarker::create(commandBuffer, __FUNCTION__);

		/**
		*  @brief
		*    Begin debug event
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use, must be ended by using "COMMAND_END_DEBUG_EVENT()"
		*  @param[in] name
		*    ASCII name of the debug event
		*/
		#define COMMAND_BEGIN_DEBUG_EVENT(commandBuffer, name) Rhi::Command::BeginDebugEvent::create(commandBuffer, name);

		/**
		*  @brief
		*    Begin debug event by using the current function name ("__FUNCTION__") as event name, must be ended by using "COMMAND_END_DEBUG_EVENT()"
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_BEGIN_DEBUG_EVENT_FUNCTION(commandBuffer) Rhi::Command::BeginDebugEvent::create(commandBuffer, __FUNCTION__);

		/**
		*  @brief
		*    End the last started debug event
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_END_DEBUG_EVENT(commandBuffer) Rhi::Command::EndDebugEvent::create(commandBuffer);

		/**
		*  @brief
		*    Scoped debug event class
		*/
		class CommandScopedDebugEventOnExit
		{
		// Public methods
		public:
			inline explicit CommandScopedDebugEventOnExit(CommandBuffer& commandBuffer) :
				mCommandBuffer(commandBuffer)
			{}

			inline ~CommandScopedDebugEventOnExit()
			{
				Rhi::Command::EndDebugEvent::create(mCommandBuffer);
			}

		// Private methods
		private:
			explicit CommandScopedDebugEventOnExit(const CommandScopedDebugEventOnExit& commandScopedDebugEventOnExit) = delete;
			CommandScopedDebugEventOnExit& operator =(const CommandScopedDebugEventOnExit& commandScopedDebugEventOnExit) = delete;

		// Private data
		private:
			CommandBuffer& mCommandBuffer;
		};

		/**
		*  @brief
		*    Scoped debug event, minor internal overhead compared to manual begin/end
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*  @param[in] name
		*    ASCII name of the debug event
		*/
		#define COMMAND_SCOPED_DEBUG_EVENT(commandBuffer, name) \
			Rhi::Command::BeginDebugEvent::create(commandBuffer, name); \
			SE_PRAGMA_WARNING_PUSH \
				SE_PRAGMA_WARNING_DISABLE_MSVC(4456) \
				Rhi::CommandScopedDebugEventOnExit commandScopedDebugEventOnExit##__FUNCTION__(commandBuffer); \
			SE_PRAGMA_WARNING_POP

		/**
		*  @brief
		*    Scoped debug event by using the current function name ("__FUNCTION__") as event name, minor internal overhead compared to manual begin/end
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(commandBuffer) \
			Rhi::Command::BeginDebugEvent::create(commandBuffer, __FUNCTION__); \
			SE_PRAGMA_WARNING_PUSH \
				SE_PRAGMA_WARNING_DISABLE_MSVC(4456) \
				Rhi::CommandScopedDebugEventOnExit commandScopedDebugEventOnExit##__FUNCTION__(commandBuffer); \
			SE_PRAGMA_WARNING_POP
	#else
		/**
		*  @brief
		*    Set a debug marker
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*  @param[in] name
		*    ASCII name of the debug marker
		*/
		#define COMMAND_SET_DEBUG_MARKER(commandBuffer, name)

		/**
		*  @brief
		*    Set a debug marker by using the current function name ("__FUNCTION__") as marker name
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_SET_DEBUG_MARKER_FUNCTION(commandBuffer)

		/**
		*  @brief
		*    Begin debug event, must be ended by using "COMMAND_END_DEBUG_EVENT()"
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*  @param[in] name
		*    ASCII name of the debug event
		*/
		#define COMMAND_BEGIN_DEBUG_EVENT(commandBuffer, name)

		/**
		*  @brief
		*    Begin debug event by using the current function name ("__FUNCTION__") as event name, must be ended by using "COMMAND_END_DEBUG_EVENT()"
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_BEGIN_DEBUG_EVENT_FUNCTION(commandBuffer)

		/**
		*  @brief
		*    End the last started debug event
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_END_DEBUG_EVENT(commandBuffer)

		/**
		*  @brief
		*    Scoped debug event, minor internal overhead compared to manual begin/end
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*  @param[in] name
		*    ASCII name of the debug event
		*/
		#define COMMAND_SCOPED_DEBUG_EVENT(commandBuffer, name)

		/**
		*  @brief
		*    Scoped debug event by using the current function name ("__FUNCTION__") as event name, minor internal overhead compared to manual begin/end
		*
		*  @param[in] commandBuffer
		*    Reference to the RHI instance to use
		*/
		#define COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(commandBuffer)
	#endif




//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Rhi




//[-------------------------------------------------------]
//[ Debug macros                                          ]
//[-------------------------------------------------------]
#if SE_DEBUG
	/**
	*  @brief
	*    Set a debug marker
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*  @param[in] name
	*    ASCII name of the debug marker
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_SET_DEBUG_MARKER(rhi, name) if (nullptr != rhi) { (rhi)->setDebugMarker(name); }

	/**
	*  @brief
	*    Set a debug marker by using the current function name ("__FUNCTION__") as marker name
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_SET_DEBUG_MARKER_FUNCTION(rhi) if (nullptr != rhi) { (rhi)->setDebugMarker(__FUNCTION__); }

	/**
	*  @brief
	*    Begin debug event
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*  @param[in] name
	*    ASCII name of the debug event
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_BEGIN_DEBUG_EVENT(rhi, name) if (nullptr != rhi) { (rhi)->beginDebugEvent(name); }

	/**
	*  @brief
	*    Begin debug event by using the current function name ("__FUNCTION__") as event name
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_BEGIN_DEBUG_EVENT_FUNCTION(rhi) if (nullptr != rhi) { (rhi)->beginDebugEvent(__FUNCTION__); }

	/**
	*  @brief
	*    End the last started debug event
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_END_DEBUG_EVENT(rhi) if (nullptr != rhi) { (rhi)->endDebugEvent(); }

	/**
	*  @brief
	*    Resource name for debugging purposes, ignored when not using "SE_DEBUG"
	*
	*  @param[in] name
	*    ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
	*/
	#define RHI_RESOURCE_DEBUG_NAME(name) , name

	/**
	*  @brief
	*    Decorate the debug name to make it easier to see the semantic of the resource
	*
	*  @param[in] name
	*    Debug name provided from the outside
	*  @param[in] decoration
	*    Decoration to append in front (e.g. "IBO", will result in appended "IBO: " in front if the provided name isn't empty)
	*  @param[in] numberOfDecorationCharacters
	*    Number of decoration characters
	*
	*  @note
	*    - The result is in local string variable "detailedName"
	*    - Traditional C-string on the runtime stack used for efficiency reasons (just for debugging, but must still be some kind of usable)
	*/
	#define RHI_DECORATED_DEBUG_NAME(name, detailedName, decoration, numberOfDecorationCharacters) \
		RHI_ASSERT(strlen(name) < 256, "Name is not allowed to exceed 255 characters") \
		char detailedName[256 + numberOfDecorationCharacters] = decoration; \
		if (name[0] != '\0') \
		{ \
			strcat(detailedName, ": "); \
			strncat(detailedName, name, 256); \
		}
#else
	/**
	*  @brief
	*    Set a debug marker
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*  @param[in] name
	*    ASCII name of the debug marker
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_SET_DEBUG_MARKER(rhi, name)

	/**
	*  @brief
	*    Set a debug marker by using the current function name ("__FUNCTION__") as marker name
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_SET_DEBUG_MARKER_FUNCTION(rhi)

	/**
	*  @brief
	*    Begin debug event
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*  @param[in] name
	*    ASCII name of the debug event
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_BEGIN_DEBUG_EVENT(rhi, name)

	/**
	*  @brief
	*    Begin debug event by using the current function name ("__FUNCTION__") as event name
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_BEGIN_DEBUG_EVENT_FUNCTION(rhi)

	/**
	*  @brief
	*    End the last started debug event
	*
	*  @param[in] rhi
	*    Pointer to the RHI instance to use, can be a null pointer
	*
	*  @note
	*    - Only for RHI implementation internal usage, don't expose it inside the public RHI header
	*/
	#define RHI_END_DEBUG_EVENT(rhi)

	/**
	*  @brief
	*    Resource name for debugging purposes, ignored when not using "SE_DEBUG"
	*
	*  @param[in] name
	*    ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
	*/
	#define RHI_RESOURCE_DEBUG_NAME(name)
#endif
