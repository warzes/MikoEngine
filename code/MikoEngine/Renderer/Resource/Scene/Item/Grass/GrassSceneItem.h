#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/Scene/Item/MaterialSceneItem.h"


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
	*    Grass scene item
	*
	*  @todo
	*    - TODO(co) Work-in-progress
	*/
	class GrassSceneItem final : public MaterialSceneItem
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class SceneFactory;	// Needs to be able to create scene item instances


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t TYPE_ID = SE_STRING_ID("GrassSceneItem");
		struct GrassDataStruct final
		{
			float PositionSize[4];	// Object space grass xyz-position, w = grass size
			float ColorRotation[4];	// Linear RGB grass color and rotation in radians
		};


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::ISceneItem methods           ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual SceneItemTypeId getSceneItemTypeId() const override
		{
			return TYPE_ID;
		}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::ISceneItem methods        ]
	//[-------------------------------------------------------]
	protected:
		[[nodiscard]] virtual void onExecuteOnRendering(const Rhi::IRenderTarget& renderTarget, const CompositorContextData& compositorContextData, Rhi::CommandBuffer& commandBuffer) const override;


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::MaterialSceneItem methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void initialize() override;
		virtual void onMaterialResourceCreated() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit GrassSceneItem(SceneResource& sceneResource);

		inline virtual ~GrassSceneItem() override
		{
			// Nothing here
		}

		explicit GrassSceneItem(const GrassSceneItem&) = delete;
		GrassSceneItem& operator=(const GrassSceneItem&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32_t				  mMaximumNumberOfGrass;	// Maximum number of grass
		Rhi::IStructuredBufferPtr mStructuredBufferPtr;		// Structured buffer the data of the individual grass ("Renderer::GrassSceneItem::GrassDataStruct")
		Rhi::IIndirectBufferPtr   mIndirectBufferPtr;		// Indirect buffer holding data related to the current grass "Rhi::DrawArguments" draw call


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
