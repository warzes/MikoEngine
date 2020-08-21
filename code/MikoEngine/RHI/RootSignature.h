#pragma once

namespace OpenGLRhi
{
	//[-------------------------------------------------------]
//[ OpenGLRhi/RootSignature.h                             ]
//[-------------------------------------------------------]
/**
*  @brief
*    OpenGL root signature ("pipeline layout" in Vulkan terminology) class
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
		*  @param[in] openGLRhi
		*    Owner OpenGL RHI instance
		*  @param[in] rootSignature
		*    Root signature to use
		*/
		RootSignature(OpenGLRhi& openGLRhi, const Rhi::RootSignature& rootSignature RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IRootSignature(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
			mRootSignature(rootSignature)
		{
			const Rhi::Context& context = openGLRhi.getContext();

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

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RootSignature() override
		{
			// Destroy the root signature data
			const Rhi::Context& context = getRhi().getContext();
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


		//[-------------------------------------------------------]
		//[ Public virtual Rhi::IRootSignature methods            ]
		//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual Rhi::IResourceGroup* createResourceGroup(uint32_t rootParameterIndex, uint32_t numberOfResources, Rhi::IResource** resources, Rhi::ISamplerState** samplerStates = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override
		{
			Rhi::IRhi& rhi = getRhi();

			// Sanity checks
			RHI_ASSERT(rootParameterIndex < mRootSignature.numberOfParameters, "The OpenGL root parameter index is out-of-bounds")
				RHI_ASSERT(numberOfResources > 0, "The number of OpenGL resources must not be zero")
				RHI_ASSERT(nullptr != resources, "The OpenGL resource pointers must be valid")

				// Create resource group
				return RHI_NEW(ResourceGroup)(rhi, mRootSignature, rootParameterIndex, numberOfResources, resources, samplerStates RHI_RESOURCE_DEBUG_PASS_PARAMETER);
		}


		//[-------------------------------------------------------]
		//[ Protected virtual RefCount methods               ]
		//[-------------------------------------------------------]
	protected:
		inline virtual void selfDestruct() override
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
		Rhi::RootSignature mRootSignature;


	};
} // namespace OpenGLRhi