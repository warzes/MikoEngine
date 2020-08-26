#include "CubeTexture.h"

bool CubeTexture::init(int argc, const char * argv[])
{
	const bool textureCubeArraySupported = (rhi->getCapabilities().maximumNumberOfCubeTextureArraySlices > 0);

	// Create the buffer and texture manager
	mBufferManager = rhi->createBufferManager();
	mTextureManager = rhi->createTextureManager();

	{ // Create the root signature
		Rhi::DescriptorRangeBuilder ranges[4];
		ranges[0].initialize(Rhi::ResourceType::UNIFORM_BUFFER, 0, "UniformBlockDynamicVs", Rhi::ShaderVisibility::VERTEX);
		if ( textureCubeArraySupported )
		{
			ranges[1].initialize(Rhi::ResourceType::TEXTURE_CUBE_ARRAY, 0, "CubeMapArray", Rhi::ShaderVisibility::FRAGMENT);
		}
		else
		{
			ranges[1].initialize(Rhi::ResourceType::TEXTURE_CUBE, 0, "CubeMap", Rhi::ShaderVisibility::FRAGMENT);
		}
		ranges[2].initializeSampler(0, Rhi::ShaderVisibility::FRAGMENT);
		ranges[3].initializeSampler(1, Rhi::ShaderVisibility::FRAGMENT);

		Rhi::RootParameterBuilder rootParameters[2];
		rootParameters[0].initializeAsDescriptorTable(2, &ranges[0]);
		rootParameters[1].initializeAsDescriptorTable(2, &ranges[2]);

		// Setup
		Rhi::RootSignatureBuilder rootSignatureBuilder;
		rootSignatureBuilder.initialize(static_cast<uint32_t>(GLM_COUNTOF(rootParameters)), rootParameters, 0, nullptr, Rhi::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		// Create the instance
		mRootSignature = rhi->createRootSignature(rootSignatureBuilder);
	}

	// Create uniform buffer
	// -> Direct3D 11 and 12 do not support individual uniforms
	// -> The RHI is just a light weight abstraction layer, so we need to handle the differences
	// -> Allocate enough memory for one 4x4 floating point matrix
	if ( 0 != rhi->getCapabilities().maximumUniformBufferSize )
	{
		mUniformBuffer = mBufferManager->createUniformBuffer(4 * 4 * sizeof(float), nullptr, Rhi::BufferUsage::DYNAMIC_DRAW);
	}

	// Create sampler state and wrap it into a resource group instance
	Rhi::ISamplerState* pointSamplerResource = nullptr;
	{
		// Create the sampler resources
		Rhi::SamplerState samplerState = Rhi::ISamplerState::getDefaultSamplerState();
		samplerState.addressU = Rhi::TextureAddressMode::WRAP;
		samplerState.addressV = Rhi::TextureAddressMode::WRAP;
		samplerState.addressW = Rhi::TextureAddressMode::WRAP;
		samplerState.filter = Rhi::FilterMode::MIN_MAG_MIP_POINT;
		pointSamplerResource = rhi->createSamplerState(samplerState);

		// Create the resource group
		Rhi::IResource* resources[1] = { pointSamplerResource };
		mSamplerStateGroup = mRootSignature->createResourceGroup(1, static_cast<uint32_t>(GLM_COUNTOF(resources)), resources);
	}

	{ // Create the texture group
		Rhi::IResource* resources[2];
		resources[0] = mUniformBuffer;

		{ // Create the cube texture
			static constexpr uint32_t TEXTURE_WIDTH = 1;
			static constexpr uint32_t TEXEL_ELEMENTS = 4;
			static constexpr uint32_t NUMBER_OF_BYTES_PER_SLICE = TEXTURE_WIDTH * TEXTURE_WIDTH * TEXEL_ELEMENTS * 6;
			static constexpr uint32_t NUMBER_OF_BYTES = NUMBER_OF_BYTES_PER_SLICE * 2;
			uint8_t data[NUMBER_OF_BYTES] = {};
			uint8_t* currentData = data + NUMBER_OF_BYTES_PER_SLICE;	// The first cube slice will be black

			// Face 0 = positive X = red
			currentData[0] = 255;
			currentData[1] = 0;
			currentData[2] = 0;
			currentData[3] = 0;
			currentData += TEXEL_ELEMENTS;

			// Face 1 = negative X = dark red
			currentData[0] = 127;
			currentData[1] = 0;
			currentData[2] = 0;
			currentData[3] = 0;
			currentData += TEXEL_ELEMENTS;

			// Face 2 = positive Y = green
			currentData[0] = 0;
			currentData[1] = 255;
			currentData[2] = 0;
			currentData[3] = 0;
			currentData += TEXEL_ELEMENTS;

			// Face 3 = negative Y = dark green
			currentData[0] = 0;
			currentData[1] = 127;
			currentData[2] = 0;
			currentData[3] = 0;
			currentData += TEXEL_ELEMENTS;

			// Face 4 = positive Z = blue
			currentData[0] = 0;
			currentData[1] = 0;
			currentData[2] = 255;
			currentData[3] = 0;
			currentData += TEXEL_ELEMENTS;

			// Face 5 = negative Z = dark blue
			currentData[0] = 0;
			currentData[1] = 0;
			currentData[2] = 127;
			currentData[3] = 0;

			// Create the texture instance
			if ( textureCubeArraySupported )
			{
				resources[1] = mTextureManager->createTextureCubeArray(TEXTURE_WIDTH, 2, Rhi::TextureFormat::R8G8B8A8, data, Rhi::TextureFlag::SHADER_RESOURCE);
			}
			else
			{
				resources[1] = mTextureManager->createTextureCube(TEXTURE_WIDTH, Rhi::TextureFormat::R8G8B8A8, data + NUMBER_OF_BYTES_PER_SLICE, Rhi::TextureFlag::SHADER_RESOURCE);
			}
		}

		// Create the texture group
		Rhi::ISamplerState* samplerStates[2] = { nullptr, pointSamplerResource };
		mTextureGroup = mRootSignature->createResourceGroup(0, static_cast<uint32_t>(GLM_COUNTOF(resources)), resources, samplerStates);
	}

	// Vertex input layout
	static constexpr Rhi::VertexAttribute vertexAttributesLayout[] =
	{
		{ // Attribute 0
			// Data destination
			Rhi::VertexAttributeFormat::FLOAT_3,	// vertexAttributeFormat (Rhi::VertexAttributeFormat)
			"Position",								// name[32] (char)
			"POSITION",								// semanticName[32] (char)
			0,										// semanticIndex (uint32_t)
			// Data source
			0,										// inputSlot (uint32_t)
			0,										// alignedByteOffset (uint32_t)
			sizeof(float) * 3,						// strideInBytes (uint32_t)
			0										// instancesPerElement (uint32_t)
		}
	};
	const Rhi::VertexAttributes vertexAttributes(static_cast<uint32_t>(GLM_COUNTOF(vertexAttributesLayout)), vertexAttributesLayout);

	{ // Create vertex array object (VAO)
		// Our cube is constructed like this
		/*
				3+------+2  y
				/|     /|   |
			   / |    / |   |
			  / 0+---/--+1  *---x
			7+------+6 /   /
			 | /    | /   z
			 |/     |/
			4+------+5
		*/

		// Create the vertex buffer object (VBO)
		// -> Object space vertex positions
		static constexpr float VERTEX_POSITION[] =
		{
			-0.5f, -0.5f, -0.5f,	// 0
			 0.5f, -0.5f, -0.5f,	// 1
			 0.5f,  0.5f, -0.5f,	// 2
			-0.5f,  0.5f, -0.5f,	// 3
			-0.5f, -0.5f,  0.5f,	// 4
			 0.5f, -0.5f,  0.5f,	// 5
			 0.5f,  0.5f,  0.5f,	// 6
			-0.5f,  0.5f,  0.5f,	// 7
		};
		Rhi::IVertexBufferPtr vertexBuffer(mBufferManager->createVertexBuffer(sizeof(VERTEX_POSITION), VERTEX_POSITION, 0, Rhi::BufferUsage::STATIC_DRAW));

		// Create the index buffer object (IBO)
		static constexpr uint16_t INDICES[] =
		{
			// Back		Triangle
			0, 3, 2,	// 0
			2, 1, 0,	// 1
			// Front
			5, 6, 7,	// 0
			7, 4, 5,	// 1
			// Left
			4, 7, 3,	// 0
			3, 0, 4,	// 1
			// Right
			1, 2, 6,	// 0
			6, 5, 1,	// 1
			// Top
			6, 2, 3,	// 0
			3, 7, 6,	// 1
			// Bottom
			5, 4, 0,	// 0
			0, 1, 5		// 1
		};
		Rhi::IIndexBufferPtr indexBuffer(mBufferManager->createIndexBuffer(sizeof(INDICES), INDICES, 0, Rhi::BufferUsage::STATIC_DRAW, Rhi::IndexBufferFormat::UNSIGNED_SHORT));

		// Create vertex array object (VAO)
		// -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
		// -> This means that there's no need to keep an own vertex buffer object (VBO) reference
		// -> When the vertex array object (VAO) is destroyed, it automatically decreases the
		//    reference of the used vertex buffer objects (VBO). If the reference counter of a
		//    vertex buffer object (VBO) reaches zero, it's automatically destroyed.
		const Rhi::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBuffer };
		mVertexArray = mBufferManager->createVertexArray(vertexAttributes, static_cast<uint32_t>(GLM_COUNTOF(vertexArrayVertexBuffers)), vertexArrayVertexBuffers, indexBuffer);
	}

	{ // Create the graphics program
		// Get the shader source code (outsourced to keep an overview)
		const char* vertexShaderSourceCode = nullptr;
		const char* fragmentShaderSourceCode = nullptr;
#include "CubeTexture_GLSL_450.h"	// For Vulkan
#include "CubeTexture_GLSL_410.h"	// macOS 10.11 only supports OpenGL 4.1 hence it's our OpenGL minimum
#include "CubeTexture_GLSL_ES3.h"
#include "CubeTexture_HLSL_D3D12.h"
#include "CubeTexture_HLSL_D3D11.h"
#include "CubeTexture_Null.h"

		// Create the graphics program
		Rhi::IShaderLanguage& shaderLanguage = rhi->getDefaultShaderLanguage();
		mGraphicsProgram = shaderLanguage.createGraphicsProgram(
			*mRootSignature,
			vertexAttributes,
			shaderLanguage.createVertexShaderFromSourceCode(vertexAttributes, vertexShaderSourceCode),
			shaderLanguage.createFragmentShaderFromSourceCode(fragmentShaderSourceCode));
	}

	// Create the graphics pipeline state object (PSO)
	if ( nullptr != mGraphicsProgram )
	{
		mGraphicsPipelineState = rhi->createGraphicsPipelineState(Rhi::GraphicsPipelineStateBuilder(mRootSignature, mGraphicsProgram, vertexAttributes, getMainRenderTarget()->getRenderPass()));

		// Optimization: Cached data to not bother the RHI too much
		if ( nullptr == mUniformBuffer )
		{
			mObjectSpaceToClipSpaceMatrixUniformHandle = mGraphicsProgram->getUniformHandle("ObjectSpaceToClipSpaceMatrix");
		}
	}

	// Since we're always submitting the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
	fillCommandBuffer();

	return true;
}

