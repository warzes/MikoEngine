#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]

#include "Renderer/Resource/CompositorNode/Pass/ICompositorPassFactory.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class CompositorPassFactory : public ICompositorPassFactory
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline CompositorPassFactory()
		{
			// Nothing here
		}

		inline virtual ~CompositorPassFactory() override
		{
			// Nothing here
		}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::ICompositorPassFactory methods ]
	//[-------------------------------------------------------]
	protected:
		[[nodiscard]] virtual ICompositorResourcePass* createCompositorResourcePass(const CompositorTarget& compositorTarget, CompositorPassTypeId compositorPassTypeId) const override;
		[[nodiscard]] virtual ICompositorInstancePass* createCompositorInstancePass(const ICompositorResourcePass& compositorResourcePass, const CompositorNodeInstance& compositorNodeInstance) const override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		explicit CompositorPassFactory(const CompositorPassFactory&) = delete;
		CompositorPassFactory& operator=(const CompositorPassFactory&) = delete;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
