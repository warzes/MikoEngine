#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/MaterialBlueprint/MaterialBlueprintResource.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	// Material blueprint file format content:
	// - File format header
	// - Material blueprint header
	// - Material blueprint properties
	// - Visual importance of shader properties
	// - Root signature
	// - Pipeline state: A material blueprint can have a compute or a graphics pipeline state, but never both at one and the same time
	//   - Compute pipeline state object (PSO)
	//     - Compute shader blueprint
	//   - Graphics pipeline state object (PSO)
	//     - Graphics shader blueprints
	//     - Rasterization state etc.
	// - Resources
	//   - Uniform buffers
	//   - Texture buffers
	//   - Sampler states
	//   - Textures
	namespace v1MaterialBlueprint
	{


		//[-------------------------------------------------------]
		//[ Definitions                                           ]
		//[-------------------------------------------------------]
		static constexpr uint32_t FORMAT_TYPE	 = STRING_ID("MaterialBlueprint");
		static constexpr uint32_t FORMAT_VERSION = 11;

		#pragma pack(push)
		#pragma pack(1)
			struct MaterialBlueprintHeader final
			{
				uint32_t numberOfProperties;
				uint32_t numberOfShaderCombinationProperties;
				uint32_t numberOfIntegerShaderCombinationProperties;
				uint32_t numberOfUniformBuffers;
				uint32_t numberOfTextureBuffers;
				uint32_t numberOfStructuredBuffers;
				uint32_t numberOfSamplerStates;
				uint32_t numberOfTextures;
			};

			struct RootSignatureHeader final
			{
				uint32_t numberOfRootParameters;
				uint32_t numberOfDescriptorRanges;
				uint32_t numberOfStaticSamplers;
				uint32_t flags;
			};

			struct UniformBufferHeader final
			{
				uint32_t							   rootParameterIndex;			// Root parameter index = resource group index
				MaterialBlueprintResource::BufferUsage bufferUsage;
				uint32_t							   numberOfElements;
				uint32_t							   numberOfElementProperties;
				uint32_t							   uniformBufferNumberOfBytes;	// Includes handling of packing rules for uniform variables (see "Reference for HLSL - Shader Models vs Shader Profiles - Shader Model 4 - Packing Rules for Constant Variables" at https://msdn.microsoft.com/en-us/library/windows/desktop/bb509632%28v=vs.85%29.aspx )
			};

			struct TextureBufferHeader final
			{
				MaterialPropertyValue				   materialPropertyValue = MaterialPropertyValue::fromUnknown();
				uint32_t							   rootParameterIndex	 = getInvalid<uint32_t>();	// Root parameter index = resource group index
				MaterialBlueprintResource::BufferUsage bufferUsage			 = MaterialBlueprintResource::BufferUsage::UNKNOWN;
			};

			struct SamplerState final
			{
				Rhi::SamplerState samplerState;
				uint32_t		  rootParameterIndex;	// Root parameter index = resource group index
			};

			struct Texture final
			{
				uint32_t		 rootParameterIndex;	// Root parameter index = resource group index
				MaterialProperty materialProperty;
				AssetId			 fallbackTextureAssetId;
				bool			 rgbHardwareGammaCorrection;
				uint32_t		 samplerStateIndex;		// Index of the material blueprint sampler state resource to use, can be invalid (e.g. texel fetch instead of sampling might be used)

				Texture() :
					rootParameterIndex(getInvalid<uint32_t>()),
					rgbHardwareGammaCorrection(false),
					samplerStateIndex(getInvalid<uint32_t>())
				{
					// Nothing here
				}

				Texture(uint32_t _rootParameterIndex, MaterialProperty _materialProperty, AssetId _fallbackTextureAssetId, bool _rgbHardwareGammaCorrection, uint32_t _samplerStateIndex) :
					rootParameterIndex(_rootParameterIndex),
					materialProperty(_materialProperty),
					fallbackTextureAssetId(_fallbackTextureAssetId),
					rgbHardwareGammaCorrection(_rgbHardwareGammaCorrection),
					samplerStateIndex(_samplerStateIndex)
				{
					// Nothing here
				}
			};
		#pragma pack(pop)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // v1Material
} // Renderer
