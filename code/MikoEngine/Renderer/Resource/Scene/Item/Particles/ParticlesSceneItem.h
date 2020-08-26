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
	*    Particles scene item
	*
	*  @todo
	*    - TODO(co) Particles rendering part is implemented, implement GPU based particles simulation as well
	*/
	class ParticlesSceneItem final : public MaterialSceneItem
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class SceneFactory;	// Needs to be able to create scene item instances


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t TYPE_ID = SE_STRING_ID("ParticlesSceneItem");
		struct ParticleDataStruct final
		{
			float PositionSize[4];	// Object space particle xyz-position, w = particle size
			float Color[4];			// Linear RGBA particle color and opacity
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
		explicit ParticlesSceneItem(SceneResource& sceneResource);

		inline virtual ~ParticlesSceneItem() override
		{
			// Nothing here
		}

		explicit ParticlesSceneItem(const ParticlesSceneItem&) = delete;
		ParticlesSceneItem& operator=(const ParticlesSceneItem&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32_t				  mMaximumNumberOfParticles;	// Maximum number of particles
		Rhi::IStructuredBufferPtr mStructuredBufferPtr;			// Structured buffer the data of the individual particles ("Renderer::ParticlesSceneItem::ParticleDataStruct")


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
