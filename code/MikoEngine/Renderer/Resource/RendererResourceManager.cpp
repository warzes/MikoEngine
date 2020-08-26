#include "stdafx.h"
#include "Renderer/Resource/RendererResourceManager.h"
#include "Math/Math.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	Rhi::IResourceGroup* RendererResourceManager::createResourceGroup(Rhi::IRootSignature& rootSignature, uint32_t rootParameterIndex, uint32_t numberOfResources, Rhi::IResource** resources, Rhi::ISamplerState** samplerStates RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		// Create hash
		uint32_t hash = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&rootSignature), sizeof(Rhi::IRootSignature&));
		hash = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&rootParameterIndex), sizeof(uint32_t), hash);
		hash = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&numberOfResources), sizeof(uint32_t), hash);
		for (uint32_t i = 0; i < numberOfResources; ++i)
		{
			hash = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&resources[i]), sizeof(Rhi::IResource*), hash);
			if (nullptr != samplerStates && nullptr != samplerStates[i])
			{
				hash = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&samplerStates[i]), sizeof(Rhi::ISamplerState*), hash);
			}
			else
			{
				static const uint32_t NOTHING = 42;	// Not "static constexpr" by intent
				hash = Math::calculateFNV1a32(reinterpret_cast<const uint8_t*>(&NOTHING), sizeof(uint32_t), hash);
			}
		}
		ResourceGroups::const_iterator iterator = mResourceGroups.find(hash);
		if (mResourceGroups.cend() != iterator)
		{
			return iterator->second;
		}
		else
		{
			// Create RHI resource and add the managers reference
			Rhi::IResourceGroup* resourceGroup = rootSignature.createResourceGroup(rootParameterIndex, numberOfResources, resources, samplerStates RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			resourceGroup->AddReference();
			mResourceGroups.emplace(hash, resourceGroup);
			return resourceGroup;
		}
	}

	void RendererResourceManager::garbageCollection()
	{
		// TODO(co) "Renderer::RendererResourceManager": From time to time, look for orphaned RHI resources and free them. Currently a trivial approach is used which might cause hiccups. For example distribute the traversal over time.
		++mGarbageCollectionCounter;
		if (mGarbageCollectionCounter > 100)
		{
			ResourceGroups::iterator iterator = mResourceGroups.begin();
			while (iterator != mResourceGroups.end())
			{
				if (iterator->second->GetRefCount() == 1)
				{
					iterator->second->ReleaseReference();
					iterator = mResourceGroups.erase(iterator);
				}
				else
				{
					++iterator;
				}
			}
			mGarbageCollectionCounter = 0;
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	RendererResourceManager::~RendererResourceManager()
	{
		// Release manager RHI resource references
		for (auto& pair : mResourceGroups)
		{
			pair.second->ReleaseReference();
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
