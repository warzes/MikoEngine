#include "stdafx.h"
#include "Renderer/Resource/MaterialBlueprint/MaterialBlueprintResourceManager.h"
#include "Renderer/Resource/MaterialBlueprint/MaterialBlueprintResource.h"
#include "Renderer/Resource/MaterialBlueprint/Loader/MaterialBlueprintResourceLoader.h"
#include "Renderer/Resource/MaterialBlueprint/Listener/MaterialBlueprintResourceListener.h"
#include "Renderer/Resource/MaterialBlueprint/BufferManager/UniformInstanceBufferManager.h"
#include "Renderer/Resource/MaterialBlueprint/BufferManager/TextureInstanceBufferManager.h"
#include "Renderer/Resource/MaterialBlueprint/BufferManager/IndirectBufferManager.h"
#include "Renderer/Resource/MaterialBlueprint/BufferManager/LightBufferManager.h"
#include "Renderer/Resource/MaterialBlueprint/BufferManager/MaterialBufferManager.h"
#include "Renderer/Resource/Material/MaterialResourceManager.h"
#include "Renderer/Resource/Material/MaterialTechnique.h"
#include "Renderer/Resource/Material/MaterialResource.h"
#include "Renderer/Resource/ResourceManagerTemplate.h"
#include "Renderer/Core/Time/TimeManager.h"
#include "Renderer/Context.h"
#include "Renderer/Resource/Material/MaterialPropertyValue.h"


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
		struct MaterialBlueprintCacheEntry final
		{
			Renderer::AssetId materialBlueprintAssetId;
			uint32_t		  numberOfBytes;
		};


		//[-------------------------------------------------------]
		//[ Global variables                                      ]
		//[-------------------------------------------------------]
		static Renderer::MaterialBlueprintResourceListener defaultMaterialBlueprintResourceListener;


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
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	// TODO(co) Work-in-progress
	void MaterialBlueprintResourceManager::loadMaterialBlueprintResourceByAssetId(AssetId assetId, MaterialBlueprintResourceId& materialBlueprintResourceId, IResourceListener* resourceListener, bool reload, ResourceLoaderTypeId resourceLoaderTypeId, bool createInitialPipelineStateCaches)
	{
		// Choose default resource loader type ID, if necessary
		if (IsInvalid(resourceLoaderTypeId))
		{
			resourceLoaderTypeId = MaterialBlueprintResourceLoader::TYPE_ID;
		}

		// Get or create the instance
		MaterialBlueprintResource* materialBlueprintResource = mInternalResourceManager->getResourceByAssetId(assetId);

		// Create the resource instance
		const Asset* asset = mRenderer.getAssetManager().tryGetAssetByAssetId(assetId);
		RHI_ASSERT(nullptr != asset, "Unknown asset ID")
		bool load = (reload && nullptr != asset);
		if (nullptr == materialBlueprintResource && nullptr != asset)
		{
			materialBlueprintResource = &mInternalResourceManager->getResources().addElement();
			materialBlueprintResource->setResourceManager(this);
			materialBlueprintResource->setAssetId(assetId);
			materialBlueprintResource->setResourceLoaderTypeId(resourceLoaderTypeId);
			load = true;
		}

		// Before connecting a resource listener, ensure we set the output resource ID at once so it can already directly be used inside the resource listener
		if (nullptr != materialBlueprintResource)
		{
			materialBlueprintResourceId = materialBlueprintResource->getId();
			if (nullptr != resourceListener)
			{
				materialBlueprintResource->connectResourceListener(*resourceListener);
			}
		}
		else
		{
			materialBlueprintResourceId = GetInvalid<MaterialBlueprintResourceId>();
		}

		// Load the resource, if required
		if (load)
		{
			// Commit resource streamer asset load request
			mRenderer.getResourceStreamer().commitLoadRequest(ResourceStreamer::LoadRequest(*asset, resourceLoaderTypeId, reload, *this, materialBlueprintResourceId));

			// TODO(co) Currently material blueprint resource loading is a blocking process.
			//          Later on, we can probably just write "mInternalResourceManager->loadResourceByAssetId(assetId, meshResourceId, resourceListener, reload, resourceLoaderTypeId);" and be done in this method.
			materialBlueprintResource->enforceFullyLoaded();

			// Create default pipeline state caches
			// -> Material blueprints should be loaded by a cache manager upfront so that the following expensive call doesn't cause runtime hiccups
			// -> Runtime hiccups would also be there without fallback pipeline state caches, so there's no real way around
			// -> We must enforce fully loaded material blueprint resource state for this
			if (mCreateInitialPipelineStateCaches && createInitialPipelineStateCaches)
			{
				materialBlueprintResource->createPipelineStateCaches(true);
			}
		}
	}

	void MaterialBlueprintResourceManager::setInvalidResourceId(MaterialBlueprintResourceId& materialBlueprintResourceId, IResourceListener& resourceListener) const
	{
		MaterialBlueprintResource* materialBlueprintResource = tryGetById(materialBlueprintResourceId);
		if (nullptr != materialBlueprintResource)
		{
			materialBlueprintResource->disconnectResourceListener(resourceListener);
		}
		SetInvalid(materialBlueprintResourceId);
	}

	void MaterialBlueprintResourceManager::setMaterialBlueprintResourceListener(IMaterialBlueprintResourceListener* materialBlueprintResourceListener)
	{
		// There must always be a valid material blueprint resource listener instance
		if (mMaterialBlueprintResourceListener != materialBlueprintResourceListener)
		{
			// We know there must be a currently set material blueprint resource listener
			RHI_ASSERT(nullptr != mMaterialBlueprintResourceListener, "Invalid material blueprint resource listener")
			mMaterialBlueprintResourceListener->onShutdown(mRenderer);
			mMaterialBlueprintResourceListener = (nullptr != materialBlueprintResourceListener) ? materialBlueprintResourceListener : &::detail::defaultMaterialBlueprintResourceListener;
			mMaterialBlueprintResourceListener->onStartup(mRenderer);
		}
	}

	void MaterialBlueprintResourceManager::onPreCommandBufferExecution()
	{
		if (nullptr != mUniformInstanceBufferManager)
		{
			mUniformInstanceBufferManager->onPreCommandBufferExecution();
		}
		if (nullptr != mTextureInstanceBufferManager)
		{
			mTextureInstanceBufferManager->onPreCommandBufferExecution();
		}
		if (nullptr != mIndirectBufferManager)
		{
			mIndirectBufferManager->onPreCommandBufferExecution();
		}
	}

	void MaterialBlueprintResourceManager::setDefaultTextureFiltering(Rhi::FilterMode filterMode, uint8_t maximumAnisotropy)
	{
		// State change?
		if (mDefaultTextureFilterMode != filterMode || mDefaultMaximumTextureAnisotropy != maximumAnisotropy)
		{
			// Backup the new state
			mDefaultTextureFilterMode = filterMode;
			mDefaultMaximumTextureAnisotropy = maximumAnisotropy;

			{ // Recreate sampler state instances of all material blueprint resources
				const uint32_t numberOfElements = mInternalResourceManager->getResources().getNumberOfElements();
				for (uint32_t i = 0; i < numberOfElements; ++i)
				{
					mInternalResourceManager->getResources().getElementByIndex(i).onDefaultTextureFilteringChanged(mDefaultTextureFilterMode, mDefaultMaximumTextureAnisotropy);
				}
			}

			{ // Make the resource groups of all material techniques dirty to instantly see default texture filtering changes
				const MaterialResourceManager& materialResourceManager = mRenderer.getMaterialResourceManager();
				const uint32_t numberOfElements = materialResourceManager.getNumberOfResources();
				for (uint32_t i = 0; i < numberOfElements; ++i)
				{
					for (MaterialTechnique* materialTechnique : materialResourceManager.getByIndex(i).getSortedMaterialTechniqueVector())
					{
						materialTechnique->makeResourceGroupDirty();
					}
				}
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceManager methods     ]
	//[-------------------------------------------------------]
	uint32_t MaterialBlueprintResourceManager::getNumberOfResources() const
	{
		return mInternalResourceManager->getResources().getNumberOfElements();
	}

	IResource& MaterialBlueprintResourceManager::getResourceByIndex(uint32_t index) const
	{
		return mInternalResourceManager->getResources().getElementByIndex(index);
	}

	IResource& MaterialBlueprintResourceManager::getResourceByResourceId(ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().getElementById(resourceId);
	}

	IResource* MaterialBlueprintResourceManager::tryGetResourceByResourceId(ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().tryGetElementById(resourceId);
	}

	void MaterialBlueprintResourceManager::reloadResourceByAssetId(AssetId assetId)
	{
		// TODO(co) Experimental implementation (take care of resource cleanup etc.)
		const uint32_t numberOfElements = mInternalResourceManager->getResources().getNumberOfElements();
		for (uint32_t i = 0; i < numberOfElements; ++i)
		{
			MaterialBlueprintResource& materialBlueprintResource = mInternalResourceManager->getResources().getElementByIndex(i);
			if (materialBlueprintResource.getAssetId() == assetId)
			{
				{ // Properly release material buffer slots
					MaterialBufferManager* materialBufferManager = materialBlueprintResource.mMaterialBufferManager;
					const MaterialResourceManager& materialResourceManager = mRenderer.getMaterialResourceManager();
					const uint32_t numberOfMaterials = materialResourceManager.getNumberOfResources();
					for (uint32_t materialIndex = 0; materialIndex < numberOfMaterials; ++materialIndex)
					{
						for (MaterialTechnique* materialTechnique : materialResourceManager.getByIndex(materialIndex).getSortedMaterialTechniqueVector())
						{
							if (materialTechnique->getMaterialBlueprintResourceId() == materialBlueprintResource.getId() && IsValid(materialTechnique->getAssignedMaterialSlot()))
							{
								materialBufferManager->releaseSlot(*materialTechnique);
							}
						}
					}
				}

				// Reload material blueprint resource
				MaterialBlueprintResourceId materialBlueprintResourceId = GetInvalid<MaterialBlueprintResourceId>();
				loadMaterialBlueprintResourceByAssetId(assetId, materialBlueprintResourceId, nullptr, true, materialBlueprintResource.getResourceLoaderTypeId());

				// Clear pipeline state cache manager
				materialBlueprintResource.clearPipelineStateObjectCache();

				{ // Make the texture resource groups of all material techniques
					MaterialBufferManager* materialBufferManager = materialBlueprintResource.mMaterialBufferManager;
					const MaterialResourceManager& materialResourceManager = mRenderer.getMaterialResourceManager();
					const uint32_t numberOfMaterials = materialResourceManager.getNumberOfResources();

					// Loop through all materials
					for (uint32_t materialIndex = 0; materialIndex < numberOfMaterials; ++materialIndex)
					{
						// Loop through all material techniques of the current material
						MaterialResource& materialResource = materialResourceManager.getByIndex(materialIndex);
						for (MaterialTechnique* materialTechnique : materialResource.getSortedMaterialTechniqueVector())
						{
							if (materialTechnique->getMaterialBlueprintResourceId() == materialBlueprintResource.getId())
							{
								materialTechnique->makeResourceGroupDirty();
								for (const MaterialProperty& materialProperty : materialResource.getSortedPropertyVector())
								{
									// Update material property values as long as a material property was not explicitly overwritten inside a material
									if (!materialProperty.isOverwritten())
									{
										const MaterialPropertyId materialPropertyId = materialProperty.getMaterialPropertyId();
										const MaterialProperty* blueprintMaterialProperty = materialBlueprintResource.getMaterialProperties().getPropertyById(materialPropertyId);
										if (nullptr != blueprintMaterialProperty)
										{
											materialResource.setPropertyByIdInternal(materialPropertyId, *blueprintMaterialProperty, materialProperty.getUsage(), false);
										}
									}
								}
								if (nullptr != materialBufferManager)
								{
									materialBufferManager->requestSlot(*materialTechnique);
								}
								materialTechnique->calculateSerializedGraphicsPipelineStateHash();
							}
						}
					}
				}
				break;
			}
		}
	}

	void MaterialBlueprintResourceManager::update()
	{
		const TimeManager& timeManager = mRenderer.getTimeManager();
		mGlobalMaterialProperties.setPropertyById(STRING_ID("GlobalPastSecondsSinceLastFrame"), MaterialPropertyValue::fromFloat(timeManager.getPastSecondsSinceLastFrame()), MaterialProperty::Usage::SHADER_UNIFORM);
		{ // Set previous global time in seconds
			// TODO: хрень:
			const MaterialProperty* materialProperty = mGlobalMaterialProperties.getPropertyById(STRING_ID("GlobalTimeInSeconds"));
			const MaterialProperty* materialProperty2 = (const MaterialProperty*)&MaterialPropertyValue::fromFloat(timeManager.getGlobalTimeInSeconds());
			
			
			mGlobalMaterialProperties.setPropertyById(
				STRING_ID("PreviousGlobalTimeInSeconds"), 
				(nullptr != materialProperty) ? *materialProperty : *materialProperty2,
				MaterialProperty::Usage::SHADER_UNIFORM);
		}
		mGlobalMaterialProperties.setPropertyById(STRING_ID("GlobalTimeInSeconds"), MaterialPropertyValue::fromFloat(timeManager.getGlobalTimeInSeconds()), MaterialProperty::Usage::SHADER_UNIFORM);
	}


	//[-------------------------------------------------------]
	//[ Private virtual Renderer::IResourceManager methods    ]
	//[-------------------------------------------------------]
	IResourceLoader* MaterialBlueprintResourceManager::createResourceLoaderInstance(ResourceLoaderTypeId resourceLoaderTypeId)
	{
		return mInternalResourceManager->createResourceLoaderInstance(resourceLoaderTypeId);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	MaterialBlueprintResourceManager::MaterialBlueprintResourceManager(IRenderer& renderer) :
		mRenderer(renderer),
		mCreateInitialPipelineStateCaches(renderer.getRhi().getNameId() != Rhi::NameId::OPENGLES3),	// TODO(co) Not all example material blueprints are OpenGL ES 3 ready, yet
		mMaterialBlueprintResourceListener(&::detail::defaultMaterialBlueprintResourceListener),
		mDefaultTextureFilterMode(Rhi::FilterMode::MIN_MAG_MIP_LINEAR),
		mDefaultMaximumTextureAnisotropy(1),
		mUniformInstanceBufferManager(nullptr),
		mTextureInstanceBufferManager(nullptr),
		mIndirectBufferManager(nullptr),
		mLightBufferManager(nullptr)
	{
		// Create internal resource manager
		mInternalResourceManager = new ResourceManagerTemplate<MaterialBlueprintResource, MaterialBlueprintResourceLoader, MaterialBlueprintResourceId, 64>(renderer, *this);

		// Startup material blueprint resource listener
		mMaterialBlueprintResourceListener->onStartup(mRenderer);

		// Create buffer managers
		const Rhi::Capabilities& capabilities = renderer.getRhi().getCapabilities();
		if (capabilities.maximumUniformBufferSize > 0 && capabilities.maximumTextureBufferSize > 0)
		{
			mUniformInstanceBufferManager = new UniformInstanceBufferManager(renderer);
			mTextureInstanceBufferManager = new TextureInstanceBufferManager(renderer);
			mIndirectBufferManager = new IndirectBufferManager(renderer);
			mLightBufferManager = new LightBufferManager(renderer);
		}

		// Update at once to have all managed global material properties known from the start
		update();
		mGlobalMaterialProperties.setPropertyById(STRING_ID("GlobalNumberOfMultisamples"), MaterialPropertyValue::fromInteger(0), MaterialProperty::Usage::SHADER_COMBINATION);
	}

	MaterialBlueprintResourceManager::~MaterialBlueprintResourceManager()
	{
		// Destroy buffer managers
		delete mUniformInstanceBufferManager;
		delete mTextureInstanceBufferManager;
		delete mIndirectBufferManager;
		delete mLightBufferManager;

		// Shutdown material blueprint resource listener (we know there must be such an instance)
		RHI_ASSERT(nullptr != mMaterialBlueprintResourceListener, "Invalid material blueprint resource listener")
		mMaterialBlueprintResourceListener->onShutdown(mRenderer);

		// Destroy internal resource manager
		delete mInternalResourceManager;

		// Explicitly clear the default material blueprint resource listener in order to avoid false-positive "_CrtMemDumpAllObjectsSince()" memory leak detection
		::detail::defaultMaterialBlueprintResourceListener.clear();
	}

	void MaterialBlueprintResourceManager::addSerializedGraphicsPipelineState(uint32_t serializedGraphicsPipelineStateHash, const Rhi::SerializedGraphicsPipelineState& serializedGraphicsPipelineState)
	{
		std::lock_guard<std::mutex> serializedGraphicsPipelineStatesMutexLock(mSerializedGraphicsPipelineStatesMutex);
		SerializedGraphicsPipelineStates::iterator iterator = mSerializedGraphicsPipelineStates.find(serializedGraphicsPipelineStateHash);
		if (iterator == mSerializedGraphicsPipelineStates.cend())
		{
			mSerializedGraphicsPipelineStates.emplace(serializedGraphicsPipelineStateHash, serializedGraphicsPipelineState);
		}
	}

	void MaterialBlueprintResourceManager::applySerializedGraphicsPipelineState(uint32_t serializedGraphicsPipelineStateHash, Rhi::GraphicsPipelineState& graphicsPipelineState)
	{
		std::lock_guard<std::mutex> serializedGraphicsPipelineStatesMutexLock(mSerializedGraphicsPipelineStatesMutex);
		MaterialBlueprintResourceManager::SerializedGraphicsPipelineStates::const_iterator iterator = mSerializedGraphicsPipelineStates.find(serializedGraphicsPipelineStateHash);
		if (iterator != mSerializedGraphicsPipelineStates.cend())
		{
			static_cast<Rhi::SerializedGraphicsPipelineState&>(graphicsPipelineState) = iterator->second;
		}
		else
		{
			// We can and will end up in here when e.g. heating the shader cache
		}
	}

	void MaterialBlueprintResourceManager::clearPipelineStateObjectCache()
	{
		// Loop through all material blueprint resources and clear the cache entries
		const uint32_t numberOfElements = mInternalResourceManager->getResources().getNumberOfElements();
		for (uint32_t i = 0; i < numberOfElements; ++i)
		{
			mInternalResourceManager->getResources().getElementByIndex(i).clearPipelineStateObjectCache();
		}
	}

	void MaterialBlueprintResourceManager::loadPipelineStateObjectCache(IFile& file)
	{
		{ // Read the serialized graphics pipeline states
			uint32_t numberOfElements = GetInvalid<uint32_t>();
			file.read(&numberOfElements, sizeof(uint32_t));
			mSerializedGraphicsPipelineStates.reserve(numberOfElements);
			for (uint32_t i = 0; i < numberOfElements; ++i)
			{
				uint32_t key = GetInvalid<uint32_t>();
				file.read(&key, sizeof(uint32_t));
				Rhi::SerializedGraphicsPipelineState serializedGraphicsPipelineState = {};
				file.read(&serializedGraphicsPipelineState, sizeof(Rhi::SerializedGraphicsPipelineState));
				mSerializedGraphicsPipelineStates.emplace(key, serializedGraphicsPipelineState);
			}
		}

		{ // Read the pipeline state object cache header which consists of information about the contained material blueprint resources
			uint32_t numberOfElements = 0;
			file.read(&numberOfElements, sizeof(uint32_t));
			if (numberOfElements > 0)
			{
				std::vector< ::detail::MaterialBlueprintCacheEntry> materialBlueprintCacheEntries;
				materialBlueprintCacheEntries.resize(numberOfElements);
				file.read(materialBlueprintCacheEntries.data(), sizeof(::detail::MaterialBlueprintCacheEntry) * numberOfElements);

				// Loop through all material blueprint resources and read the cache entries
				for (uint32_t i = 0; i < numberOfElements; ++i)
				{
					// TODO(co) Currently material blueprint resource loading is a blocking process
					const ::detail::MaterialBlueprintCacheEntry& materialBlueprintCacheEntry = materialBlueprintCacheEntries[i];
					MaterialBlueprintResourceId materialBlueprintResourceId = GetInvalid<MaterialBlueprintResourceId>();
					loadMaterialBlueprintResourceByAssetId(materialBlueprintCacheEntry.materialBlueprintAssetId, materialBlueprintResourceId, nullptr, false, GetInvalid<ResourceLoaderTypeId>(), false);
					if (IsValid(materialBlueprintResourceId))
					{
						mInternalResourceManager->getResources().getElementById(materialBlueprintResourceId).loadPipelineStateObjectCache(file);
					}
					else
					{
						RHI_LOG(COMPATIBILITY_WARNING, "The pipeline state object cache contains an unknown material blueprint asset. Might have happened due to renaming or removal which can be considered normal during development, but not in shipped builds.")

						// TODO(co) Enable file skip after "Renderer::MaterialBlueprintResource::savePipelineStateObjectCache()" has been implemented
						// file.skip(materialBlueprintCacheEntry.numberOfBytes);
					}
				}
			}
			else
			{
				RHI_LOG(WARNING, "The pipeline state object cache contains no elements which is a bit unusual")
			}
		}
	}

	bool MaterialBlueprintResourceManager::doesPipelineStateObjectCacheNeedSaving() const
	{
		const uint32_t numberOfElements = mInternalResourceManager->getResources().getNumberOfElements();
		for (uint32_t i = 0; i < numberOfElements; ++i)
		{
			if (mInternalResourceManager->getResources().getElementByIndex(i).doesPipelineStateObjectCacheNeedSaving())
			{
				// Cache saving needed
				return true;
			}
		}

		// No cache saving needed
		return false;
	}

	void MaterialBlueprintResourceManager::savePipelineStateObjectCache(MemoryFile& memoryFile)
	{
		{ // Write the serialized graphics pipeline states
			const uint32_t numberOfElements = static_cast<uint32_t>(mSerializedGraphicsPipelineStates.size());
			memoryFile.write(&numberOfElements, sizeof(uint32_t));
			for (const auto& elementPair : mSerializedGraphicsPipelineStates)
			{
				memoryFile.write(&elementPair.first, sizeof(uint32_t));
				memoryFile.write(&elementPair.second, sizeof(Rhi::SerializedGraphicsPipelineState));
			}
		}

		{ // Write the pipeline state object cache header which consists of information about the contained material blueprint resources
			const uint32_t numberOfElements = mInternalResourceManager->getResources().getNumberOfElements();
			memoryFile.write(&numberOfElements, sizeof(uint32_t));
			uint32_t firstMaterialBlueprintCacheEntryIndex = 0;
			for (uint32_t i = 0; i < numberOfElements; ++i)
			{
				::detail::MaterialBlueprintCacheEntry materialBlueprintCacheEntry;
				materialBlueprintCacheEntry.materialBlueprintAssetId = mInternalResourceManager->getResources().getElementByIndex(i).getAssetId();
				materialBlueprintCacheEntry.numberOfBytes			 = 0;	// At this point in time we don't know the number of bytes the material blueprint cache entry consumes
				memoryFile.write(&materialBlueprintCacheEntry, sizeof(::detail::MaterialBlueprintCacheEntry));
				if (0 == firstMaterialBlueprintCacheEntryIndex)
				{
					firstMaterialBlueprintCacheEntryIndex = static_cast<uint32_t>(memoryFile.getNumberOfBytes() - sizeof(::detail::MaterialBlueprintCacheEntry));
				}
			}
			::detail::MaterialBlueprintCacheEntry* firstMaterialBlueprintCacheEntry = reinterpret_cast< ::detail::MaterialBlueprintCacheEntry*>(&memoryFile.getByteVector()[firstMaterialBlueprintCacheEntryIndex]);

			// Loop through all material blueprint resources and write the cache entries
			for (uint32_t i = 0; i < numberOfElements; ++i)
			{
				const uint32_t fileStart = static_cast<uint32_t>(memoryFile.getNumberOfBytes());
				mInternalResourceManager->getResources().getElementByIndex(i).savePipelineStateObjectCache(memoryFile);
				firstMaterialBlueprintCacheEntry[i].numberOfBytes = static_cast<uint32_t>(memoryFile.getNumberOfBytes() - fileStart);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
