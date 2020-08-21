#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/CompositorNode/Pass/ICompositorResourcePass.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class CompositorResourcePassVrHiddenAreaMesh final : public ICompositorResourcePass
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CompositorPassFactory;	// The only one allowed to create instances of this class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t TYPE_ID = STRING_ID("VrHiddenAreaMesh");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline uint32_t getFlags() const	// Combination of "Rhi::ClearFlag", except for color-flag
		{
			return mFlags;
		}

		[[nodiscard]] inline uint32_t getStencil() const
		{
			return mStencil;
		}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::ICompositorResourcePass methods ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual CompositorPassTypeId getTypeId() const override
		{
			return TYPE_ID;
		}

		virtual void deserialize(uint32_t numberOfBytes, const uint8_t* data) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline explicit CompositorResourcePassVrHiddenAreaMesh(const CompositorTarget& compositorTarget) :
			ICompositorResourcePass(compositorTarget),
			mFlags(0),
			mStencil(0)
		{
			// Nothing here
		}

		inline virtual ~CompositorResourcePassVrHiddenAreaMesh() override
		{
			// Nothing here
		}

		explicit CompositorResourcePassVrHiddenAreaMesh(const CompositorResourcePassVrHiddenAreaMesh&) = delete;
		CompositorResourcePassVrHiddenAreaMesh& operator=(const CompositorResourcePassVrHiddenAreaMesh&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32_t mFlags;	// Combination of "Rhi::ClearFlag", except for color-flag
		uint32_t mStencil;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
