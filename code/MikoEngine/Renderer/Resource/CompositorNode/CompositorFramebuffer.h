#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Utility/StringId.h"
#include "Renderer/Core/Renderer/FramebufferSignature.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringId CompositorFramebufferId;	// Compositor framebuffer identifier, internally just a POD "uint32_t"


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Compositor framebuffer; used for compositor workspace and nodes intermediate rendering results
	*/
	class CompositorFramebuffer final
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline CompositorFramebuffer(CompositorFramebufferId compositorFramebufferId, const FramebufferSignature& framebufferSignature) :
			mCompositorFramebufferId(compositorFramebufferId),
			mFramebufferSignature(framebufferSignature)
		{
			// Nothing here
		}

		inline explicit CompositorFramebuffer(const CompositorFramebuffer& compositorFramebuffer) :
			mCompositorFramebufferId(compositorFramebuffer.mCompositorFramebufferId),
			mFramebufferSignature(compositorFramebuffer.mFramebufferSignature)
		{
			// Nothing here
		}

		inline ~CompositorFramebuffer()
		{
			// Nothing here
		}

		inline CompositorFramebuffer& operator=(const CompositorFramebuffer& compositorFramebuffer)
		{
			mCompositorFramebufferId = compositorFramebuffer.mCompositorFramebufferId;
			mFramebufferSignature	 = compositorFramebuffer.mFramebufferSignature;
			return *this;
		}

		[[nodiscard]] inline CompositorFramebufferId getCompositorFramebufferId() const
		{
			return mCompositorFramebufferId;
		}

		[[nodiscard]] inline const FramebufferSignature& getFramebufferSignature() const
		{
			return mFramebufferSignature;
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		CompositorFramebufferId mCompositorFramebufferId;
		FramebufferSignature	mFramebufferSignature;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
