#include "VertexBuffer.h"

bool VertexBuffer::init(int argc, const char * argv[])
{
	// Create the buffer manager
	mBufferManager = rhi->createBufferManager();

	{ // Create the root signature
		// Setup
		Rhi::RootSignatureBuilder rootSignatureBuilder;
		rootSignatureBuilder.initialize(0, nullptr, 0, nullptr, Rhi::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		// Create the instance
		mRootSignature = rhi->createRootSignature(rootSignatureBuilder);
	}

	// Vertex input layout
	static constexpr Rhi::VertexAttribute vertexAttributesLayoutVBO[] =
	{
		{ // Attribute 0
			// Data destination
			Rhi::VertexAttributeFormat::FLOAT_2,	// vertexAttributeFormat (Rhi::VertexAttributeFormat)
			"Position",								// name[32] (char)
			"POSITION",								// semanticName[32] (char)
			0,										// semanticIndex (uint32_t)
			// Data source
			0,										// inputSlot (uint32_t)
			0,										// alignedByteOffset (uint32_t)
			sizeof(float) * 5,						// strideInBytes (uint32_t)
			0										// instancesPerElement (uint32_t)
		},
		{ // Attribute 1
			// Data destination
			Rhi::VertexAttributeFormat::FLOAT_3,	// vertexAttributeFormat (Rhi::VertexAttributeFormat)
			"Color",								// name[32] (char)
			"COLOR",								// semanticName[32] (char)
			0,										// semanticIndex (uint32_t)
			// Data source
			0,										// inputSlot (uint32_t)
			sizeof(float) * 2,						// alignedByteOffset (uint32_t)
			sizeof(float) * 5,						// strideInBytes (uint32_t)
			0										// instancesPerElement (uint32_t)
		}
	};
	const Rhi::VertexAttributes vertexAttributesVBO(static_cast<uint32_t>(GLM_COUNTOF(vertexAttributesLayoutVBO)), vertexAttributesLayoutVBO);
	static constexpr Rhi::VertexAttribute vertexAttributesLayoutVBOs[] =
	{
		{ // Attribute 0
			// Data destination
			Rhi::VertexAttributeFormat::FLOAT_2,	// vertexAttributeFormat (Rhi::VertexAttributeFormat)
			"Position",								// name[32] (char)
			"POSITION",								// semanticName[32] (char)
			0,										// semanticIndex (uint32_t)
			// Data source
			0,										// inputSlot (uint32_t)
			0,										// alignedByteOffset (uint32_t)
			sizeof(float) * 2,						// strideInBytes (uint32_t)
			0										// instancesPerElement (uint32_t)
		},
		{ // Attribute 1
			// Data destination
			Rhi::VertexAttributeFormat::FLOAT_3,	// vertexAttributeFormat (Rhi::VertexAttributeFormat)
			"Color",								// name[32] (char)
			"COLOR",								// semanticName[32] (char)
			0,										// semanticIndex (uint32_t)
			// Data source
			1,										// inputSlot (uint32_t)
			0,										// alignedByteOffset (uint32_t)
			sizeof(float) * 3,						// strideInBytes (uint32_t)
			0										// instancesPerElement (uint32_t)
		}
	};
	const Rhi::VertexAttributes vertexAttributesVBOs(static_cast<uint32_t>(GLM_COUNTOF(vertexAttributesLayoutVBOs)), vertexAttributesLayoutVBOs);

	// Vertex array object (VAO)
	// -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
	// -> This means that there's no need to keep an own vertex buffer object (VBO) reference
	// -> When the vertex array object (VAO) is destroyed, it automatically decreases the
	//    reference of the used vertex buffer objects (VBO). If the reference counter of a
	//    vertex buffer object (VBO) reaches zero, it's automatically destroyed.

	{ // Create vertex array object (VAO)
		// Create the vertex buffer object (VBO) holding position and color data
		// -> Clip space vertex positions, left/bottom is (-1,-1) and right/top is (1,1)
		// -> Traditional normalized RGB vertex colors
		static constexpr float VERTEX_POSITION_COLOR[] =
		{	 // Position     Color				// Vertex ID	Triangle on screen
			 0.0f, 1.0f,	1.0f, 0.0f, 0.0f,	// 0				0
			 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	// 1			   .   .
			-0.5f, 0.0f,	0.0f, 0.0f, 1.0f	// 2			  2.......1
		};
		Rhi::IVertexBufferPtr vertexBufferPositionColor(mBufferManager->createVertexBuffer(sizeof(VERTEX_POSITION_COLOR), VERTEX_POSITION_COLOR));

		// Create vertex array object (VAO)
		const Rhi::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBufferPositionColor };
		mVertexArrayVbo = mBufferManager->createVertexArray(vertexAttributesVBO, static_cast<uint32_t>(GLM_COUNTOF(vertexArrayVertexBuffers)), vertexArrayVertexBuffers);
	}

	{ // Create vertex array object (VAO) using multiple vertex buffer object (VBO)
		// Create the vertex buffer object (VBO) holding color data
		// -> Traditional normalized RGB vertex colors
		static constexpr float VERTEX_COLOR[] =
		{					// Vertex ID	Triangle on screen
			1.0f, 0.0f, 0.0f,	// 0			  0.......1
			0.0f, 1.0f, 0.0f,	// 1			   .   .
			0.0f, 0.0f, 1.0f	// 2			  	2
		};
		Rhi::IVertexBufferPtr vertexBufferColor(mBufferManager->createVertexBuffer(sizeof(VERTEX_COLOR), VERTEX_COLOR));

		// Create the vertex buffer object (VBO) holding position data
		// -> Clip space vertex positions, left/bottom is (-1,-1) and right/top is (1,1)
		static constexpr float VERTEX_POSITION[] =
		{					// Vertex ID	Triangle on screen
			-0.5f,  0.0f,	// 0			  0.......1
			 1.0f,  0.0f,	// 1			   .   .
			 0.0f, -1.0f	// 2			  	2
		};
		Rhi::IVertexBufferPtr vertexBufferPosition(mBufferManager->createVertexBuffer(sizeof(VERTEX_POSITION), VERTEX_POSITION));

		// Create vertex array object (VAO)
		const Rhi::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBufferPosition, vertexBufferColor };
		mVertexArrayVbos = mBufferManager->createVertexArray(vertexAttributesVBOs, static_cast<uint32_t>(GLM_COUNTOF(vertexArrayVertexBuffers)), vertexArrayVertexBuffers);
	}

	{
		// Get the shader source code (outsourced to keep an overview)
		const char* vertexShaderSourceCode = nullptr;
		const char* fragmentShaderSourceCode = nullptr;
#include "VertexBuffer_GLSL_450.h"	// For Vulkan
#include "VertexBuffer_GLSL_410.h"	// macOS 10.11 only supports OpenGL 4.1 hence it's our OpenGL minimum
#include "VertexBuffer_GLSL_ES3.h"
#include "VertexBuffer_HLSL_D3D11_D3D12.h"
#include "VertexBuffer_Null.h"
		Rhi::IShaderLanguage& shaderLanguage = rhi->getDefaultShaderLanguage();

		{ // Create pipeline state objects (PSO) using one vertex buffer object (VBO)
			// Create the graphics program
			Rhi::IGraphicsProgramPtr graphicsProgram;
			graphicsProgram = shaderLanguage.createGraphicsProgram(
				*mRootSignature,
				vertexAttributesVBO,
				shaderLanguage.createVertexShaderFromSourceCode(vertexAttributesVBO, vertexShaderSourceCode),
				shaderLanguage.createFragmentShaderFromSourceCode(fragmentShaderSourceCode));

			// Create the graphics pipeline state objects (PSO)
			if ( nullptr != graphicsProgram )
			{
				mGraphicsPipelineStateVbo = rhi->createGraphicsPipelineState(Rhi::GraphicsPipelineStateBuilder(mRootSignature, graphicsProgram, vertexAttributesVBO, mainSwapChain->getRenderPass()));
			}
		}

		{ // Create graphics pipeline state objects (PSO) using multiple vertex buffer object (VBO)
			// Create the graphics program
			Rhi::IGraphicsProgramPtr graphicsProgram;
			graphicsProgram = shaderLanguage.createGraphicsProgram(
				*mRootSignature,
				vertexAttributesVBOs,
				shaderLanguage.createVertexShaderFromSourceCode(vertexAttributesVBOs, vertexShaderSourceCode),
				shaderLanguage.createFragmentShaderFromSourceCode(fragmentShaderSourceCode));

			// Create the graphics pipeline state objects (PSO)
			if ( nullptr != graphicsProgram )
			{
				mGraphicsPipelineStateVbos = rhi->createGraphicsPipelineState(Rhi::GraphicsPipelineStateBuilder(mRootSignature, graphicsProgram, vertexAttributesVBOs, mainSwapChain->getRenderPass()));
			}
		}
	}

	// Since we're always submitting the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
	fillCommandBuffer();
	return true;
}

