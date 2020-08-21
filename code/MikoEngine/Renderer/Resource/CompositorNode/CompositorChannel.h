#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Compositor node resource channel; used for data transport between compositor workspace and nodes
	*
	*  @note
	*    - Named in short just compositor channel because there's no instance variant
	*/
	class CompositorChannel final
	{


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline CompositorChannel()
		{
			// Nothing here
		}

		inline ~CompositorChannel()
		{
			// Nothing here
		}

		explicit CompositorChannel(const CompositorChannel&) = delete;
		CompositorChannel& operator=(const CompositorChannel&) = delete;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
