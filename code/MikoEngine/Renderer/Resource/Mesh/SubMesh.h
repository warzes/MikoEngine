#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Core/GetInvalid.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t MaterialResourceId;	// POD material resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Sub-mesh class
	*/
	class SubMesh final
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline SubMesh() :
			mMaterialResourceId(getInvalid<MaterialResourceId>()),
			mStartIndexLocation(0),
			mNumberOfIndices(0)
		{
			// Nothing here
		}

		inline SubMesh(MaterialResourceId materialResourceId, uint32_t startIndexLocation, uint32_t numberOfIndices) :
			mMaterialResourceId(materialResourceId),
			mStartIndexLocation(startIndexLocation),
			mNumberOfIndices(numberOfIndices)
		{
			// Nothing here
		}

		inline explicit SubMesh(const SubMesh& subMesh) :
			mMaterialResourceId(subMesh.mMaterialResourceId),
			mStartIndexLocation(subMesh.mStartIndexLocation),
			mNumberOfIndices(subMesh.mNumberOfIndices)
		{
			// Nothing here
		}

		inline ~SubMesh()
		{
			// Nothing here
		}

		inline SubMesh& operator=(const SubMesh& subMesh)
		{
			mMaterialResourceId	= subMesh.mMaterialResourceId;
			mStartIndexLocation = subMesh.mStartIndexLocation;
			mNumberOfIndices	= subMesh.mNumberOfIndices;

			// Done
			return *this;
		}

		[[nodiscard]] inline MaterialResourceId getMaterialResourceId() const
		{
			return mMaterialResourceId;
		}

		inline void setMaterialResourceId(MaterialResourceId materialResourceId)
		{
			mMaterialResourceId = materialResourceId;
		}

		[[nodiscard]] inline uint32_t getStartIndexLocation() const
		{
			return mStartIndexLocation;
		}

		inline void setStartIndexLocation(uint32_t startIndexLocation)
		{
			mStartIndexLocation = startIndexLocation;
		}

		[[nodiscard]] inline uint32_t getNumberOfIndices() const
		{
			return mNumberOfIndices;
		}

		inline void setNumberOfIndices(uint32_t numberOfIndices)
		{
			mNumberOfIndices = numberOfIndices;
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MaterialResourceId mMaterialResourceId;	// Material resource ID, can be set to invalid value
		uint32_t		   mStartIndexLocation;
		uint32_t		   mNumberOfIndices;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
