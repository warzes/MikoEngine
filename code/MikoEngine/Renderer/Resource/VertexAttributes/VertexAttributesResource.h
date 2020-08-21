#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/IResource.h"

#include "Rhi/Rhi.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Renderer
{
	template <class ELEMENT_TYPE, typename ID_TYPE, uint32_t MAXIMUM_NUMBER_OF_ELEMENTS> class PackedElementManager;
	template <class TYPE, class LOADER_TYPE, typename ID_TYPE, uint32_t MAXIMUM_NUMBER_OF_ELEMENTS> class ResourceManagerTemplate;
	class VertexAttributesResourceLoader;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t VertexAttributesResourceId;	// POD vertex attributes resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Vertex attributes resource
	*/
	class VertexAttributesResource final : public IResource
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class VertexAttributesResourceLoader;
		friend PackedElementManager<VertexAttributesResource, VertexAttributesResourceId, 32>;										// Type definition of template class
		friend ResourceManagerTemplate<VertexAttributesResource, VertexAttributesResourceLoader, VertexAttributesResourceId, 32>;	// Type definition of template class


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the vertex attributes
		*
		*  @return
		*    The vertex attributes
		*/
		[[nodiscard]] inline const Rhi::VertexAttributes& getVertexAttributes() const
		{
			return mVertexAttributes;
		}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline VertexAttributesResource()
		{
			// Nothing here
		}

		inline virtual ~VertexAttributesResource() override
		{
			// Nothing here
		}

		explicit VertexAttributesResource(const VertexAttributesResource&) = delete;
		VertexAttributesResource& operator=(const VertexAttributesResource&) = delete;

		//[-------------------------------------------------------]
		//[ "Renderer::PackedElementManager" management           ]
		//[-------------------------------------------------------]
		inline void initializeElement(VertexAttributesResourceId vertexAttributesResourceId)
		{
			// Sanity checks
			// TODO(co)

			// Call base implementation
			IResource::initializeElement(vertexAttributesResourceId);
		}

		inline void deinitializeElement()
		{
			// TOOD(co)

			// Call base implementation
			IResource::deinitializeElement();
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Rhi::VertexAttributes mVertexAttributes;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
