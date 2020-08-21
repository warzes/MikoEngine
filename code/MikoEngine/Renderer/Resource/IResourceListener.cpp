#include "stdafx.h"
#include "Renderer/Resource/IResourceListener.h"
#include "Renderer/Resource/IResourceManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void IResourceListener::disconnectFromResourceById(ResourceId resourceId)
	{
		RHI_ASSERT(isValid(resourceId), "Invalid resource ID")
		ResourceConnections resourceConnectionsToDisconnect;
		resourceConnectionsToDisconnect.reserve(mResourceConnections.size());
		for (const ResourceConnection& resourceConnection : mResourceConnections)
		{
			if (resourceConnection.resourceId == resourceId)
			{
				resourceConnectionsToDisconnect.push_back(resourceConnection);
			}
		}
		for (const ResourceConnection& resourceConnection : resourceConnectionsToDisconnect)
		{
			resourceConnection.resourceManager->getResourceByResourceId(resourceConnection.resourceId).disconnectResourceListener(*this);
		}
	}

	void IResourceListener::disconnectFromAllResources()
	{
		// Disconnect all resource connections
		while (!mResourceConnections.empty())
		{
			const ResourceConnection& resourceConnection = mResourceConnections[0];
			resourceConnection.resourceManager->getResourceByResourceId(resourceConnection.resourceId).disconnectResourceListener(*this);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
