#pragma once

namespace Rhi
{
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
		inline virtual ~IResourceGroup() override
		{
#if SE_RHI_STATISTICS
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
#if SE_RHI_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedResourceGroups;
			++rhi.getStatistics().currentNumberOfResourceGroups;
#endif
		}

		explicit IResourceGroup(const IResourceGroup&) = delete;
		IResourceGroup& operator=(const IResourceGroup&) = delete;
	};

	typedef SmartRefCount<IResourceGroup> IResourceGroupPtr;

} // namespace Rhi