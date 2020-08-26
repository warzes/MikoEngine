#pragma once

#include "Utility/StringId.h"
#include "Core/GetInvalid.h"
#include "RHI/Rhi.h"

namespace Renderer
{
	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t			 ShaderCacheId;	// Shader cache identifier, identical to the shader combination ID
	typedef StringId			 AssetId;		// Asset identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef std::vector<AssetId> AssetIds;


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class ShaderCache final
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class ShaderCacheManager;			// Is creating and managing shader cache instances
		friend class GraphicsPipelineStateCompiler;	// Is creating shader cache instances
		friend class ComputePipelineStateCompiler;	// Is creating shader cache instances


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the shader cache ID
		*
		*  @return
		*    The shader cache ID
		*/
		[[nodiscard]] inline ShaderCacheId getShaderCacheId() const
		{
			return mShaderCacheId;
		}

		/**
		*  @brief
		*    Return master shader cache
		*
		*  @return
		*    The master shader cache, can be a null pointer, don't destroy the instance
		*/
		[[nodiscard]] inline ShaderCache* getMasterShaderCache() const
		{
			return mMasterShaderCache;
		}

		/**
		*  @brief
		*    Return RHI shader bytecode
		*
		*  @return
		*    The RHI shader bytecode
		*/
		[[nodiscard]] inline const Rhi::ShaderBytecode& getShaderBytecode() const
		{
			return mShaderBytecode;
		}

		/**
		*  @brief
		*    Return RHI shader
		*
		*  @return
		*    The RHI shader
		*/
		[[nodiscard]] inline const Rhi::IShaderPtr& getShaderPtr() const
		{
			return (nullptr != mMasterShaderCache) ? mMasterShaderCache->mShaderPtr : mShaderPtr;
		}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline ShaderCache() :
			mShaderCacheId(GetInvalid<ShaderCacheId>()),
			mMasterShaderCache(nullptr)
		{
			// Nothing here
		}

		inline explicit ShaderCache(ShaderCacheId shaderCacheId) :
			mShaderCacheId(shaderCacheId),
			mMasterShaderCache(nullptr)
		{
			// Nothing here
		}

		inline ShaderCache(ShaderCacheId shaderCacheId, ShaderCache* masterShaderCache) :
			mShaderCacheId(shaderCacheId),
			mMasterShaderCache(masterShaderCache)
		{
			// Nothing here
		}

		inline ~ShaderCache()
		{
			// Nothing here
		}

		explicit ShaderCache(const ShaderCache&) = delete;
		ShaderCache& operator=(const ShaderCache&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ShaderCacheId		mShaderCacheId;
		ShaderCache*		mMasterShaderCache;			// If there's a master shader cache instance, we don't own the references shader but only redirect to it (multiple shader combinations resulting in same shader source code topic), don't destroy the instance
		AssetIds			mAssetIds;					// List of IDs of the assets (shader blueprint, shader piece) which took part in the shader cache creation
		uint64_t			mCombinedAssetFileHashes;	// Combination of the file hash of all assets (shader blueprint, shader piece) which took part in the shader cache creation
		Rhi::ShaderBytecode mShaderBytecode;
		Rhi::IShaderPtr		mShaderPtr;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
