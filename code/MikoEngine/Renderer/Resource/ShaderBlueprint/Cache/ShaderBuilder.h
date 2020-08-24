#pragma once

#include "Renderer/Resource/ShaderBlueprint/Cache/ShaderProperties.h"

namespace Renderer
{
	class ShaderBlueprintResource;
	class ShaderPieceResourceManager;
}

namespace Renderer
{
	typedef StringId						AssetId;				// Asset identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef std::vector<AssetId>			AssetIds;
	typedef std::map<uint32_t, std::string> DynamicShaderPieces;	// Key is "Renderer::StringId"	// TODO(co) Visual Studio 2017: "std::unordered_map" appears to have an inefficient assignment operator which does memory handling even if containers are empty all the time, "std::map" isn't the most effective structure either but currently still better

	/**
	*  @brief
	*    Shader builder
	*
	*  @note
	*   - Heavily basing on the OGRE 2.1 HLMS shader builder which is directly part of the OGRE class "Ogre::Hlms". So for syntax, have a look into the OGRE 2.1 documentation.
	*/
	class ShaderBuilder final
	{
	public:
		struct BuildShader final
		{
			std::string sourceCode;
			AssetIds	assetIds;						// List of IDs of the assets (shader blueprint, shader piece) which took part in the shader cache creation
			uint64_t	combinedAssetFileHashes = 0;	// Combination of the file hash of all assets (shader blueprint, shader piece) which took part in the shader cache creation
		};
	public:
		ShaderBuilder() = default;
		~ShaderBuilder() = default;

		/**
		*  @brief
		*    Create shader source code by using the given shader blueprint and shader properties
		*
		*  @param[in] shaderPieceResourceManager
		*    Shader piece resource manager to use
		*  @param[in] shaderBlueprintResource
		*    Shader blueprint resource to use
		*  @param[in] shaderProperties
		*    Shader properties to use
		*  @param[out] buildShader
		*    Receives the build shader
		*/
		void createSourceCode(const ShaderPieceResourceManager& shaderPieceResourceManager, const ShaderBlueprintResource& shaderBlueprintResource, const ShaderProperties& shaderProperties, BuildShader& buildShader);

	private:
		explicit ShaderBuilder(const ShaderBuilder&) = delete;
		ShaderBuilder& operator=(const ShaderBuilder&) = delete;
		bool parseMath(const std::string& inBuffer, std::string& outBuffer);
		bool parseForEach(const std::string& inBuffer, std::string& outBuffer) const;
		bool parseProperties(std::string& inBuffer, std::string& outBuffer) const;
		bool collectPieces(const std::string& inBuffer, std::string& outBuffer);
		bool insertPieces(std::string& inBuffer, std::string& outBuffer) const;
		bool parseCounter(const std::string& inBuffer, std::string& outBuffer);
		bool parse(const std::string& inBuffer, std::string& outBuffer) const;

		ShaderProperties	mShaderProperties;
		DynamicShaderPieces	mDynamicShaderPieces;
		std::string			mInString;	// Could be a local variable, but when making it to a member we reduce memory allocations
		std::string			mOutString;	// Could be a local variable, but when making it to a member we reduce memory allocations
	};
} // Renderer
