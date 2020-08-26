#include "Mesh.h"
#include "MikoEngine/Core/GetInvalid.h"
#include "MikoEngine/Renderer/IRenderer.h"
#include "MikoEngine/Renderer/Core/File/PhysicsFSFileManager.h"
#include "MikoEngine/Renderer/Core/RemoteryProfiler.h"
#include "MikoEngine/Renderer/Asset/AssetManager.h"
#include "MikoEngine/Renderer/Resource/MaterialBlueprint/MaterialBlueprintResourceManager.h"
#include "MikoEngine/Renderer/Resource/Mesh/MeshResourceManager.h"
#include "MikoEngine/Renderer/Resource/Texture/TextureResourceManager.h"
#include "MikoEngine/Renderer/Resource/Mesh/MeshResource.h"
#include "MikoEngine/Renderer/Resource/Texture/TextureResource.h"

Mesh::Mesh()
{

}

bool Mesh::init(int argc, const char * argv[])
{
	// Create the renderer instance
#ifdef __ANDROID__
	struct android_app androidApp;	// TODO(co) Get Android app instance
	RHI_ASSERT(rhi->getContext(), nullptr != androidApp.activity->assetManager, "Invalid Android asset manager instance")
		mFileManager = new Renderer::AndroidFileManager(rhi->getContext().getLog(), rhi->getContext().getAssert(), rhi->getContext().getAllocator(), std_filesystem::canonical(std_filesystem::current_path() / "..").generic_string(), *androidApp.activity->assetManager);
#else
	mFileManager = new Renderer::PhysicsFSFileManager(std::filesystem::canonical(std::filesystem::current_path() / "..").generic_string());
#endif
#if defined(RENDERER_GRAPHICS_DEBUGGER) && defined(RENDERER_PROFILER)
	mProfiler = new Renderer::RemoteryProfiler(*rhi);
	mRendererContext = new Renderer::Context(*rhi, *mFileManager, *mGraphicsDebugger, *mProfiler);
#elif defined RENDERER_GRAPHICS_DEBUGGER
	mRendererContext = new Renderer::Context(*rhi, *mFileManager, *mGraphicsDebugger);
#elif defined RENDERER_PROFILER
	mProfiler = new Renderer::RemoteryProfiler(*rhi);
	mRendererContext = new Renderer::Context(*rhi, *mFileManager, *mProfiler);
#else
	mRendererContext = new Renderer::Context(*rhi, *mFileManager);
#endif
	mRendererInstance = new Renderer::RendererInstance(*mRendererContext);

	{
		Renderer::IRenderer* renderer = getRenderer();
		if ( nullptr != renderer )
		{
			// Mount asset package
			bool mountAssetPackageResult = false;
			Renderer::AssetManager& assetManager = renderer->getAssetManager();
			bool rhiIsOpenGLES = (rhi->getNameId() == Rhi::NameId::OPENGLES3);
			if ( rhiIsOpenGLES )
			{
				// Handy fallback for development: If the mobile data isn't there, use the PC data
				mountAssetPackageResult = (assetManager.mountAssetPackage("../DataMobile/Example/Content", "Example") != nullptr);
				if ( !mountAssetPackageResult )
				{
					RHI_LOG(COMPATIBILITY_WARNING, "The examples application failed to find \"../DataMobile/Example/Content\", using \"../DataPc/Example/Content\" as fallback");
					mountAssetPackageResult = (assetManager.mountAssetPackage("../DataPc/Example/Content", "Example") != nullptr);
					rhiIsOpenGLES = false;
				}
			}
			else
			{
				mountAssetPackageResult = (assetManager.mountAssetPackage("../DataPc/Example/Content", "Example") != nullptr);
			}
			if ( !mountAssetPackageResult )
			{
				// Error!
				//showUrgentMessage("Please start \"ExampleProjectCompiler\" before starting \"Examples\" for the first time");
				//deinitialization();
				return false;
			}
			renderer->loadPipelineStateObjectCache();

			// Load renderer toolkit project to enable hot-reloading in case of asset changes
#ifdef RENDERER_TOOLKIT
			{
				RendererToolkit::IRendererToolkit* rendererToolkit = getRendererToolkit();
				if ( nullptr != rendererToolkit )
				{
					// The renderer toolkit project startup is done inside a background thread to not block the main thread
					mRendererToolkitProjectStartupThread = std::thread(&IApplicationRenderer::rendererToolkitProjectStartupThreadWorker, this, renderer, rendererToolkit, rhiIsOpenGLES);
				}
			}
#endif
		}
	}



	mMeshResourceId = GetInvalid<Renderer::MeshResourceId>();
	m_argb_nxaTextureResourceId = GetInvalid<Renderer::TextureResourceId>();
	m_hr_rg_mb_nyaTextureResourceId = GetInvalid<Renderer::TextureResourceId>();
	mEmissiveTextureResourceId = GetInvalid<Renderer::TextureResourceId>();
	mObjectSpaceToClipSpaceMatrixUniformHandle = NULL_HANDLE;
	mObjectSpaceToViewSpaceMatrixUniformHandle = NULL_HANDLE;
	mGlobalTimer = 0.0f;

	// Get and check the renderer instance
	Renderer::IRenderer& renderer = *getRenderer();

	// Don't create initial pipeline state caches after a material blueprint has been loaded since this example isn't using the material blueprint system
	renderer.getMaterialBlueprintResourceManager().setCreateInitialPipelineStateCaches(false);

	{
		{ // Create the root signature
			Rhi::DescriptorRangeBuilder ranges[5];
			ranges[0].initialize(Rhi::ResourceType::UNIFORM_BUFFER, 0, "UniformBlockDynamicVs", Rhi::ShaderVisibility::VERTEX);
			ranges[1].initialize(Rhi::ResourceType::TEXTURE_2D, 0, "_argb_nxa", Rhi::ShaderVisibility::FRAGMENT);
			ranges[2].initialize(Rhi::ResourceType::TEXTURE_2D, 1, "_hr_rg_mb_nya", Rhi::ShaderVisibility::FRAGMENT);
			ranges[3].initialize(Rhi::ResourceType::TEXTURE_2D, 2, "EmissiveMap", Rhi::ShaderVisibility::FRAGMENT);
			ranges[4].initializeSampler(0, Rhi::ShaderVisibility::FRAGMENT);

			Rhi::RootParameterBuilder rootParameters[2];
			rootParameters[0].initializeAsDescriptorTable(4, &ranges[0]);
			rootParameters[1].initializeAsDescriptorTable(1, &ranges[4]);

			// Setup
			Rhi::RootSignatureBuilder rootSignatureBuilder;
			rootSignatureBuilder.initialize(static_cast<uint32_t>(GLM_COUNTOF(rootParameters)), rootParameters, 0, nullptr, Rhi::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			// Create the instance
			mRootSignature = rhi->createRootSignature(rootSignatureBuilder);
		}

		// Create uniform buffer
		// -> Direct3D 11 and 12 do not support individual uniforms
		// -> The RHI is just a light weight abstraction layer, so we need to handle the differences
		// -> Allocate enough memory for two 4x4 floating point matrices
		if ( 0 != rhi->getCapabilities().maximumUniformBufferSize )
		{
			mUniformBuffer = renderer.getBufferManager().createUniformBuffer(2 * 4 * 4 * sizeof(float), nullptr, Rhi::BufferUsage::DYNAMIC_DRAW);
		}

		// Vertex input layout
		static constexpr Rhi::VertexAttribute vertexAttributesLayout[] =
		{
			{ // Attribute 0
				// Data destination
				Rhi::VertexAttributeFormat::FLOAT_3,		// vertexAttributeFormat (Rhi::VertexAttributeFormat)
				"Position",									// name[32] (char)
				"POSITION",									// semanticName[32] (char)
				0,											// semanticIndex (uint32_t)
				// Data source
				0,											// inputSlot (uint32_t)
				0,											// alignedByteOffset (uint32_t)
				sizeof(float) * 5 + sizeof(short) * 4,		// strideInBytes (uint32_t)
				0											// instancesPerElement (uint32_t)
			},
			{ // Attribute 1
				// Data destination
				Rhi::VertexAttributeFormat::FLOAT_2,		// vertexAttributeFormat (Rhi::VertexAttributeFormat)
				"TexCoord",									// name[32] (char)
				"TEXCOORD",									// semanticName[32] (char)
				0,											// semanticIndex (uint32_t)
				// Data source
				0,											// inputSlot (uint32_t)
				sizeof(float) * 3,							// alignedByteOffset (uint32_t)
				sizeof(float) * 5 + sizeof(short) * 4,		// strideInBytes (uint32_t)
				0											// instancesPerElement (uint32_t)
			},
			{ // Attribute 2
				// Data destination
				Rhi::VertexAttributeFormat::SHORT_4,		// vertexAttributeFormat (Rhi::VertexAttributeFormat)
				"QTangent",									// name[32] (char)
				"TEXCOORD",									// semanticName[32] (char)
				1,											// semanticIndex (uint32_t)
				// Data source
				0,											// inputSlot (uint32_t)
				sizeof(float) * 5,							// alignedByteOffset (uint32_t)
				sizeof(float) * 5 + sizeof(short) * 4,		// strideInBytes (uint32_t)
				0											// instancesPerElement (uint32_t)
			}
		};
		const Rhi::VertexAttributes vertexAttributes(static_cast<uint32_t>(GLM_COUNTOF(vertexAttributesLayout)), vertexAttributesLayout);

		{ // Create sampler state and wrap it into a resource group instance
			Rhi::SamplerState samplerStateSettings = Rhi::ISamplerState::getDefaultSamplerState();
			samplerStateSettings.addressU = Rhi::TextureAddressMode::WRAP;
			samplerStateSettings.addressV = Rhi::TextureAddressMode::WRAP;
			Rhi::IResource* samplerStateResource = mSamplerStatePtr = rhi->createSamplerState(samplerStateSettings);
			mSamplerStateGroup = mRootSignature->createResourceGroup(1, 1, &samplerStateResource);
		}

		{ // Create the graphics program
			// Get the shader source code (outsourced to keep an overview)
			const char* vertexShaderSourceCode = nullptr;
			const char* fragmentShaderSourceCode = nullptr;
#include "Mesh_GLSL_450.h"	// For Vulkan
#include "Mesh_GLSL_410.h"	// macOS 10.11 only supports OpenGL 4.1 hence it's our OpenGL minimum
#include "Mesh_GLSL_ES3.h"
#include "Mesh_HLSL_D3D11_D3D12.h"
#include "Mesh_Null.h"

			// Create the graphics program
			Rhi::IShaderLanguage& shaderLanguage = rhi->getDefaultShaderLanguage();
			mGraphicsProgram = shaderLanguage.createGraphicsProgram(
				*mRootSignature,
				vertexAttributes,
				shaderLanguage.createVertexShaderFromSourceCode(vertexAttributes, vertexShaderSourceCode),
				shaderLanguage.createFragmentShaderFromSourceCode(fragmentShaderSourceCode));
		}

		// Is there a valid graphics program?
		if ( nullptr != mGraphicsProgram )
		{
			// Create the graphics pipeline state object (PSO)
			mGraphicsPipelineState = rhi->createGraphicsPipelineState(Rhi::GraphicsPipelineStateBuilder(mRootSignature, mGraphicsProgram, vertexAttributes, mainSwapChain->getRenderPass()));

			// Optimization: Cached data to not bother the RHI too much
			if ( nullptr == mUniformBuffer )
			{
				mObjectSpaceToClipSpaceMatrixUniformHandle = mGraphicsProgram->getUniformHandle("ObjectSpaceToClipSpaceMatrix");
				mObjectSpaceToViewSpaceMatrixUniformHandle = mGraphicsProgram->getUniformHandle("ObjectSpaceToViewSpaceMatrix");
			}
		}

		// Create mesh instance
		//renderer.getMeshResourceManager().loadMeshResourceByAssetId(ASSET_ID("Example/Mesh/Imrod/SM_Imrod"), mMeshResourceId, this);

		//{ // Load in the albedo, emissive, normal and roughness texture
		//	Renderer::TextureResourceManager& textureResourceManager = renderer.getTextureResourceManager();
		//	textureResourceManager.loadTextureResourceByAssetId(ASSET_ID("Example/Mesh/Imrod/T_Imrod_argb_nxa"), ASSET_ID("Engine/Texture/DynamicByCode/Identity_argb_nxa2D"), m_argb_nxaTextureResourceId, this);
		//	textureResourceManager.loadTextureResourceByAssetId(ASSET_ID("Example/Mesh/Imrod/T_Imrod_hr_rg_mb_nya"), ASSET_ID("Engine/Texture/DynamicByCode/Identity_hr_rg_mb_nya2D"), m_hr_rg_mb_nyaTextureResourceId, this);
		//	textureResourceManager.loadTextureResourceByAssetId(ASSET_ID("Example/Mesh/Imrod/T_Imrod_e"), ASSET_ID("Engine/Texture/DynamicByCode/IdentityEmissiveMap2D"), mEmissiveTextureResourceId, this);
		//}

		renderer.getMeshResourceManager().loadMeshResourceByAssetId(ASSET_ID("Example/Mesh/Cerberus/SM_Cerberus"), mMeshResourceId, this);

		{ // Load in the albedo, emissive, normal and roughness texture
			Renderer::TextureResourceManager& textureResourceManager = renderer.getTextureResourceManager();
			textureResourceManager.loadTextureResourceByAssetId(ASSET_ID("Example/Mesh/Cerberus/T_Cerberus_argb_nxa"), ASSET_ID("Engine/Texture/DynamicByCode/Identity_argb_nxa2D"), m_argb_nxaTextureResourceId, this);
			textureResourceManager.loadTextureResourceByAssetId(ASSET_ID("Example/Mesh/Cerberus/T_Cerberus_hr_rg_mb_nya"), ASSET_ID("Engine/Texture/DynamicByCode/Identity_hr_rg_mb_nya2D"), m_hr_rg_mb_nyaTextureResourceId, this);
			textureResourceManager.loadTextureResourceByAssetId(ASSET_ID("Example/Mesh/Imrod/T_Imrod_e"), ASSET_ID("Engine/Texture/DynamicByCode/IdentityEmissiveMap2D"), mEmissiveTextureResourceId, this);
		}
	}

	// Since we're always submitting the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
	fillCommandBuffer();


	return true;
}

void Mesh::update(double delta)
{
	Renderer::IRenderer* renderer = getRenderer();
	if ( nullptr != renderer )
	{
		renderer->update();
	}
	// Stop the stopwatch
	mStopwatch.stop();

	// Update the global timer (FPS independent movement)
	mGlobalTimer += mStopwatch.getMilliseconds() * 0.0005f;

	// Start the stopwatch
	mStopwatch.start();

	{ // Set uniform
			// Get the aspect ratio
		float aspectRatio = 4.0f / 3.0f;
		{
			// Get the render target with and height
			const Rhi::IRenderTarget* renderTarget = mainSwapChain;
			if ( nullptr != renderTarget )
			{
				uint32_t width = 1;
				uint32_t height = 1;
				renderTarget->getWidthAndHeight(width, height);

				// Get the aspect ratio
				aspectRatio = static_cast<float>(width) / height;
			}
		}

		// Calculate the object space to clip space matrix
		const glm::mat4 viewSpaceToClipSpace = glm::perspective(45.0f, aspectRatio, 1000.0f, 0.1f);	// Near and far flipped due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
		const glm::mat4 viewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -7.0f, 50.0f));
		const glm::mat4 worldSpaceToViewSpace = glm::rotate(viewTranslate, mGlobalTimer, glm::vec3(0.0f, 1.0f, 0.0f));
		const glm::mat4 objectSpaceToWorldSpace = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
		glm::mat4 objectSpaceToViewSpace = worldSpaceToViewSpace * objectSpaceToWorldSpace;
		const glm::mat4 objectSpaceToClipSpace = viewSpaceToClipSpace * objectSpaceToViewSpace;

		// Upload the uniform data
		// -> Two versions: One using an uniform buffer and one setting an individual uniform
		if ( nullptr != mUniformBuffer )
		{
			struct UniformBlockDynamicVs final
			{
				float objectSpaceToClipSpaceMatrix[4 * 4];	// Object space to clip space matrix
				float objectSpaceToViewSpaceMatrix[4 * 4];	// Object space to view space matrix
			};
			UniformBlockDynamicVs uniformBlockDynamicVS;
			memcpy(uniformBlockDynamicVS.objectSpaceToClipSpaceMatrix, glm::value_ptr(objectSpaceToClipSpace), sizeof(float) * 4 * 4);

			// TODO(co) float3x3 (currently there are alignment issues when using Direct3D, have a look into possible solutions)
			glm::mat3 objectSpaceToViewSpace3x3 = glm::mat3(objectSpaceToViewSpace);
			objectSpaceToViewSpace = glm::mat4(objectSpaceToViewSpace3x3);
			memcpy(uniformBlockDynamicVS.objectSpaceToViewSpaceMatrix, glm::value_ptr(objectSpaceToViewSpace), sizeof(float) * 4 * 4);

			// Copy data
			Rhi::MappedSubresource mappedSubresource;
			if ( rhi->map(*mUniformBuffer, 0, Rhi::MapType::WRITE_DISCARD, 0, mappedSubresource) )
			{
				memcpy(mappedSubresource.data, &uniformBlockDynamicVS, sizeof(UniformBlockDynamicVs));
				rhi->unmap(*mUniformBuffer, 0);
			}
		}
		else
		{
			// TODO(co) Not compatible with command buffer: This certainly is going to be removed, we need to implement internal uniform buffer emulation
			// Set uniforms
			mGraphicsProgram->setUniformMatrix4fv(mObjectSpaceToClipSpaceMatrixUniformHandle, glm::value_ptr(objectSpaceToClipSpace));
			mGraphicsProgram->setUniformMatrix3fv(mObjectSpaceToViewSpaceMatrixUniformHandle, glm::value_ptr(glm::mat3(objectSpaceToViewSpace)));
		}
	}

	// Submit command buffer to the RHI implementation, in case the referenced assets have already been loaded and the command buffer has been filled as a consequence
	if ( !mCommandBuffer.isEmpty() )
	{
		mCommandBuffer.submitToRhi(*rhi);
	}
}

