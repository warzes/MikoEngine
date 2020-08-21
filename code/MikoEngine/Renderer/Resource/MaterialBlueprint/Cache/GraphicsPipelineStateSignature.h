#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/ShaderBlueprint/GraphicsShaderType.h"
#include "Renderer/Resource/ShaderBlueprint/Cache/ShaderProperties.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class ShaderBlueprintResource;
	class MaterialBlueprintResource;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t MaterialBlueprintResourceId;		// POD material blueprint resource identifier
	typedef uint32_t GraphicsPipelineStateSignatureId;	// Graphics pipeline state signature identifier, result of hashing the referenced shaders as well as other pipeline state properties
	typedef uint32_t ShaderCombinationId;				// Shader combination identifier, result of hashing the shader combination generating shader blueprint resource, shader properties and dynamic shader pieces


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Graphics pipeline state signature
	*
	*  @see
	*    - See "Renderer::GraphicsPipelineStateCacheManager" for additional information
	*/
	class GraphicsPipelineStateSignature final
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] static ShaderCombinationId generateShaderCombinationId(const ShaderBlueprintResource& shaderBlueprintResource, const ShaderProperties& shaderProperties);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline GraphicsPipelineStateSignature() :
			mMaterialBlueprintResourceId(getInvalid<MaterialBlueprintResourceId>()),
			mSerializedGraphicsPipelineStateHash(getInvalid<uint32_t>()),
			mGraphicsPipelineStateSignatureId(getInvalid<GraphicsPipelineStateSignatureId>()),
			mShaderCombinationId{getInvalid<ShaderCombinationId>(), getInvalid<ShaderCombinationId>(), getInvalid<ShaderCombinationId>(), getInvalid<ShaderCombinationId>(), getInvalid<ShaderCombinationId>()}
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] materialBlueprintResource
		*    Material blueprint resource to use
		*  @param[in] serializedGraphicsPipelineStateHash
		*    FNV1a hash of "Rhi::SerializedGraphicsPipelineState"
		*  @param[in] shaderProperties
		*    Shader properties to use, you should ensure that this shader properties are already optimized by using e.g. "Renderer::MaterialBlueprintResource::optimizeShaderProperties()"
		*/
		inline GraphicsPipelineStateSignature(const MaterialBlueprintResource& materialBlueprintResource, uint32_t serializedGraphicsPipelineStateHash, const ShaderProperties& shaderProperties)
		{
			set(materialBlueprintResource, serializedGraphicsPipelineStateHash, shaderProperties);
		}

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] graphicsPipelineStateSignature
		*    Graphics pipeline state signature to copy from
		*/
		explicit GraphicsPipelineStateSignature(const GraphicsPipelineStateSignature& graphicsPipelineStateSignature);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~GraphicsPipelineStateSignature()
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Copy operator
		*/
		GraphicsPipelineStateSignature& operator=(const GraphicsPipelineStateSignature& graphicsPipelineStateSignature);

		/**
		*  @brief
		*    Setter
		*
		*  @param[in] materialBlueprintResource
		*    Material blueprint resource to use
		*  @param[in] serializedGraphicsPipelineStateHash
		*    FNV1a hash of "Rhi::SerializedGraphicsPipelineState"
		*  @param[in] shaderProperties
		*    Shader properties to use, you should ensure that this shader properties are already optimized by using e.g. "Renderer::MaterialBlueprintResource::optimizeShaderProperties()"
		*/
		void set(const MaterialBlueprintResource& materialBlueprintResource, uint32_t serializedGraphicsPipelineStateHash, const ShaderProperties& shaderProperties);

		//[-------------------------------------------------------]
		//[ Getter for input data                                 ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline MaterialBlueprintResourceId getMaterialBlueprintResourceId() const
		{
			return mMaterialBlueprintResourceId;
		}

		[[nodiscard]] inline uint32_t getSerializedGraphicsPipelineStateHash() const
		{
			return mSerializedGraphicsPipelineStateHash;
		}

		[[nodiscard]] inline const ShaderProperties& getShaderProperties() const
		{
			return mShaderProperties;
		}

		//[-------------------------------------------------------]
		//[ Getter for derived data                               ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline GraphicsPipelineStateSignatureId getGraphicsPipelineStateSignatureId() const
		{
			return mGraphicsPipelineStateSignatureId;
		}

		[[nodiscard]] inline ShaderCombinationId getShaderCombinationId(GraphicsShaderType graphicsShaderType) const
		{
			return mShaderCombinationId[static_cast<uint8_t>(graphicsShaderType)];
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Input data
		MaterialBlueprintResourceId	mMaterialBlueprintResourceId;
		uint32_t					mSerializedGraphicsPipelineStateHash;
		ShaderProperties			mShaderProperties;
		// Derived data
		GraphicsPipelineStateSignatureId mGraphicsPipelineStateSignatureId;
		ShaderCombinationId				 mShaderCombinationId[NUMBER_OF_GRAPHICS_SHADER_TYPES];


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
