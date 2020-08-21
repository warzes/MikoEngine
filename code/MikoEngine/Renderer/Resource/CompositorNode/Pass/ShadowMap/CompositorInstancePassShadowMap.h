#pragma once

#include "Renderer/Resource/CompositorNode/Pass/Scene/CompositorInstancePassScene.h"
#include "Renderer/Resource/CompositorNode/Pass/ShadowMap/CompositorResourcePassShadowMap.h"

//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class CompositorResourcePassCompute;
	class CompositorInstancePassCompute;
	class CompositorResourcePassShadowMap;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t TextureResourceId;	// POD texture resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Compositor instance pass shadow map
	*
	*  @note
	*    - Basing on https://mynameismjp.wordpress.com/2013/09/10/shadow-maps/ - https://github.com/TheRealMJP/Shadows
	*/
	class CompositorInstancePassShadowMap final : public CompositorInstancePassScene
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CompositorPassFactory;	// The only one allowed to create instances of this class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct PassData final
		{
			int			shadowMapSize = 0;
			glm::mat4	shadowMatrix;
			float		shadowCascadeSplits[CompositorResourcePassShadowMap::MAXIMUM_NUMBER_OF_SHADOW_CASCADES] = {};
			glm::vec4	shadowCascadeOffsets[CompositorResourcePassShadowMap::MAXIMUM_NUMBER_OF_SHADOW_CASCADES] = {};
			glm::vec4	shadowCascadeScales[CompositorResourcePassShadowMap::MAXIMUM_NUMBER_OF_SHADOW_CASCADES];
			uint8_t		currentShadowCascadeIndex = 0;
			float		shadowFilterSize = 0.0f;
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Settings                                              ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline bool isEnabled() const
		{
			return mEnabled;
		}

		inline void setEnabled(bool enabled)
		{
			if (mEnabled != enabled)
			{
				mEnabled = enabled;
				++mSettingsGenerationCounter;
			}
		}

		[[nodiscard]] inline uint32_t getShadowMapSize() const
		{
			return mShadowMapSize;
		}

		inline void setShadowMapSize(uint32_t shadowMapSize)
		{
			if (mShadowMapSize != shadowMapSize)
			{
				mShadowMapSize = shadowMapSize;
				++mSettingsGenerationCounter;
			}
		}

		[[nodiscard]] inline uint8_t getNumberOfShadowCascades() const
		{
			return mNumberOfShadowCascades;
		}

		void setNumberOfShadowCascades(uint8_t numberOfShadowCascades);

		[[nodiscard]] inline uint8_t getNumberOfShadowMultisamples() const
		{
			return mNumberOfShadowMultisamples;
		}

		void setNumberOfShadowMultisamples(uint8_t numberOfShadowMultisamples);

		[[nodiscard]] inline float getCascadeSplitsLambda() const
		{
			return mCascadeSplitsLambda;
		}

		inline void setCascadeSplitsLambda(float cascadeSplitsLambda)
		{
			mCascadeSplitsLambda = cascadeSplitsLambda;
		}

		[[nodiscard]] inline float getShadowFilterSize() const
		{
			return mShadowFilterSize;
		}

		inline void setShadowFilterSize(float shadowFilterSize)
		{
			mShadowFilterSize = shadowFilterSize;
		}

		[[nodiscard]] inline bool getStabilizeCascades() const
		{
			return mStabilizeCascades;
		}

		inline void setStabilizeCascades(bool stabilizeCascades)
		{
			mStabilizeCascades = stabilizeCascades;
		}

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline const PassData& getPassData() const
		{
			return mPassData;
		}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::ICompositorInstancePass methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onFillCommandBuffer(const Rhi::IRenderTarget* renderTarget, const CompositorContextData& compositorContextData, Rhi::CommandBuffer& commandBuffer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		CompositorInstancePassShadowMap(const CompositorResourcePassShadowMap& compositorResourcePassShadowMap, const CompositorNodeInstance& compositorNodeInstance);

		inline virtual ~CompositorInstancePassShadowMap() override
		{
			destroyShadowMapRenderTarget();
		}

		explicit CompositorInstancePassShadowMap(const CompositorInstancePassShadowMap&) = delete;
		CompositorInstancePassShadowMap& operator=(const CompositorInstancePassShadowMap&) = delete;
		void createShadowMapRenderTarget();
		void destroyShadowMapRenderTarget();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Settings
		bool	 mEnabled;						// Shadow enabled?
		uint32_t mShadowMapSize;				// The shadow map size is usually 512, 1024 or 2048
		uint8_t  mNumberOfShadowCascades;		// Number of shadow cascades, usually 4
		uint8_t  mNumberOfShadowMultisamples;	// The number of shadow multisamples per pixel (valid values: 1, 2, 4, 8)
		float	 mCascadeSplitsLambda;			// Cascade splits lambda
		float	 mShadowFilterSize;				// Shadow filter size
		bool	 mStabilizeCascades;			// Keeps consistent sizes for each cascade, and snaps each cascade so that they move in texel-sized increments. Reduces temporal aliasing artifacts, but reduces the effective resolution of the cascades. See Valient, M., "Stable Rendering of Cascaded Shadow Maps", In: Engel, W. F ., et al., "ShaderX6: Advanced Rendering Techniques", Charles River Media, 2008, ISBN 1-58450-544-3.
		// Internal
		uint32_t					   mSettingsGenerationCounter;	// Most simple solution to detect settings changes which make internal data invalid
		uint32_t					   mUsedSettingsGenerationCounter;
		PassData					   mPassData;
		Rhi::IFramebufferPtr		   mDepthFramebufferPtr;
		Rhi::IFramebufferPtr		   mVarianceFramebufferPtr[CompositorResourcePassShadowMap::MAXIMUM_NUMBER_OF_SHADOW_CASCADES];
		Rhi::IFramebufferPtr		   mIntermediateFramebufferPtr;
		TextureResourceId			   mDepthTextureResourceId;
		TextureResourceId			   mVarianceTextureResourceId;
		TextureResourceId			   mIntermediateDepthBlurTextureResourceId;
		CompositorResourcePassCompute* mDepthToExponentialVarianceCompositorResourcePassCompute;
		CompositorInstancePassCompute* mDepthToExponentialVarianceCompositorInstancePassCompute;
		CompositorResourcePassCompute* mHorizontalBlurCompositorResourcePassCompute;
		CompositorInstancePassCompute* mHorizontalBlurCompositorInstancePassCompute;
		CompositorResourcePassCompute* mVerticalBlurCompositorResourcePassCompute;
		CompositorInstancePassCompute* mVerticalBlurCompositorInstancePassCompute;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
