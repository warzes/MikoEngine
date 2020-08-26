#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/CompositorNode/Pass/Compute/CompositorResourcePassCompute.h"


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
	*    Compositor resource pass debug GUI
	*
	*  @note
	*    - It's highly recommended to only render the debug GUI into the main render target, for example the swap chain of an operation system window
	*/
	class CompositorResourcePassDebugGui final : public CompositorResourcePassCompute
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CompositorPassFactory;	// The only one allowed to create instances of this class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t TYPE_ID = SE_STRING_ID("DebugGui");


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::ICompositorResourcePass methods ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual CompositorPassTypeId getTypeId() const override
		{
			return TYPE_ID;
		}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline explicit CompositorResourcePassDebugGui(const CompositorTarget& compositorTarget) :
			CompositorResourcePassCompute(compositorTarget, false)
		{
			// Nothing here
		}

		inline virtual ~CompositorResourcePassDebugGui() override
		{
			// Nothing here
		}

		explicit CompositorResourcePassDebugGui(const CompositorResourcePassDebugGui&) = delete;
		CompositorResourcePassDebugGui& operator=(const CompositorResourcePassDebugGui&) = delete;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