void Mesh::shutdown()
{
	{ // Disconnect resource listeners
		const Renderer::IRenderer& renderer = *getRenderer();
		const Renderer::TextureResourceManager& textureResourceManager = renderer.getTextureResourceManager();
		textureResourceManager.setInvalidResourceId(m_argb_nxaTextureResourceId, *this);
		textureResourceManager.setInvalidResourceId(m_hr_rg_mb_nyaTextureResourceId, *this);
		textureResourceManager.setInvalidResourceId(mEmissiveTextureResourceId, *this);
		renderer.getMeshResourceManager().setInvalidResourceId(mMeshResourceId, *this);
	}

	// Release the used RHI resources
	mObjectSpaceToViewSpaceMatrixUniformHandle = NULL_HANDLE;
	mObjectSpaceToClipSpaceMatrixUniformHandle = NULL_HANDLE;
	mSamplerStateGroup = nullptr;
	mSamplerStatePtr = nullptr;
	mResourceGroup = nullptr;
	mGraphicsProgram = nullptr;
	mGraphicsPipelineState = nullptr;
	mUniformBuffer = nullptr;
	mRootSignature = nullptr;
	mCommandBuffer.clear();



	delete mRendererInstance;
	mRendererInstance = nullptr;
	delete mRendererContext;
	mRendererContext = nullptr;
#ifdef RENDERER_PROFILER
	delete static_cast<Renderer::RemoteryProfiler*>(mProfiler);
#endif
#ifdef __ANDROID__
	delete static_cast<Renderer::AndroidFileManager*>(mFileManager);
#else
	delete static_cast<Renderer::PhysicsFSFileManager*>(mFileManager);
#endif
	mFileManager = nullptr;
#ifdef RENDERER_TOOLKIT
	{
		mRendererToolkitProjectStartupThread.join();
		std::lock_guard<std::mutex> projectMutexLock(mProjectMutex);
		if ( nullptr != mProject )
		{
			delete mProject;
			mProject = nullptr;
		}
	}
	if ( nullptr != mRendererToolkitInstance )
	{
		delete mRendererToolkitInstance;
		mRendererToolkitInstance = nullptr;
	}
	if ( nullptr != mRendererToolkitContext )
	{
		delete mRendererToolkitContext;
		mRendererToolkitContext = nullptr;
	}
	if ( nullptr != mRendererToolkitFileManager )
	{
		delete static_cast<Renderer::DefaultFileManager*>(mRendererToolkitFileManager);
		mRendererToolkitFileManager = nullptr;
	}
#endif
}