void CubeTexture::update(double delta)
{
	mGlobalTimer += delta * 0.0005f;

	{ // Set uniform
			// Get the aspect ratio
		float aspectRatio = 4.0f / 3.0f;
		{
			// Get the render target with and height
			const Rhi::IRenderTarget* renderTarget = getMainRenderTarget();
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
		const glm::mat4 viewSpaceToClipSpace = glm::perspective(45.0f, aspectRatio, 100.0f, 0.1f);	// Near and far flipped due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
		const glm::mat4 viewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 3.0f));
		const glm::mat4 worldSpaceToViewSpace = glm::rotate(viewTranslate, mGlobalTimer, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
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
			};
			UniformBlockDynamicVs uniformBlockDynamicVS;
			memcpy(uniformBlockDynamicVS.objectSpaceToClipSpaceMatrix, glm::value_ptr(objectSpaceToClipSpace), sizeof(float) * 4 * 4);

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
		}
	}

	// Submit command buffer to the RHI implementation
	mCommandBuffer.submitToRhi(*rhi);
}

void CubeTexture::shutdown()
{
	// Release the used resources
	mObjectSpaceToClipSpaceMatrixUniformHandle = NULL_HANDLE;
	mVertexArray = nullptr;
	mGraphicsProgram = nullptr;
	mGraphicsPipelineState = nullptr;
	mSamplerStateGroup = nullptr;
	mTextureGroup = nullptr;
	mUniformBuffer = nullptr;
	mRootSignature = nullptr;
	mCommandBuffer.clear();
	mTextureManager = nullptr;
	mBufferManager = nullptr;
}

