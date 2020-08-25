#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Core/StringId.h"
#include "Core/GetInvalid.h"
#include "Renderer/Resource/IResourceListener.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class IRenderer;
	class SkeletonAnimationEvaluator;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringId AssetId;						// Asset identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef uint32_t SkeletonResourceId;			// POD skeleton resource identifier
	typedef uint32_t SkeletonAnimationResourceId;	// POD skeleton animation resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Rigid skeleton animation controller
	*
	*  @todo
	*    - TODO(co) Right now only a single skeleton animation at one and the same time is supported to have something to start with.
	*               This isn't practical, of course, and in reality one has multiple animation sources at one and the same time which
	*               are blended together. But well, as mentioned, one has to start somewhere.
	*    - TODO(co) Currently "Renderer::SkeletonAnimationEvaluator" is directly used, probably it makes sense to manage those
	*               and then update all of them in parallel using multithreading
	*    - TODO(co) It might make sense to let the skeleton animation resource manager manage skeleton animation controller instances as well
	*/
	class SkeletonAnimationController final : public IResourceListener
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class SkeletonAnimationResourceManager;	// Calls "Renderer::SkeletonAnimationController::update()"


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] renderer
		*    Renderer to use
		*  @param[in] skeletonResourceId
		*    ID of the controlled skeleton resource
		*/
		inline SkeletonAnimationController(const IRenderer& renderer, SkeletonResourceId skeletonResourceId) :
			mRenderer(renderer),
			mSkeletonResourceId(skeletonResourceId),
			mSkeletonAnimationResourceId(GetInvalid<SkeletonAnimationResourceId>()),
			mSkeletonAnimationEvaluator(nullptr),
			mTimeInSeconds(0.0f)
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline ~SkeletonAnimationController()
		{
			clear();
		}

		/**
		*  @brief
		*    Start skeleton animation by resource ID
		*
		*  @param[in] skeletonAnimationResourceId
		*    Skeleton animation resource ID
		*/
		void startSkeletonAnimationByResourceId(SkeletonAnimationResourceId skeletonAnimationResourceId);

		/**
		*  @brief
		*    Start skeleton animation by asset ID
		*
		*  @param[in] skeletonAnimationAssetId
		*    Skeleton animation asset ID
		*/
		void startSkeletonAnimationByAssetId(AssetId skeletonAnimationAssetId);

		/**
		*  @brief
		*    Clear the controller
		*/
		void clear();


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::IResourceListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onLoadingStateChange(const IResource& resource) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit SkeletonAnimationController(const SkeletonAnimationController&) = delete;
		SkeletonAnimationController& operator=(const SkeletonAnimationController&) = delete;
		void createSkeletonAnimationEvaluator();
		void destroySkeletonAnimationEvaluator();

		/**
		*  @brief
		*    Update the controller
		*
		*  @param[in] pastSecondsSinceLastFrame
		*    Past seconds since last frame
		*/
		void update(float pastSecondsSinceLastFrame);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const IRenderer&			mRenderer;						// Renderer to use
		SkeletonResourceId			mSkeletonResourceId;			// ID of the controlled skeleton resource
		SkeletonAnimationResourceId mSkeletonAnimationResourceId;	// Skeleton animation resource ID, can be set to invalid value
		SkeletonAnimationEvaluator* mSkeletonAnimationEvaluator;	// Skeleton animation evaluator instance, can be a null pointer, destroy the instance if you no longer need it
		float						mTimeInSeconds;					// Time in seconds


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
