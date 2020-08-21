#pragma once


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class IResource;
	class IResourceManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t ResourceId;	// POD resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract resource listener interface
	*/
	class IResourceListener
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class IResource;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct ResourceConnection final
		{
			IResourceManager* resourceManager;	// Owner resource manager, always valid
			ResourceId		  resourceId;		// Unique resource ID inside the resource manager
			ResourceConnection(IResourceManager* _resourceManager, ResourceId _resourceId) :
				resourceManager(_resourceManager),
				resourceId(_resourceId)
			{ }
		};
		typedef std::vector<ResourceConnection> ResourceConnections;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline const ResourceConnections& getResourceConnections() const
		{
			return mResourceConnections;
		}

		void disconnectFromResourceById(ResourceId resourceId);
		void disconnectFromAllResources();


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::IResourceListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onLoadingStateChange(const IResource& resource) = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline IResourceListener()
		{
			// Nothing here
		}

		inline virtual ~IResourceListener()
		{
			disconnectFromAllResources();
		}

		explicit IResourceListener(const IResourceListener&) = delete;
		IResourceListener& operator=(const IResourceListener&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ResourceConnections mResourceConnections;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
