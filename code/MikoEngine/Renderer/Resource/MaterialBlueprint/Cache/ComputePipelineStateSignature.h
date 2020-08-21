#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/ShaderBlueprint/Cache/ShaderProperties.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
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
	typedef uint32_t ComputePipelineStateSignatureId;	// Compute pipeline state signature identifier, result of hashing the referenced shaders as well as other pipeline state properties
	typedef uint32_t ShaderCombinationId;				// Shader combination identifier, result of hashing the shader combination generating shader blueprint resource, shader properties and dynamic shader pieces


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Compute pipeline state signature
	*
	*  @see
	*    - See "Renderer::ComputePipelineStateCacheManager" for additional information
	*/
	class ComputePipelineStateSignature final
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline ComputePipelineStateSignature() :
			mMaterialBlueprintResourceId(getInvalid<MaterialBlueprintResourceId>()),
			mComputePipelineStateSignatureId(getInvalid<ComputePipelineStateSignatureId>()),
			mShaderCombinationId(getInvalid<ShaderCombinationId>())
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] materialBlueprintResource
		*    Material blueprint resource to use
		*  @param[in] shaderProperties
		*    Shader properties to use, you should ensure that this shader properties are already optimized by using e.g. "Renderer::MaterialBlueprintResource::optimizeShaderProperties()"
		*/
		inline ComputePipelineStateSignature(const MaterialBlueprintResource& materialBlueprintResource, const ShaderProperties& shaderProperties)
		{
			set(materialBlueprintResource, shaderProperties);
		}

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] computePipelineStateSignature
		*    Compute pipeline state signature to copy from
		*/
		explicit ComputePipelineStateSignature(const ComputePipelineStateSignature& computePipelineStateSignature);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~ComputePipelineStateSignature()
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Copy operator
		*/
		ComputePipelineStateSignature& operator=(const ComputePipelineStateSignature& computePipelineStateSignature);

		/**
		*  @brief
		*    Setter
		*
		*  @param[in] materialBlueprintResource
		*    Material blueprint resource to use
		*  @param[in] shaderProperties
		*    Shader properties to use, you should ensure that this shader properties are already optimized by using e.g. "Renderer::MaterialBlueprintResource::optimizeShaderProperties()"
		*/
		void set(const MaterialBlueprintResource& materialBlueprintResource, const ShaderProperties& shaderProperties);

		//[-------------------------------------------------------]
		//[ Getter for input data                                 ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline MaterialBlueprintResourceId getMaterialBlueprintResourceId() const
		{
			return mMaterialBlueprintResourceId;
		}

		[[nodiscard]] inline const ShaderProperties& getShaderProperties() const
		{
			return mShaderProperties;
		}

		//[-------------------------------------------------------]
		//[ Getter for derived data                               ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline ComputePipelineStateSignatureId getComputePipelineStateSignatureId() const
		{
			return mComputePipelineStateSignatureId;
		}

		[[nodiscard]] inline ShaderCombinationId getShaderCombinationId() const
		{
			return mShaderCombinationId;
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Input data
		MaterialBlueprintResourceId	mMaterialBlueprintResourceId;
		ShaderProperties			mShaderProperties;
		// Derived data
		ComputePipelineStateSignatureId	mComputePipelineStateSignatureId;
		ShaderCombinationId				mShaderCombinationId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
