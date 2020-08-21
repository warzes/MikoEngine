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
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t MaterialTechniqueId;	// Material technique identifier, result of hashing the material technique name via "Renderer::StringId"


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class CompositorResourcePassScene : public ICompositorResourcePass
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CompositorPassFactory;	// The only one allowed to create instances of this class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t TYPE_ID = STRING_ID("Scene");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline uint8_t getMinimumRenderQueueIndex() const	// Inclusive
		{
			return mMinimumRenderQueueIndex;
		}

		[[nodiscard]] inline uint8_t getMaximumRenderQueueIndex() const	// Inclusive
		{
			return mMaximumRenderQueueIndex;
		}

		[[nodiscard]] inline bool isTransparentPass() const
		{
			return mTransparentPass;
		}

		[[nodiscard]] inline MaterialTechniqueId getMaterialTechniqueId() const
		{
			return mMaterialTechniqueId;
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
		[[nodiscard]] virtual bool getRenderQueueIndexRange(uint8_t& minimumRenderQueueIndex, uint8_t& maximumRenderQueueIndex) const override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline explicit CompositorResourcePassScene(const CompositorTarget& compositorTarget) :
			ICompositorResourcePass(compositorTarget),
			mMinimumRenderQueueIndex(0),
			mMaximumRenderQueueIndex(255),
			mTransparentPass(false),
			mMaterialTechniqueId(getInvalid<MaterialTechniqueId>())
		{
			// Nothing here
		}

		inline virtual ~CompositorResourcePassScene() override
		{
			// Nothing here
		}

		explicit CompositorResourcePassScene(const CompositorResourcePassScene&) = delete;
		CompositorResourcePassScene& operator=(const CompositorResourcePassScene&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint8_t				mMinimumRenderQueueIndex;	// Inclusive
		uint8_t				mMaximumRenderQueueIndex;	// Inclusive
		bool				mTransparentPass;
		MaterialTechniqueId	mMaterialTechniqueId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
