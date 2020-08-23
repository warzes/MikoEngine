#pragma once

#include "Renderer/Core/Manager.h"
#include "Renderer/Core/StringId.h"
#include "RHI/Rhi.h"

struct ImGuiContext;
namespace Renderer
{
	class IRenderer;
	class CompositorWorkspaceInstance;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringId			 AssetId;	// Asset identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef std::vector<AssetId> AssetIds;


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Debug GUI manager using ImGui ( https://github.com/ocornut/imgui )
	*
	*  @remarks
	*    Supports two command buffer fill modes:
	*    - Using fixed build in RHI configuration, including shaders
	*    - Using a material resource blueprint set by the caller
	*/
	class DebugGuiManager : private Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererImpl;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Set ImGui allocator functions
		*
		*  @param[in] allocator
		*    RHI allocator to use
		*/
		static void setImGuiAllocatorFunctions(DefaultAllocator& allocator);

		/**
		*  @brief
		*    Return the asset IDs of automatically generated dynamic default texture assets
		*
		*  @param[out] assetIds
		*    Receives the asset IDs of automatically generated dynamic default texture assets, the list is not cleared before new entries are added
		*
		*  @remarks
		*    The debug GUI manager automatically generates some dynamic default texture assets one can reference e.g. inside material blueprint resources:
		*    - "Engine/Texture/DynamicByCode/ImGuiGlyphMap2D"
		*/
		static void getDefaultTextureAssetIds(AssetIds& assetIds);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		void newFrame(Rhi::IRenderTarget& renderTarget, CompositorWorkspaceInstance* compositorWorkspaceInstance = nullptr);
		[[nodiscard]] const Rhi::IVertexArrayPtr& getFillVertexArrayPtr();
		void fillGraphicsCommandBuffer(Rhi::CommandBuffer& commandBuffer);
		void fillGraphicsCommandBufferUsingFixedBuildInRhiConfiguration(Rhi::CommandBuffer& commandBuffer);

		// Helper
		inline bool hasOpenMetricsWindow() const
		{
			return mOpenMetricsWindow;
		}

		inline void openMetricsWindow()
		{
			mOpenMetricsWindow = true;
		}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::DebugGuiManager methods   ]
	//[-------------------------------------------------------]
	protected:
		virtual void initializeImGuiKeyMap() = 0;
		virtual void startup();
		virtual void onNewFrame(Rhi::IRenderTarget& renderTarget) = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		explicit DebugGuiManager(IRenderer& renderer);
		virtual ~DebugGuiManager();
		explicit DebugGuiManager(const DebugGuiManager&) = delete;
		DebugGuiManager& operator=(const DebugGuiManager&) = delete;
		void createFixedBuildInRhiConfigurationResources();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IRenderer&		   mRenderer;				// Renderer instance, do not destroy the instance
		ImGuiContext*	   mImGuiContext;			// ImGui context, always valid
		std::string		   mAbsoluteIniFilename;	// Absolute UTF-8 ImGui ini-filename, class member since ImGui just keeps a pointer to this string instance
		std::string		   mAbsoluteLogFilename;	// Absolute UTF-8 ImGui log-filename, class member since ImGui just keeps a pointer to this string instance
		bool			   mIsRunning;				// The debug GUI manager will be initialized lazy when "Renderer::DebugGuiManager::newFrame()" is called for the first time
		Rhi::ITexture2DPtr mTexture2D;
		// Fixed build in RHI configuration resources
		Rhi::IRootSignaturePtr			mRootSignature;
		Rhi::IGraphicsProgramPtr		mGraphicsProgram;
		Rhi::IGraphicsPipelineStatePtr	mGraphicsPipelineState;
		Rhi::IUniformBufferPtr			mVertexShaderUniformBuffer;
		handle						mObjectSpaceToClipSpaceMatrixUniformHandle;
		Rhi::IResourceGroupPtr			mResourceGroup;		// Resource group, can be a null pointer
		Rhi::IResourceGroupPtr			mSamplerStateGroup;	// Sampler state resource group, can be a null pointer
		// Vertex and index buffer
		Rhi::IVertexBufferPtr mVertexBufferPtr;
		uint32_t			  mNumberOfAllocatedVertices;
		Rhi::IIndexBufferPtr  mIndexBufferPtr;
		uint32_t			  mNumberOfAllocatedIndices;
		Rhi::IVertexArrayPtr  mVertexArrayPtr;
		// Helper
		bool mOpenMetricsWindow;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
