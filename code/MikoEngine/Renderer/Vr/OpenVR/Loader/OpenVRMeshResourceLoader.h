#pragma once

#if RENDERER_OPENVR

//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Resource/Mesh/Loader/IMeshResourceLoader.h"

// Disable warnings in external headers, we can't fix them
SE_PRAGMA_WARNING_PUSH
	SE_PRAGMA_WARNING_DISABLE_MSVC(4127)	// warning C4127: conditional expression is constant
	SE_PRAGMA_WARNING_DISABLE_MSVC(4201)	// warning C4201: nonstandard extension used: nameless struct/union
	SE_PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
	SE_PRAGMA_WARNING_DISABLE_MSVC(4324)	// warning C4324: '<x>': structure was padded due to alignment specifier
	#include <glm/glm.hpp>
SE_PRAGMA_WARNING_POP

// Disable warnings in external headers, we can't fix them
SE_PRAGMA_WARNING_PUSH
	SE_PRAGMA_WARNING_DISABLE_MSVC(4365)	// warning C4365: 'argument': conversion from 'long' to 'unsigned int', signed/unsigned mismatch
	SE_PRAGMA_WARNING_DISABLE_MSVC(4530)	// warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc
	SE_PRAGMA_WARNING_DISABLE_MSVC(5026)	// warning C5026: 'std::_Generic_error_category': move constructor was implicitly defined as deleted
	SE_PRAGMA_WARNING_DISABLE_MSVC(5027)	// warning C5027: 'std::_Generic_error_category': move assignment operator was implicitly defined as deleted
	SE_PRAGMA_WARNING_DISABLE_MSVC(4571)	// warning C4571: Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
	SE_PRAGMA_WARNING_DISABLE_MSVC(4625)	// warning C4625: 'std::codecvt_base': copy constructor was implicitly defined as deleted
	SE_PRAGMA_WARNING_DISABLE_MSVC(4626)	// warning C4626: 'std::codecvt<char16_t,char,_Mbstatet>': assignment operator was implicitly defined as deleted
	SE_PRAGMA_WARNING_DISABLE_MSVC(4668)	// warning C4668: '_M_HYBRID_X86_ARM64' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
	SE_PRAGMA_WARNING_DISABLE_MSVC(4774)	// warning C4774: 'sprintf_s' : format string expected in argument 3 is not a string literal
	#include <string>
	#include <vector>
SE_PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace vr
{
	struct RenderModel_t;
}
namespace Rhi
{
	class IVertexArray;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class OpenVRMeshResourceLoader final : public IMeshResourceLoader
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MeshResourceManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t TYPE_ID = SE_STRING_ID("openvr_mesh");
		typedef std::vector<glm::vec3> BufferData;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline vr::RenderModel_t* getVrRenderModel() const
		{
			return mVrRenderModel;
		}

		[[nodiscard]] inline BufferData& getTangentsData()
		{
			return mTangentsData;
		}

		[[nodiscard]] inline BufferData& getBinormalsData()
		{
			return mBinormalsData;
		}


	//[-------------------------------------------------------]
	//[ Public virtual Renderer::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual ResourceLoaderTypeId getResourceLoaderTypeId() const override
		{
			return TYPE_ID;
		}

		[[nodiscard]] inline virtual bool hasDeserialization() const override
		{
			return false;
		}

		[[nodiscard]] inline virtual bool onDeserialization(IFile&) override
		{
			// We're using the OpenVR API instead of reading from a file

			// Done
			return true;
		}

		[[nodiscard]] inline virtual bool hasProcessing() const override
		{
			return true;
		}

		virtual void onProcessing() override;
		[[nodiscard]] virtual bool onDispatch() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline OpenVRMeshResourceLoader(IResourceManager& resourceManager, IRenderer& renderer) :
			IMeshResourceLoader(resourceManager, renderer),
			mVrRenderModel(nullptr),
			mVertexArray(nullptr)
		{
			// Nothing here
		}

		inline virtual ~OpenVRMeshResourceLoader() override
		{
			// Nothing here
		}

		explicit OpenVRMeshResourceLoader(const OpenVRMeshResourceLoader&) = delete;
		OpenVRMeshResourceLoader& operator=(const OpenVRMeshResourceLoader&) = delete;
		[[nodiscard]] Rhi::IVertexArray* createVertexArray() const;
		[[nodiscard]] const std::string& getRenderModelName() const;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<uint8_t>  VertexBufferData;
		typedef std::vector<uint16_t> IndexBufferData;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Temporary data
		vr::RenderModel_t* mVrRenderModel;
		BufferData		   mTangentsData;
		BufferData		   mBinormalsData;
		VertexBufferData   mVertexBufferData;
		IndexBufferData	   mIndexBufferData;
		Rhi::IVertexArray* mVertexArray;
		glm::vec3		   mMinimumBoundingBoxPosition;
		glm::vec3		   mMaximumBoundingBoxPosition;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer

#endif // RENDERER_OPENVR