#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Core/Manager.h"
#include "Renderer/Resource/MaterialBlueprint/MaterialBlueprintResource.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class Renderable;
	class IRenderer;
	class MaterialTechnique;
	class PassBufferManager;
}


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
	*    Uniform instance buffer manager; limited float4 only data per instance without an additional texture buffer
	*
	*  @remarks
	*    "Renderer::UniformInstanceBufferManager" is a simplified version of "Renderer::TextureInstanceBufferManager". Shared code is duplicated by intent
	*     (including this comment) to avoid making the implementations too complex due to over-engineering. This is performance critical code and the topic is complex
	*     enough as it is. When changing one implementation don't forget to update the other one as well.
	*/
	class UniformInstanceBufferManager final : private Manager
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] renderer
		*    Renderer instance to use
		*/
		explicit UniformInstanceBufferManager(IRenderer& renderer);

		/**
		*  @brief
		*    Destructor
		*/
		~UniformInstanceBufferManager();

		/**
		*  @brief
		*    Startup instance buffer filling
		*
		*  @param[in] materialBlueprintResource
		*    Material blueprint resource
		*  @param[out] commandBuffer
		*    RHI command buffer to fill
		*/
		void startupBufferFilling(const MaterialBlueprintResource& materialBlueprintResource, Rhi::CommandBuffer& commandBuffer);

		/**
		*  @brief
		*    Fill the instance buffer
		*
		*  @param[in] materialBlueprintResource
		*    Material blueprint resource
		*  @param[in] passBufferManager
		*    Pass buffer manager instance to use, can be a null pointer
		*  @param[in] instanceUniformBuffer
		*    Instance uniform buffer instance to use
		*  @param[in] renderable
		*    Renderable to fill the buffer for
		*  @param[in] materialTechnique
		*    Used material technique
		*  @param[out] commandBuffer
		*    RHI command buffer to fill
		*
		*  @return
		*    Start instance location, used for draw ID (see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html)
		*/
		[[nodiscard]] uint32_t fillBuffer(const MaterialBlueprintResource& materialBlueprintResource, PassBufferManager* passBufferManager, const MaterialBlueprintResource::UniformBuffer& instanceUniformBuffer, const Renderable& renderable, MaterialTechnique& materialTechnique, Rhi::CommandBuffer& commandBuffer);

		/**
		*  @brief
		*    Called pre command buffer execution
		*/
		void onPreCommandBufferExecution();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit UniformInstanceBufferManager(const UniformInstanceBufferManager&) = delete;
		UniformInstanceBufferManager& operator=(const UniformInstanceBufferManager&) = delete;
		void createInstanceBuffer();
		void mapCurrentInstanceBuffer();
		void unmapCurrentInstanceBuffer();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct InstanceBuffer final
		{
			Rhi::IUniformBuffer* uniformBuffer;	// Uniform buffer instance, always valid
			Rhi::IResourceGroup* resourceGroup;	// Resource group instance, can be a null pointer
			bool				 mapped;
			explicit InstanceBuffer(Rhi::IUniformBuffer& _uniformBuffer) :
				uniformBuffer(&_uniformBuffer),
				resourceGroup(nullptr),
				mapped(false)
			{
				// Nothing here
			}

		};
		typedef std::vector<InstanceBuffer> InstanceBuffers;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IRenderer&		mRenderer;						// Renderer instance to use
		const uint32_t	mMaximumUniformBufferSize;		// Maximum uniform buffer size in bytes
		InstanceBuffers	mInstanceBuffers;				// Instance buffers
		// Current instance buffer related data
		size_t			mCurrentInstanceBufferIndex;	// Current instance buffer index, can be invalid if there's currently no current instance buffer
		InstanceBuffer* mCurrentInstanceBuffer;			// Current instance buffer, can be a null pointer, don't destroy the instance since this is just a reference
		uint8_t*		mStartUniformBufferPointer;
		uint8_t*		mCurrentUniformBufferPointer;
		uint32_t		mStartInstanceLocation;			// Start instance location, used for draw ID (see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html)


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
