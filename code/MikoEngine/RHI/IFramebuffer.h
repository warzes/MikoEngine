#pragma once

namespace Rhi
{
	struct FramebufferAttachment final
	{
		ITexture* texture;
		uint32_t  mipmapIndex;
		uint32_t  layerIndex;	// "slice" in Direct3D terminology, depending on the texture type it's a 2D texture array layer, 3D texture slice or cube map face
		inline FramebufferAttachment() :
			texture(nullptr),
			mipmapIndex(0),
			layerIndex(0)
		{
		}
		inline FramebufferAttachment(ITexture* _texture, uint32_t _mipmapIndex = 0, uint32_t _layerIndex = 0) :
			texture(_texture),
			mipmapIndex(_mipmapIndex),
			layerIndex(_layerIndex)
		{
		}
	};

	/**
	*  @brief
	*    Abstract framebuffer (FBO) interface
	*/
	class IFramebuffer : public IRenderTarget
	{

		// Public methods
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IFramebuffer() override
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			--getRhi().getStatistics().currentNumberOfFramebuffers;
#endif
		}

		// Protected methods
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] renderPass
		*    Render pass to use, the framebuffer a reference to the render pass
		*/
		inline explicit IFramebuffer(IRenderPass& renderPass RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
			IRenderTarget(ResourceType::FRAMEBUFFER, renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER)
		{
#ifdef SE_RHI_STATISTICS
			// Update the statistics
			++getRhi().getStatistics().numberOfCreatedFramebuffers;
			++getRhi().getStatistics().currentNumberOfFramebuffers;
#endif
		}

		explicit IFramebuffer(const IFramebuffer& source) = delete;
		IFramebuffer& operator =(const IFramebuffer& source) = delete;

	};

	typedef SmartRefCount<IFramebuffer> IFramebufferPtr;
} // namespace Rhi