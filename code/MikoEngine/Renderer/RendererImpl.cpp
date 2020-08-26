#include "stdafx.h"
#include "Renderer/RendererImpl.h"
#include "Renderer/Asset/AssetManager.h"
#include "Renderer/Core/File/MemoryFile.h"
#include "Renderer/Core/Time/TimeManager.h"
#include "Renderer/Core/File/IFileManager.h"
#include "Renderer/Core/Thread/ThreadPool.h"
#include "Renderer/Resource/ResourceStreamer.h"
#include "Renderer/Resource/RendererResourceManager.h"
#include "Renderer/Resource/Mesh/MeshResourceManager.h"
#include "Renderer/Resource/Scene/SceneResourceManager.h"
#include "Renderer/Resource/ShaderPiece/ShaderPieceResourceManager.h"
#include "Renderer/Resource/ShaderBlueprint/ShaderBlueprintResourceManager.h"
#include "Renderer/Resource/VertexAttributes/VertexAttributesResourceManager.h"
#include "Renderer/Resource/Texture/TextureResourceManager.h"
#include "Renderer/Resource/MaterialBlueprint/MaterialBlueprintResourceManager.h"
#include "Renderer/Resource/MaterialBlueprint/Cache/GraphicsPipelineStateCompiler.h"
#include "Renderer/Resource/MaterialBlueprint/Cache/ComputePipelineStateCompiler.h"
#include "Renderer/Resource/MaterialBlueprint/BufferManager/LightBufferManager.h"
#include "Renderer/Resource/MaterialBlueprint/Listener/MaterialBlueprintResourceListener.h"
#include "Renderer/Resource/Material/MaterialResourceManager.h"
#include "Renderer/Resource/Skeleton/SkeletonResourceManager.h"
#include "Renderer/Resource/SkeletonAnimation/SkeletonAnimationResourceManager.h"
#include "Renderer/Resource/CompositorNode/CompositorNodeResourceManager.h"
#include "Renderer/Resource/CompositorWorkspace/CompositorWorkspaceResourceManager.h"
#ifdef RENDERER_IMGUI
	#ifdef _WIN32
		#include "Renderer/DebugGui/Detail/DebugGuiManagerWindows.h"
	#elif LINUX
		#include "Renderer/DebugGui/Detail/DebugGuiManagerLinux.h"
	#endif
#endif
#include "Renderer/Context.h"

#include <cstring>