ApplicationSetting CubeTexture::intial_app_settings()
{
	ApplicationSetting settings;
	settings.width = 1280;
	settings.height = 720;
	settings.title = "CubeTexture";
	settings.rhiApi = RHIApi::Direct3D11;
	return settings;
}

void CubeTexture::window_resized(int width, int height)
{
}

void CubeTexture::fillCommandBuffer()
{
	SE_ASSERT(mCommandBuffer.isEmpty(), "The command buffer is already filled");
	SE_ASSERT(nullptr != mRootSignature, "Invalid root signature");
	SE_ASSERT(nullptr != mTextureGroup, "Invalid texture group");
	SE_ASSERT(nullptr != mSamplerStateGroup, "Invalid sampler state group");
	SE_ASSERT(nullptr != mGraphicsPipelineState, "Invalid graphics pipeline state");
	SE_ASSERT(nullptr != mVertexArray, "Invalid vertex array");

	// Scoped debug event
	COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(mCommandBuffer);

	// Clear the graphics color buffer of the current render target with color, do also clear the depth buffer
	const float color[4] = { 0.6f, 0.8f, 1.0f, 1.0f };
	Rhi::Command::ClearGraphics::create(mCommandBuffer, Rhi::ClearFlag::COLOR_DEPTH, color);

	// Set the used graphics root signature
	Rhi::Command::SetGraphicsRootSignature::create(mCommandBuffer, mRootSignature);

	// Set the used graphics pipeline state object (PSO)
	Rhi::Command::SetGraphicsPipelineState::create(mCommandBuffer, mGraphicsPipelineState);

	// Set graphics resource groups
	Rhi::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 0, mTextureGroup);
	Rhi::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 1, mSamplerStateGroup);

	// Input assembly (IA): Set the used vertex array
	Rhi::Command::SetGraphicsVertexArray::create(mCommandBuffer, mVertexArray);

	// Render the specified geometric primitive, based on an array of vertices
	Rhi::Command::DrawIndexedGraphics::create(mCommandBuffer, 36);
}