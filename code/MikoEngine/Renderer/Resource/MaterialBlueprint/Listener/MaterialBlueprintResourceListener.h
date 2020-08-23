#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]

#include "Renderer/Core/StringId.h"
#include "Renderer/Core/GetInvalid.h"
#include "Renderer/Resource/MaterialBlueprint/Listener/IMaterialBlueprintResourceListener.h"

// Disable warnings in external headers, we can't fix them
SE_PRAGMA_WARNING_PUSH
	SE_PRAGMA_WARNING_DISABLE_MSVC(4127)	// warning C4127: conditional expression is constant
	SE_PRAGMA_WARNING_DISABLE_MSVC(4201)	// warning C4201: nonstandard extension used: nameless struct/union
	SE_PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
	SE_PRAGMA_WARNING_DISABLE_MSVC(4324)	// warning C4324: '<x>': structure was padded due to alignment specifier
	#include <glm/glm.hpp>
SE_PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class HosekWilkieSky;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t			 TextureResourceId;	// POD texture resource identifier
	typedef StringId			 AssetId;			// Asset identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef std::vector<AssetId> AssetIds;


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Material blueprint resource listener
	*
	*  @remarks
	*    Several things have to be taken into consideration when feeding shaders with parameters:
	*    - Reversed-Z ( https://developer.nvidia.com/content/depth-precision-visualized ) for improved depth buffer precision to reduce z-fighting
	*    - Camera relative rendering for rendering large scale scenes without jittering/wobbling
	*    - Single pass stereo rendering via instancing
	*/
	class MaterialBlueprintResourceListener : public IMaterialBlueprintResourceListener
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the asset IDs of automatically generated dynamic default texture assets
		*
		*  @param[out] assetIds
		*    Receives the asset IDs of automatically generated dynamic default texture assets, the list is not cleared before new entries are added
		*
		*  @remarks
		*    The material blueprint resource listener automatically generates some dynamic default texture assets one can reference e.g. inside material blueprint resources:
		*    - "Engine/Texture/DynamicByCode/IdentityColorCorrectionLookupTable3D"
		*    - "Engine/Texture/DynamicByCode/SsaoSampleKernel"
		*    - "Engine/Texture/DynamicByCode/SsaoNoise4x4"
		*/
		static void getDefaultTextureAssetIds(AssetIds& assetIds);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline MaterialBlueprintResourceListener() :
			mIdentityColorCorrectionLookupTable3D(getInvalid<TextureResourceId>()),
			mSsaoSampleKernelTextureResourceId(getInvalid<TextureResourceId>()),
			mSsaoNoiseTexture4x4ResourceId(getInvalid<TextureResourceId>()),
			mRenderer(nullptr),
			mPassData(nullptr),
			mCompositorContextData(nullptr),
			mWorldSpaceCameraPosition(0.0f, 0.0f, 0.0f),
			mCameraRelativeWorldSpaceCameraPosition{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
			mRenderTargetWidth(1),
			mRenderTargetHeight(1),
			mNearZ(0.0f),
			mFarZ(0.0f),
			mPreviousJitter(0.0f, 0.0f),
			mPreviousNumberOfRenderedFrames(getInvalid<uint64_t>()),
			mHosekWilkieSky(nullptr),
			#if SE_DEBUG
				mIsComputePipeline(false),
			#endif
			mObjectSpaceToWorldSpaceTransform(nullptr),
			mMaterialTechnique(nullptr)
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~MaterialBlueprintResourceListener() override
		{
			clear();
		}

		/**
		*  @brief
		*    Clear the material blueprint resource listener by freeing e.g. allocated memory
		*/
		void clear();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		explicit MaterialBlueprintResourceListener(const MaterialBlueprintResourceListener&) = delete;
		MaterialBlueprintResourceListener& operator=(const MaterialBlueprintResourceListener&) = delete;

		[[nodiscard]] inline const PassBufferManager::PassData& getPassData() const	// Memory address received via "Renderer::MaterialBlueprintResourceListener::beginFillPass()", can be invalid outside the correct scope, don't destroy the memory
		{
			RHI_ASSERT(nullptr != mPassData, "Invalid pass data")
			return *mPassData;
		}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::IMaterialBlueprintResourceListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onStartup(const IRenderer& renderer) override;
		virtual void onShutdown(const IRenderer& renderer) override;

		inline virtual void beginFillUnknown() override
		{
			// Nothing here
		}

		[[nodiscard]] inline virtual bool fillUnknownValue([[maybe_unused]] uint32_t referenceValue, [[maybe_unused]] uint8_t* buffer, [[maybe_unused]] uint32_t numberOfBytes) override
		{
			// Nothing here

			// Value not filled
			return false;
		}

		virtual void beginFillPass(IRenderer& renderer, const Rhi::IRenderTarget* renderTarget, const CompositorContextData& compositorContextData, PassBufferManager::PassData& passData) override;
		[[nodiscard]] virtual bool fillPassValue(uint32_t referenceValue, uint8_t* buffer, uint32_t numberOfBytes) override;

		inline virtual void beginFillMaterial() override
		{
			// Nothing here
		}

		[[nodiscard]] inline virtual bool fillMaterialValue([[maybe_unused]] uint32_t referenceValue, [[maybe_unused]] uint8_t* buffer, [[maybe_unused]] uint32_t numberOfBytes) override
		{
			// Nothing here

			// Value not filled
			return false;
		}

		inline virtual void beginFillInstance(const PassBufferManager::PassData& passData, const Transform& objectSpaceToWorldSpaceTransform, MaterialTechnique& materialTechnique) override
		{
			// Remember the pass data memory address of the current scope
			// TODO(co) Rethink the evil const-cast on here
			mPassData = &const_cast<PassBufferManager::PassData&>(passData);

			// Remember the instance data of the current scope
			mObjectSpaceToWorldSpaceTransform = &objectSpaceToWorldSpaceTransform;
			mMaterialTechnique				  = &materialTechnique;
		}

		[[nodiscard]] virtual bool fillInstanceValue(uint32_t referenceValue, uint8_t* buffer, uint32_t numberOfBytes, uint32_t instanceTextureBufferStartIndex) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		[[nodiscard]] glm::vec3 getWorldSpaceSunlightDirection() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Resource
		TextureResourceId mIdentityColorCorrectionLookupTable3D;
		TextureResourceId mSsaoSampleKernelTextureResourceId;
		TextureResourceId mSsaoNoiseTexture4x4ResourceId;

		// Pass
		IRenderer*					 mRenderer;					// Memory address received via "Renderer::MaterialBlueprintResourceListener::beginFillPass()", can be a null pointer outside the correct scope, don't destroy the memory
		PassBufferManager::PassData* mPassData;					// Memory address received via "Renderer::MaterialBlueprintResourceListener::beginFillPass()", can be a null pointer outside the correct scope, don't destroy the memory
		const CompositorContextData* mCompositorContextData;
		glm::dvec3					 mWorldSpaceCameraPosition;	// Cached 64 bit world space position of the camera since often accessed due to camera relative rendering
		glm::vec3					 mCameraRelativeWorldSpaceCameraPosition[2];
		uint32_t					 mRenderTargetWidth;
		uint32_t					 mRenderTargetHeight;
		float						 mNearZ;
		float						 mFarZ;
		glm::vec2					 mPreviousJitter;
		uint64_t					 mPreviousNumberOfRenderedFrames;
		HosekWilkieSky*				 mHosekWilkieSky;
		#if SE_DEBUG
			bool					 mIsComputePipeline;
		#endif

		// Instance
		const Transform*   mObjectSpaceToWorldSpaceTransform;
		MaterialTechnique* mMaterialTechnique;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