void VertexBuffer::update(double delta)
{
	mCommandBuffer.submitToRhi(*rhi);
}

void VertexBuffer::shutdown()
{
	// Release the used resources
	mGraphicsPipelineStateVbos = nullptr;
	mVertexArrayVbos = nullptr;
	mVertexArrayVbo = nullptr;
	mGraphicsPipelineStateVbo = nullptr;
	mRootSignature = nullptr;
	mCommandBuffer.clear();
	mBufferManager = nullptr;
}

ApplicationSetting VertexBuffer::intial_app_settings()
{
	ApplicationSetting settings;
	settings.width = 1280;
	settings.height = 720;
	settings.title = "VertexBuffer";
	settings.rhiApi = RHIApi::Direct3D12;
	return settings;
}

void VertexBuffer::window_resized(int width, int height)
{
}

void VertexBuffer::fillCommandBuffer()
{
	RHI_ASSERT(mCommandBuffer.isEmpty(), "The command buffer is already filled");
	RHI_ASSERT(nullptr != mRootSignature, "Invalid root signature");
	RHI_ASSERT(nullptr != mGraphicsPipelineStateVbo, "Invalid graphics pipeline state VBO");
	RHI_ASSERT(nullptr != mVertexArrayVbo, "Invalid vertex array VBO");
	RHI_ASSERT(nullptr != mGraphicsPipelineStateVbos, "Invalid graphics pipeline state VBOs");
	RHI_ASSERT(nullptr != mVertexArrayVbos, "Invalid vertex array VBOs");

	// Scoped debug event
	COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(mCommandBuffer);

	// Clear the graphics color buffer of the current render target with color, do also clear the depth buffer
	const float color[4] = { 0.6f, 0.8f, 1.0f, 1.0f };
	Rhi::Command::ClearGraphics::create(mCommandBuffer, Rhi::ClearFlag::COLOR_DEPTH, color);

	// Set the used graphics root signature
	Rhi::Command::SetGraphicsRootSignature::create(mCommandBuffer, mRootSignature);

	// First lower triangle using one vertex buffer object (VBO)
	if ( nullptr != mGraphicsPipelineStateVbo )
	{
		// Scoped debug event
		COMMAND_SCOPED_DEBUG_EVENT(mCommandBuffer, "Draw using one VBO");

		// Set the used graphics pipeline state object (PSO)
		Rhi::Command::SetGraphicsPipelineState::create(mCommandBuffer, mGraphicsPipelineStateVbo);

		// Input assembly (IA): Set the used vertex array
		Rhi::Command::SetGraphicsVertexArray::create(mCommandBuffer, mVertexArrayVbo);

		// Render the specified geometric primitive, based on an array of vertices
		Rhi::Command::DrawGraphics::create(mCommandBuffer, 3);
	}

	// Second upper triangle using multiple vertex buffer object (VBO)
	if ( nullptr != mGraphicsPipelineStateVbos )
	{
		// Scoped debug event
		COMMAND_SCOPED_DEBUG_EVENT(mCommandBuffer, "Draw using multiple VBOs");

		// Set the used graphics pipeline state object (PSO)
		Rhi::Command::SetGraphicsPipelineState::create(mCommandBuffer, mGraphicsPipelineStateVbos);

		// Input assembly (IA): Set the used vertex array
		Rhi::Command::SetGraphicsVertexArray::create(mCommandBuffer, mVertexArrayVbos);

		// Render the specified geometric primitive, based on an array of vertices
		Rhi::Command::DrawGraphics::create(mCommandBuffer, 3);
	}
}