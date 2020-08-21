#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/IResourceLoader.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	class IRenderer;
	class SkeletonAnimationResource;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class SkeletonAnimationResourceLoader final : public IResourceLoader
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class SkeletonAnimationResourceManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t TYPE_ID = STRING_ID("skeleton_animation");


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual ResourceLoaderTypeId getResourceLoaderTypeId() const override
		{
			return TYPE_ID;
		}

		virtual void initialize(const Asset& asset, bool reload, IResource& resource) override;

		[[nodiscard]] inline virtual bool hasDeserialization() const override
		{
			return true;
		}

		[[nodiscard]] virtual bool onDeserialization(IFile& file) override;

		[[nodiscard]] inline virtual bool hasProcessing() const override
		{
			return false;
		}

		inline virtual void onProcessing() override
		{
			// Nothing here
		}

		[[nodiscard]] inline virtual bool onDispatch() override
		{
			// Fully loaded
			return true;
		}

		[[nodiscard]] inline virtual bool isFullyLoaded() override
		{
			// Fully loaded
			return true;
		}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		#if SE_DEBUG
			inline SkeletonAnimationResourceLoader(IResourceManager& resourceManager, IRenderer& renderer) :
				IResourceLoader(resourceManager),
				mRenderer(renderer),
				mSkeletonAnimationResource(nullptr)
			{
				// Nothing here
			}
		#else
			inline explicit SkeletonAnimationResourceLoader(IResourceManager& resourceManager) :
				IResourceLoader(resourceManager),
				mSkeletonAnimationResource(nullptr)
			{
				// Nothing here
			}
		#endif

		inline virtual ~SkeletonAnimationResourceLoader() override
		{
			// Nothing here
		}

		explicit SkeletonAnimationResourceLoader(const SkeletonAnimationResourceLoader&) = delete;
		SkeletonAnimationResourceLoader& operator=(const SkeletonAnimationResourceLoader&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		#if SE_DEBUG
			IRenderer& mRenderer;	// Renderer instance, do not destroy the instance
		#endif
		SkeletonAnimationResource* mSkeletonAnimationResource;	// Destination resource


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