ApplicationSetting Mesh::intial_app_settings()
{
	ApplicationSetting settings;
	settings.width = 1280;
	settings.height = 720;
	settings.title = "Mesh Example";
	settings.vsync = false;
	return settings;
}

void Mesh::window_resized(int width, int height)
{
}

void Mesh::fillCommandBuffer()
{
	const Renderer::IRenderer& renderer = *getRenderer();
	const Renderer::MeshResource* meshResource = renderer.getMeshResourceManager().tryGetById(mMeshResourceId);
	if ( nullptr != meshResource && nullptr != meshResource->getVertexArrayPtr() )
	{
		// Due to background texture loading, some textures might not be ready yet resulting in fallback texture usage
		// -> "Mesh::onLoadingStateChange()" will invalidate the resource group as soon as a texture resource finishes loading 
		if ( nullptr == mResourceGroup )
		{
			const Renderer::TextureResourceManager& textureResourceManager = renderer.getTextureResourceManager();
			const Renderer::TextureResource* _argb_nxaTextureResource = textureResourceManager.tryGetById(m_argb_nxaTextureResourceId);
			const Renderer::TextureResource* _hr_rg_mb_nyaTextureResource = textureResourceManager.tryGetById(m_hr_rg_mb_nyaTextureResourceId);
			const Renderer::TextureResource* emissiveTextureResource = textureResourceManager.tryGetById(mEmissiveTextureResourceId);
			if ( nullptr == _argb_nxaTextureResource || nullptr == _argb_nxaTextureResource->getTexturePtr() ||
				nullptr == _hr_rg_mb_nyaTextureResource || nullptr == _hr_rg_mb_nyaTextureResource->getTexturePtr() ||
				nullptr == emissiveTextureResource || nullptr == emissiveTextureResource->getTexturePtr() )
			{
				return;
			}

			// Create resource group
			Rhi::IResource* resources[4] = { mUniformBuffer, _argb_nxaTextureResource->getTexturePtr(), _hr_rg_mb_nyaTextureResource->getTexturePtr(), emissiveTextureResource->getTexturePtr() };
			Rhi::ISamplerState* samplerStates[4] = { nullptr, mSamplerStatePtr, mSamplerStatePtr, mSamplerStatePtr };
			mResourceGroup = mRootSignature->createResourceGroup(0, static_cast<uint32_t>(GLM_COUNTOF(resources)), resources, samplerStates);
		}

		// Get and check the RHI instance

		if ( nullptr != rhi && nullptr != mGraphicsPipelineState )
		{
			// Combined scoped profiler CPU and GPU sample as well as renderer debug event command
			RENDERER_SCOPED_PROFILER_EVENT(renderer.getContext(), mCommandBuffer, "Mesh");

			// Clear the graphics color buffer of the current render target with gray, do also clear the depth buffer
			const float color[4] = { 0.6f, 0.8f, 1.0f, 1.0f };
			Rhi::Command::ClearGraphics::create(mCommandBuffer, Rhi::ClearFlag::COLOR_DEPTH, color);

			// Set the used graphics root signature
			Rhi::Command::SetGraphicsRootSignature::create(mCommandBuffer, mRootSignature);

			// Set the used graphics pipeline state object (PSO)
			Rhi::Command::SetGraphicsPipelineState::create(mCommandBuffer, mGraphicsPipelineState);

			// Set graphics resource groups
			Rhi::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 0, mResourceGroup);
			Rhi::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 1, mSamplerStateGroup);

			{ // Draw mesh instance
				// Input assembly (IA): Set the used vertex array
				Rhi::Command::SetGraphicsVertexArray::create(mCommandBuffer, meshResource->getVertexArrayPtr());

				// Render the specified geometric primitive, based on indexing into an array of vertices
				Rhi::Command::DrawIndexedGraphics::create(mCommandBuffer, meshResource->getNumberOfIndices());
			}
		}
	}
}

Renderer::IRenderer* Mesh::getRenderer() const
{
	return (nullptr != mRendererInstance) ? mRendererInstance->getRenderer() : nullptr;
}