#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/MaterialBlueprint/BufferManager/PassBufferManager.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Transform;
namespace Renderer
{

	class IRenderer;
	class MaterialTechnique;
	class CompositorContextData;
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
	*    Abstract material blueprint resource listener interface
	*/
	class IMaterialBlueprintResourceListener
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class PassBufferManager;					// Is calling the protected interface methods
		friend class MaterialBlueprintResource;			// Is calling the protected interface methods
		friend class MaterialBufferManager;				// Is calling the protected interface methods
		friend class UniformInstanceBufferManager;		// Is calling the protected interface methods
		friend class TextureInstanceBufferManager;		// Is calling the protected interface methods
		friend class MaterialBlueprintResourceManager;	// Is calling the protected interface methods


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline IMaterialBlueprintResourceListener()
		{
			// Nothing here
		}

		inline virtual ~IMaterialBlueprintResourceListener()
		{
			// Nothing here
		}

		explicit IMaterialBlueprintResourceListener(const IMaterialBlueprintResourceListener&) = delete;
		IMaterialBlueprintResourceListener& operator=(const IMaterialBlueprintResourceListener&) = delete;


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::IMaterialBlueprintResourceListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onStartup(const IRenderer& renderer) = 0;	// Becomes the currently used material blueprint resource listener
		virtual void onShutdown(const IRenderer& renderer) = 0;	// Is no longer the currently used material blueprint resource listener
		virtual void beginFillUnknown() = 0;
		[[nodiscard]] virtual bool fillUnknownValue(uint32_t referenceValue, uint8_t* buffer, uint32_t numberOfBytes) = 0;
		virtual void beginFillPass(IRenderer& renderer, const Rhi::IRenderTarget* renderTarget, const CompositorContextData& compositorContextData, PassBufferManager::PassData& passData) = 0;	// The render target to render into must be valid for graphics pipeline and must be a null pointer for compute pipeline
		[[nodiscard]] virtual bool fillPassValue(uint32_t referenceValue, uint8_t* buffer, uint32_t numberOfBytes) = 0;
		virtual void beginFillMaterial() = 0;
		[[nodiscard]] virtual bool fillMaterialValue(uint32_t referenceValue, uint8_t* buffer, uint32_t numberOfBytes) = 0;

		// TODO(co) It might make sense to remove those instance methods from the interface and directly hard-code them for performance reasons. Profiling later on with real world scenes will show.
		virtual void beginFillInstance(const PassBufferManager::PassData& passData, const Transform& objectSpaceToWorldSpaceTransform, MaterialTechnique& materialTechnique) = 0;
		[[nodiscard]] virtual bool fillInstanceValue(uint32_t referenceValue, uint8_t* buffer, uint32_t numberOfBytes, uint32_t instanceTextureBufferStartIndex) = 0;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