//[-------------------------------------------------------]
//[ Global functions                                      ]
//[-------------------------------------------------------]
// Export the instance creation function
Renderer::IRenderer* createRendererInstance(Renderer::Context& context)
{
	return RHI_NEW(Renderer::RendererImpl)(context);
}


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		namespace PipelineStateCache
		{
			static constexpr uint32_t FORMAT_TYPE	 = SE_STRING_ID("PipelineStateCache");
			static constexpr uint32_t FORMAT_VERSION = 1;
		}


		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		void getPipelineStateObjectCacheFilename(const Renderer::IRenderer& renderer, std::string& virtualDirectoryName, std::string& virtualFilename)
		{
			virtualDirectoryName = renderer.getFileManager().getLocalDataMountPoint();
			virtualFilename = virtualDirectoryName + '/' + renderer.getRhi().getName() + ".pso_cache";
		}

		[[nodiscard]] bool loadPipelineStateObjectCacheFile(const Renderer::IRenderer& renderer, Renderer::MemoryFile& memoryFile)
		{
			// Tell the memory mapped file about the LZ4 compressed data and decompress it at once
			std::string virtualDirectoryName;
			std::string virtualFilename;
			getPipelineStateObjectCacheFilename(renderer, virtualDirectoryName, virtualFilename);
			const Renderer::IFileManager& fileManager = renderer.getFileManager();
			if (fileManager.doesFileExist(virtualFilename.c_str()) && memoryFile.loadLz4CompressedDataByVirtualFilename(PipelineStateCache::FORMAT_TYPE, PipelineStateCache::FORMAT_VERSION, fileManager, virtualFilename.c_str()))
			{
				memoryFile.decompress();

				// Done
				return true;
			}
			
			// Failed to load the cache
			// -> No error since the cache might just not exist which is a valid situation
			return false;
		}

		void savePipelineStateObjectCacheFile(const Renderer::IRenderer& renderer, const Renderer::MemoryFile& memoryFile)
		{
			std::string virtualDirectoryName;
			std::string virtualFilename;
			getPipelineStateObjectCacheFilename(renderer, virtualDirectoryName, virtualFilename);
			Renderer::IFileManager& fileManager = renderer.getFileManager();
			if (fileManager.createDirectories(virtualDirectoryName.c_str()) && !memoryFile.writeLz4CompressedDataByVirtualFilename(PipelineStateCache::FORMAT_TYPE, PipelineStateCache::FORMAT_VERSION, fileManager, virtualFilename.c_str()))
			{
				RHI_LOG(CRITICAL, "The renderer failed to save the pipeline state object cache to \"%s\"", virtualFilename.c_str())
			}
		}


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	void RendererImpl::getDefaultTextureAssetIds(AssetIds& assetIds)
	{
		TextureResourceManager::getDefaultTextureAssetIds(assetIds);
		MaterialBlueprintResourceListener::getDefaultTextureAssetIds(assetIds);
		LightBufferManager::getDefaultTextureAssetIds(assetIds);
		#ifdef RENDERER_IMGUI
			DebugGuiManager::getDefaultTextureAssetIds(assetIds);
		#endif
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RendererImpl::RendererImpl(Context& context) :
		IRenderer(context)
	{
		// Backup the given RHI and add our reference
		mRhi = &context.getRhi();
		mRhi->AddReference();

		// Create the buffer and texture manager instance and add our reference
		mBufferManager = mRhi->createBufferManager();
		mBufferManager->AddReference();
		mTextureManager = mRhi->createTextureManager();
		mTextureManager->AddReference();

		// Backup the given file manager instance
		mFileManager = &context.getFileManager();

		// Create the core manager instances
		mDefaultThreadPool = new DefaultThreadPool();
		mAssetManager = new AssetManager(*this);
		mTimeManager = new TimeManager();

		// Create the resource manager instances
		mRendererResourceManager = new RendererResourceManager(*this);
		mResourceStreamer = new ResourceStreamer(*this);
		mVertexAttributesResourceManager = new VertexAttributesResourceManager(*this);
		mTextureResourceManager = new TextureResourceManager(*this);
		mShaderPieceResourceManager = new ShaderPieceResourceManager(*this);
		mShaderBlueprintResourceManager = new ShaderBlueprintResourceManager(*this);
		mMaterialBlueprintResourceManager = new MaterialBlueprintResourceManager(*this);
		mMaterialResourceManager = new MaterialResourceManager(*this);
		mSkeletonResourceManager = new SkeletonResourceManager(*this);
		mSkeletonAnimationResourceManager = new SkeletonAnimationResourceManager(*this);
		mMeshResourceManager = new MeshResourceManager(*this);
		mSceneResourceManager = new SceneResourceManager(*this);
		mCompositorNodeResourceManager = new CompositorNodeResourceManager(*this);
		mCompositorWorkspaceResourceManager = new CompositorWorkspaceResourceManager(*this);

		// Register the resource managers inside the resource managers list
		mResourceManagers.push_back(mVertexAttributesResourceManager);
		mResourceManagers.push_back(mTextureResourceManager);
		mResourceManagers.push_back(mShaderPieceResourceManager);
		mResourceManagers.push_back(mShaderBlueprintResourceManager);
		mResourceManagers.push_back(mMaterialBlueprintResourceManager);
		mResourceManagers.push_back(mMaterialResourceManager);
		mResourceManagers.push_back(mSkeletonResourceManager);
		mResourceManagers.push_back(mSkeletonAnimationResourceManager);
		mResourceManagers.push_back(mMeshResourceManager);
		mResourceManagers.push_back(mSceneResourceManager);
		mResourceManagers.push_back(mCompositorNodeResourceManager);
		mResourceManagers.push_back(mCompositorWorkspaceResourceManager);

		// Misc
		mGraphicsPipelineStateCompiler = new GraphicsPipelineStateCompiler(*this);
		mComputePipelineStateCompiler = new ComputePipelineStateCompiler(*this);

		// Create the optional manager instances
		#ifdef RENDERER_IMGUI
			#ifdef _WIN32
				mDebugGuiManager = new DebugGuiManagerWindows(*this);
			#elif LINUX
				mDebugGuiManager = new DebugGuiManagerLinux(*this);
			#else
				#error "Unsupported platform"
			#endif
			mDebugGuiManager->initializeImGuiKeyMap();
		#endif
		// Don't try to load the pipeline state object cache at this point in time, the asset manager will have no asset packages and hence there will be no material blueprint assets
	}

	RendererImpl::~RendererImpl()
	{
		// Before doing anything else, ensure the resource streamer has no more work to do
		flushAllQueues();

		// Save pipeline state object cache
		savePipelineStateObjectCache();

		// Destroy the optional manager instances
		#ifdef RENDERER_IMGUI
			delete mDebugGuiManager;
		#endif

		// Destroy miscellaneous
		delete mGraphicsPipelineStateCompiler;
		delete mComputePipelineStateCompiler;

		{ // Destroy the resource manager instances in reverse order
			const int numberOfResourceManagers = static_cast<int>(mResourceManagers.size());
			for (int i = numberOfResourceManagers - 1; i >= 0; --i)
			{
				delete mResourceManagers[static_cast<size_t>(i)];
			}
		}
		delete mResourceStreamer;

		// Destroy the core manager instances
		delete mTimeManager;
		delete mAssetManager;
		delete mDefaultThreadPool;

		// Release the texture and buffer manager instance
		mTextureManager->ReleaseReference();
		mBufferManager->ReleaseReference();
		delete mRendererResourceManager;

		// Release our RHI reference
		mRhi->ReleaseReference();
	}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IRenderer methods            ]
	//[-------------------------------------------------------]
	void RendererImpl::reloadResourceByAssetId(AssetId assetId)
	{
		// TODO(co) Optimization: If required later on, change this method to a "were's one, there are many"-signature (meaning passing multiple asset IDs at once)
		std::unique_lock<std::mutex> assetIdsOfResourcesToReloadMutexLock(mAssetIdsOfResourcesToReloadMutex);
		if (std::find(mAssetIdsOfResourcesToReload.begin(), mAssetIdsOfResourcesToReload.end(), assetId) == mAssetIdsOfResourcesToReload.cend())
		{
			mAssetIdsOfResourcesToReload.push_back(assetId);
		}
	}

	void RendererImpl::flushAllQueues()
	{
		mResourceStreamer->flushAllQueues();
		mGraphicsPipelineStateCompiler->flushAllQueues();
		mComputePipelineStateCompiler->flushAllQueues();
	}

	void RendererImpl::update()
	{
		// Update the time manager
		mTimeManager->update();

		{ // Handle resource reloading requests
			std::unique_lock<std::mutex> assetIdsOfResourcesToReloadMutexLock(mAssetIdsOfResourcesToReloadMutex);
			if (!mAssetIdsOfResourcesToReload.empty())
			{
				const size_t numberOfResourceManagers = mResourceManagers.size();
				for (uint32_t assetId : mAssetIdsOfResourcesToReload)
				{
					// Inform the individual resource manager instances
					for (size_t i = 0; i < numberOfResourceManagers; ++i)
					{
						mResourceManagers[i]->reloadResourceByAssetId(assetId);
					}
				}
				mAssetIdsOfResourcesToReload.clear();
			}
		}

		// Pipeline state compiler and resource streamer update
		mGraphicsPipelineStateCompiler->dispatch();
		mComputePipelineStateCompiler->dispatch();
		mResourceStreamer->dispatch();

		// Inform the individual resource manager instances
		const size_t numberOfResourceManagers = mResourceManagers.size();
		for (size_t i = 0; i < numberOfResourceManagers; ++i)
		{
			mResourceManagers[i]->update();
		}
		mRendererResourceManager->garbageCollection();
	}

	void RendererImpl::clearPipelineStateObjectCache()
	{
		mShaderBlueprintResourceManager->clearPipelineStateObjectCache();
		mMaterialBlueprintResourceManager->clearPipelineStateObjectCache();
	}

	void RendererImpl::loadPipelineStateObjectCache()
	{
		if (mRhi->getCapabilities().shaderBytecode)
		{
			clearPipelineStateObjectCache();

			// Load file
			MemoryFile memoryFile;
			if (::detail::loadPipelineStateObjectCacheFile(*this, memoryFile))
			{
				mShaderBlueprintResourceManager->loadPipelineStateObjectCache(memoryFile);
				mMaterialBlueprintResourceManager->loadPipelineStateObjectCache(memoryFile);
			}
			else
			{
				// TODO(co) As soon as everything is in place, we might want to enable this assert
				// SE_ASSERT(false, "Renderer is unable to load the pipeline state object cache. This will possibly result decreased runtime performance up to runtime hiccups. You might want to create the pipeline state object cache via the renderer toolkit.")
			}
		}
	}

	void RendererImpl::savePipelineStateObjectCache()
	{
		// Do only save the pipeline state object cache if writing local data is allowed
		// -> We only support saving material blueprint based shader bytecodes, creating shaders without material blueprint is supposed to be only used for debugging and tiny shaders which are compiled at the very beginning of rendering
		if (mRhi->getCapabilities().shaderBytecode && nullptr != mFileManager->getLocalDataMountPoint() && (mShaderBlueprintResourceManager->doesPipelineStateObjectCacheNeedSaving() || mMaterialBlueprintResourceManager->doesPipelineStateObjectCacheNeedSaving()))
		{
			MemoryFile memoryFile;
			mShaderBlueprintResourceManager->savePipelineStateObjectCache(memoryFile);
			mMaterialBlueprintResourceManager->savePipelineStateObjectCache(memoryFile);
			::detail::savePipelineStateObjectCacheFile(*this, memoryFile);
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual RefCount methods               ]
	//[-------------------------------------------------------]
	void RendererImpl::selfDestruct() noexcept
	{
		RHI_DELETE(RendererImpl, this);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
