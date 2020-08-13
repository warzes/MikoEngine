#pragma once

namespace Rhi
{
	class IQueryPool : public IResource
	{
	public:
		inline virtual ~IQueryPool() override
		{
#if SE_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfQueryPools;
#endif
		}

		// Protected methods
	protected:
		inline explicit IQueryPool(IRhi& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IResource(ResourceType::QUERY_POOL, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#if SE_STATISTICS
			// Update the statistics
			++rhi.getStatistics().numberOfCreatedQueryPools;
			++rhi.getStatistics().currentNumberOfQueryPools;
#endif
		}

		explicit IQueryPool(const IQueryPool&) = delete;
		IQueryPool& operator =(const IQueryPool&) = delete;

	};

	typedef SmartRefCount<IQueryPool> IQueryPoolPtr;
} // namespace Rhi