#pragma once

namespace Rhi
{
	/**
	*  @brief
	*    Abstract root signature ("pipeline layout" in Vulkan terminology) interface
	*
	*  @note
	*    - Overview of the binding models of explicit APIs: "Choosing a binding model" - https://github.com/gpuweb/gpuweb/issues/19
	*/
	class IRootSignature : public IResource
	{
	public:
		inline virtual ~IRootSignature() override
		{
#if SE_STATISTICS
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
		inline explicit IRootSignature(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(ResourceType::ROOT_SIGNATURE, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#if SE_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedRootSignatures;
			++rhi.getStatistics().currentNumberOfRootSignatures;
#endif
		}

		explicit IRootSignature(const IRootSignature&) = delete;
		IRootSignature& operator=(const IRootSignature&) = delete;

	};

	typedef SmartRefCount<IRootSignature> IRootSignaturePtr;

} // namespace Rhi