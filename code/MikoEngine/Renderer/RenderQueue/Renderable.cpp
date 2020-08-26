#include "stdafx.h"
#include "Renderer/RenderQueue/Renderable.h"
#include "Renderer/RenderQueue/RenderableManager.h"
#include "Renderer/Resource/Material/MaterialResourceManager.h"
#include "Renderer/Resource/Material/MaterialResource.h"
#include "Renderer/Core/SwizzleVectorElementRemove.h"
#include "Renderer/IRenderer.h"

namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global variables                                      ]
		//[-------------------------------------------------------]
		Renderer::RenderableManager NullRenderableManager;


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	Renderable::Renderable() :
		// Debug
		#if SE_DEBUG
			mDebugName{},
		#endif
		// Data
		mRenderableManager(::detail::NullRenderableManager),
		mStartIndexLocation(0),
		mNumberOfIndices(0),
		mInstanceCount(1),
		mMaterialResourceId(GetInvalid<MaterialResourceId>()),
		mSkeletonResourceId(GetInvalid<SkeletonResourceId>()),
		mDrawIndexed(false),
		// Cached material data
		mRenderQueueIndex(0),
		mCastShadows(false),
		mUseAlphaMap(false),
		// Internal data
		mMaterialResourceManager(nullptr),
		mMaterialResourceAttachmentIndex(GetInvalid<int>())
	{
		// Nothing here
	}

	Renderable::Renderable(RenderableManager& renderableManager, const Rhi::IVertexArrayPtr& vertexArrayPtr, const MaterialResourceManager& materialResourceManager, MaterialResourceId materialResourceId, SkeletonResourceId skeletonResourceId, bool drawIndexed, uint32_t startIndexLocation, uint32_t numberOfIndices, uint32_t instanceCount RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
		// Debug
		#if SE_DEBUG
			mDebugName{},
		#endif
		// Data
		mRenderableManager(renderableManager),
		mVertexArrayPtr(vertexArrayPtr),
		mStartIndexLocation(startIndexLocation),
		mNumberOfIndices(numberOfIndices),
		mInstanceCount(instanceCount),
		mMaterialResourceId(GetInvalid<MaterialResourceId>()),
		mSkeletonResourceId(skeletonResourceId),
		mDrawIndexed(drawIndexed),
		// Cached material data
		mRenderQueueIndex(0),
		mCastShadows(false),
		mUseAlphaMap(false),
		// Internal data
		mMaterialResourceManager(nullptr),
		mMaterialResourceAttachmentIndex(GetInvalid<int>())
	{
		#if SE_DEBUG
			if ('\0' != debugName[0])
			{
				setDebugName(debugName);
			}
		#endif
		if (IsValid(materialResourceId))
		{
			setMaterialResourceId(materialResourceManager, materialResourceId);
		}
	}

	Renderable::Renderable(RenderableManager& renderableManager, const Rhi::IVertexArrayPtr& vertexArrayPtr, const MaterialResourceManager& materialResourceManager, MaterialResourceId materialResourceId, SkeletonResourceId skeletonResourceId, bool drawIndexed, const Rhi::IIndirectBufferPtr& indirectBufferPtr, uint32_t indirectBufferOffset, uint32_t numberOfDraws RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
		// Debug
		#if SE_DEBUG
			mDebugName{},
		#endif
		// Data
		mRenderableManager(renderableManager),
		mVertexArrayPtr(vertexArrayPtr),
		mIndirectBufferPtr(indirectBufferPtr),
		mIndirectBufferOffset(indirectBufferOffset),	// Indirect buffer used
		mNumberOfIndices(~0u),							// Invalid since read from the indirect buffer
		mNumberOfDraws(numberOfDraws),					// Indirect buffer used
		mMaterialResourceId(GetInvalid<MaterialResourceId>()),
		mSkeletonResourceId(skeletonResourceId),
		mDrawIndexed(drawIndexed),
		// Cached material data
		mRenderQueueIndex(0),
		mCastShadows(false),
		mUseAlphaMap(false),
		// Internal data
		mMaterialResourceManager(nullptr),
		mMaterialResourceAttachmentIndex(GetInvalid<int>())
	{
		#if SE_DEBUG
			if ('\0' != debugName[0])
			{
				setDebugName(debugName);
			}
		#endif
		if (IsValid(materialResourceId))
		{
			setMaterialResourceId(materialResourceManager, materialResourceId);
		}
	}

	Renderable::Renderable(RenderableManager& renderableManager, const Rhi::IVertexArrayPtr& vertexArrayPtr, const Rhi::IVertexArrayPtr& positionOnlyVertexArrayPtr, const MaterialResourceManager& materialResourceManager, MaterialResourceId materialResourceId, SkeletonResourceId skeletonResourceId, bool drawIndexed, uint32_t startIndexLocation, uint32_t numberOfIndices, uint32_t instanceCount RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
		// Debug
		#if SE_DEBUG
			mDebugName{},
		#endif
		// Data
		mRenderableManager(renderableManager),
		mVertexArrayPtr(vertexArrayPtr),
		mPositionOnlyVertexArrayPtr(positionOnlyVertexArrayPtr),
		mStartIndexLocation(startIndexLocation),
		mNumberOfIndices(numberOfIndices),
		mInstanceCount(instanceCount),
		mMaterialResourceId(GetInvalid<MaterialResourceId>()),
		mSkeletonResourceId(skeletonResourceId),
		mDrawIndexed(drawIndexed),
		// Cached material data
		mRenderQueueIndex(0),
		mCastShadows(false),
		mUseAlphaMap(false),
		// Internal data
		mMaterialResourceManager(nullptr),
		mMaterialResourceAttachmentIndex(GetInvalid<int>())
	{
		#if SE_DEBUG
			if ('\0' != debugName[0])
			{
				setDebugName(debugName);
			}
		#endif
		if (IsValid(materialResourceId))
		{
			setMaterialResourceId(materialResourceManager, materialResourceId);
		}
	}

	Renderable::Renderable(RenderableManager& renderableManager, const Rhi::IVertexArrayPtr& vertexArrayPtr, const Rhi::IVertexArrayPtr& positionOnlyVertexArrayPtr, const MaterialResourceManager& materialResourceManager, MaterialResourceId materialResourceId, SkeletonResourceId skeletonResourceId, bool drawIndexed, const Rhi::IIndirectBufferPtr& indirectBufferPtr, uint32_t indirectBufferOffset, uint32_t numberOfDraws RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
		// Debug
		#if SE_DEBUG
			mDebugName{},
		#endif
		// Data
		mRenderableManager(renderableManager),
		mVertexArrayPtr(vertexArrayPtr),
		mPositionOnlyVertexArrayPtr(positionOnlyVertexArrayPtr),
		mIndirectBufferPtr(indirectBufferPtr),
		mIndirectBufferOffset(indirectBufferOffset),	// Indirect buffer used
		mNumberOfIndices(~0u),							// Invalid since read from the indirect buffer
		mNumberOfDraws(numberOfDraws),					// Indirect buffer used
		mMaterialResourceId(GetInvalid<MaterialResourceId>()),
		mSkeletonResourceId(skeletonResourceId),
		mDrawIndexed(drawIndexed),
		// Cached material data
		mRenderQueueIndex(0),
		mCastShadows(false),
		mUseAlphaMap(false),
		// Internal data
		mMaterialResourceManager(nullptr),
		mMaterialResourceAttachmentIndex(GetInvalid<int>())
	{
		#if SE_DEBUG
			if ('\0' != debugName[0])
			{
				setDebugName(debugName);
			}
		#endif
		if (IsValid(materialResourceId))
		{
			setMaterialResourceId(materialResourceManager, materialResourceId);
		}
	}

	void Renderable::setMaterialResourceId(const MaterialResourceManager& materialResourceManager, MaterialResourceId materialResourceId)
	{
		// State change?
		if (mMaterialResourceId != materialResourceId)
		{
			// Detach the renderable from the previous material resource, first
			unsetMaterialResourceIdInternal();

			// Renderables can only be attached to really existing material resources
			MaterialResource* materialResource = materialResourceManager.tryGetById(materialResourceId);
			if (nullptr != materialResource)
			{
				// Sanity checks
				SE_ASSERT(IsInvalid(mMaterialResourceAttachmentIndex), "Invalid material resource attachment index")
				SE_ASSERT(nullptr == mMaterialResourceManager, "Invalid material resource manager instance")

				// Attach the renderable from the material resource
				mMaterialResourceId = materialResourceId;
				mMaterialResourceManager = &materialResourceManager;
				mMaterialResourceAttachmentIndex = static_cast<int>(materialResource->mAttachedRenderables.size());
				materialResource->mAttachedRenderables.push_back(this);

				{ // Cached material data, incremental updates are handled inside "Renderer::MaterialResource::setPropertyByIdInternal()"
					// Optional "RenderQueueIndex" (e.g. compositor materials usually don't need this property)
					const MaterialProperty* materialProperty = materialResource->getPropertyById(MaterialResource::RENDER_QUEUE_INDEX_PROPERTY_ID);
					if (nullptr != materialProperty)
					{
						const int renderQueueIndex = materialProperty->getIntegerValue();

						// Sanity checks
						SE_ASSERT(materialProperty->getUsage() == MaterialProperty::Usage::STATIC, "Invalid material property usage")
						SE_ASSERT(renderQueueIndex >= 0, "Invalid render queue index")
						SE_ASSERT(renderQueueIndex <= 255, "Invalid render queue index")

						// Set value
						mRenderQueueIndex = static_cast<uint8_t>(renderQueueIndex);
					}
					else
					{
						mRenderQueueIndex = 0;
					}

					// Optional "CastShadows" (e.g. compositor materials usually don't need this property)
					materialProperty = materialResource->getPropertyById(MaterialResource::CAST_SHADOWS_PROPERTY_ID);
					if (nullptr != materialProperty)
					{
						// Sanity checks
						SE_ASSERT(materialProperty->getUsage() == MaterialProperty::Usage::STATIC, "Invalid material property usage")

						// Set value
						mCastShadows = materialProperty->getBooleanValue();
					}
					else
					{
						mCastShadows = false;
					}

					// Optional "UseAlphaMap"
					materialProperty = materialResource->getPropertyById(MaterialResource::USE_ALPHA_MAP_PROPERTY_ID);
					if (nullptr != materialProperty)
					{
						// Sanity checks
						SE_ASSERT(materialProperty->getUsage() == MaterialProperty::Usage::SHADER_COMBINATION, "Invalid material property usage")

						// Set value
						mUseAlphaMap = materialProperty->getBooleanValue();
					}
					else
					{
						mUseAlphaMap = false;
					}
				}
			}
			else
			{
				// Error!
				SE_ASSERT(false, "We should never end up in here")
			}
		}
		else
		{
			// Sanity check
			SE_ASSERT((IsValid(mMaterialResourceId) && &materialResourceManager == mMaterialResourceManager) || (IsInvalid(mMaterialResourceId) && nullptr == mMaterialResourceManager), "Invalid renderable configuration")
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void Renderable::unsetMaterialResourceIdInternal()
	{
		if (IsValid(mMaterialResourceId))
		{
			// Sanity checks
			SE_ASSERT(nullptr != mMaterialResourceManager, "Invalid material resource manager")
			SE_ASSERT(IsValid(mMaterialResourceAttachmentIndex), "Invalid material resource attachment index")

			// Get the material resource we're going to detach from
			MaterialResource& materialResource = mMaterialResourceManager->getById(mMaterialResourceId);

			// Sanity checks
			SE_ASSERT(mMaterialResourceAttachmentIndex < static_cast<int>(materialResource.mAttachedRenderables.size()), "Invalid material resource attachment index")
			SE_ASSERT(this == *(materialResource.mAttachedRenderables.begin() + mMaterialResourceAttachmentIndex), "Invalid material resource attachment index")
			SE_ASSERT(materialResource.getId() == mMaterialResourceId, "Invalid material resource ID")

			// Detach the renderable from the material resource
			MaterialResource::AttachedRenderables::iterator iterator = materialResource.mAttachedRenderables.begin() + mMaterialResourceAttachmentIndex;
			iterator = ::detail::swizzleVectorElementRemove(materialResource.mAttachedRenderables, iterator);
			if (iterator != materialResource.mAttachedRenderables.end())
			{
				// The node that was at the end got swapped and has now a different index
				(*iterator)->mMaterialResourceAttachmentIndex = static_cast<int>(iterator - materialResource.mAttachedRenderables.begin());
			}
			SetInvalid(mMaterialResourceId);
			mMaterialResourceManager = nullptr;
			SetInvalid(mMaterialResourceAttachmentIndex);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
