#pragma once

namespace OpenGLRhi
{
	//[-------------------------------------------------------]
//[ OpenGLRhi/ResourceGroup.h                             ]
//[-------------------------------------------------------]
/**
*  @brief
*    OpenGL resource group class
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
		*  @param[in] rhi
		*    Owner RHI instance
		*  @param[in] rootSignature
		*    Root signature
		*  @param[in] rootParameterIndex
		*    The root parameter index number for binding
		*  @param[in] numberOfResources
		*    Number of resources, having no resources is invalid
		*  @param[in] resources
		*    At least "numberOfResources" resource pointers, must be valid, the resource group will keep a reference to the resources
		*  @param[in] samplerStates
		*    If not a null pointer at least "numberOfResources" sampler state pointers, must be valid if there's at least one texture resource, the resource group will keep a reference to the sampler states
		*/
		ResourceGroup(Rhi::IRhi& rhi, const Rhi::RootSignature& rootSignature, uint32_t rootParameterIndex, uint32_t numberOfResources, Rhi::IResource** resources, Rhi::ISamplerState** samplerStates RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResourceGroup(rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mRootParameterIndex(rootParameterIndex),
			mNumberOfResources(numberOfResources),
			mResources(RHI_MALLOC_TYPED(Rhi::IResource*, mNumberOfResources)),
			mSamplerStates(nullptr),
			mResourceIndexToUniformBlockBindingIndex(nullptr)
		{
			// Get the uniform block binding start index
			const Rhi::Context& context = rhi.getContext();
			uint32_t uniformBlockBindingIndex = 0;
			for ( uint32_t currentRootParameterIndex = 0; currentRootParameterIndex < rootParameterIndex; ++currentRootParameterIndex )
			{
				const Rhi::RootParameter& rootParameter = rootSignature.parameters[currentRootParameterIndex];
				if ( Rhi::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType )
				{
					RHI_ASSERT(nullptr != reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "Invalid OpenGL descriptor ranges")
						const uint32_t numberOfDescriptorRanges = rootParameter.descriptorTable.numberOfDescriptorRanges;
					for ( uint32_t descriptorRangeIndex = 0; descriptorRangeIndex < numberOfDescriptorRanges; ++descriptorRangeIndex )
					{
						if ( Rhi::DescriptorRangeType::UBV == reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges)[descriptorRangeIndex].rangeType )
						{
							++uniformBlockBindingIndex;
						}
					}
				}
			}

			// Process all resources and add our reference to the RHI resource
			const Rhi::RootParameter& rootParameter = rootSignature.parameters[rootParameterIndex];
			for ( uint32_t resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex, ++resources )
			{
				Rhi::IResource* resource = *resources;
				RHI_ASSERT(nullptr != resource, "Invalid OpenGL resource")
					mResources[resourceIndex] = resource;
				resource->addReference();

				// Uniform block binding index handling
				const Rhi::DescriptorRange& descriptorRange = reinterpret_cast<const Rhi::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges)[resourceIndex];
				if ( Rhi::DescriptorRangeType::UBV == descriptorRange.rangeType )
				{
					if ( nullptr == mResourceIndexToUniformBlockBindingIndex )
					{
						mResourceIndexToUniformBlockBindingIndex = RHI_MALLOC_TYPED(uint32_t, mNumberOfResources);
						memset(mResourceIndexToUniformBlockBindingIndex, 0, sizeof(uint32_t) * mNumberOfResources);
					}
					mResourceIndexToUniformBlockBindingIndex[resourceIndex] = uniformBlockBindingIndex;
					++uniformBlockBindingIndex;
				}
			}
			if ( nullptr != samplerStates )
			{
				mSamplerStates = RHI_MALLOC_TYPED(Rhi::ISamplerState*, mNumberOfResources);
				for ( uint32_t resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex )
				{
					Rhi::ISamplerState* samplerState = mSamplerStates[resourceIndex] = samplerStates[resourceIndex];
					if ( nullptr != samplerState )
					{
						samplerState->addReference();
					}
				}
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ResourceGroup() override
		{
			// Remove our reference from the RHI resources
			const Rhi::Context& context = getRhi().getContext();
			if ( nullptr != mSamplerStates )
			{
				for ( uint32_t resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex )
				{
					Rhi::ISamplerState* samplerState = mSamplerStates[resourceIndex];
					if ( nullptr != samplerState )
					{
						samplerState->releaseReference();
					}
				}
				RHI_FREE(mSamplerStates);
			}
			for ( uint32_t resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex )
			{
				mResources[resourceIndex]->releaseReference();
			}
			RHI_FREE(mResources);
			RHI_FREE(mResourceIndexToUniformBlockBindingIndex);
		}

		/**
		*  @brief
		*    Return the number of resources this resource group groups together
		*
		*  @return
		*    The number of resources this resource group groups together
		*/
		[[nodiscard]] inline uint32_t getNumberOfResources() const
		{
			return mNumberOfResources;
		}

		/**
		*  @brief
		*    Return the RHI resources
		*
		*  @return
		*    The RHI resources, don't release or destroy the returned pointer
		*/
		[[nodiscard]] inline Rhi::IResource** getResources() const
		{
			return mResources;
		}

		/**
		*  @brief
		*    Return the sampler states
		*
		*  @return
		*    The sampler states, don't release or destroy the returned pointer
		*/
		[[nodiscard]] inline Rhi::ISamplerState** getSamplerState() const
		{
			return mSamplerStates;
		}

		/**
		*  @brief
		*    Return the resource index to uniform block binding index mapping
		*
		*  @return
		*    The resource index to uniform block binding index mapping, only valid for uniform buffer resources
		*/
		[[nodiscard]] inline uint32_t* getResourceIndexToUniformBlockBindingIndex() const
		{
			return mResourceIndexToUniformBlockBindingIndex;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
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
		uint32_t			 mRootParameterIndex;						// The root parameter index number for binding
		uint32_t			 mNumberOfResources;						// Number of resources this resource group groups together
		Rhi::IResource**	 mResources;								// RHI resources, we keep a reference to it
		Rhi::ISamplerState** mSamplerStates;							// Sampler states, we keep a reference to it
		uint32_t*			 mResourceIndexToUniformBlockBindingIndex;	// Resource index to uniform block binding index mapping, only valid for uniform buffer resources


	};
} // namespace OpenGLRhi