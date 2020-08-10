#include "MyApp.h"

bool MyApp::init(int argc, const char * argv[])
{
	// Create the buffer manager
	bufferManager = rhi->createBufferManager();

	{ // Create the root signature
		// Setup
		Rhi::RootSignatureBuilder rootSignatureBuilder;
		rootSignatureBuilder.initialize(0, nullptr, 0, nullptr, Rhi::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		// Create the instance
		rootSignature = rhi->createRootSignature(rootSignatureBuilder RHI_RESOURCE_DEBUG_NAME("Triangle"));
	}

	// Vertex input layout
	static constexpr Rhi::VertexAttribute vertexAttributesLayout[] =
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
		}
	};
	const Rhi::VertexAttributes vertexAttributes(sizeof(vertexAttributesLayout) / sizeof(Rhi::VertexAttribute), vertexAttributesLayout);

	{ // Create vertex array object (VAO)
		// Create the vertex buffer object (VBO)
		// -> Clip space vertex positions, left/bottom is (-1,-1) and right/top is (1,1)
		static constexpr float VERTEX_POSITION[] =
		{					// Vertex ID	Triangle on screen
			 0.0f, 1.0f,	// 0				 0
			 1.0f, 0.0f,	// 1			   .   .
			-0.5f, 0.0f		// 2		     2.......1
		};
		Rhi::IVertexBufferPtr vertexBuffer(bufferManager->createVertexBuffer(sizeof(VERTEX_POSITION), VERTEX_POSITION, 0, Rhi::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME("Triangle")));

		// Create vertex array object (VAO)
		// -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
		// -> This means that there's no need to keep an own vertex buffer object (VBO) reference
		// -> When the vertex array object (VAO) is destroyed, it automatically decreases the
		//    reference of the used vertex buffer objects (VBO). If the reference counter of a
		//    vertex buffer object (VBO) reaches zero, it's automatically destroyed.
		const Rhi::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBuffer };
		vertexArray = bufferManager->createVertexArray(vertexAttributes, sizeof(vertexArrayVertexBuffers) / sizeof(Rhi::VertexArrayVertexBuffer), vertexArrayVertexBuffers, nullptr RHI_RESOURCE_DEBUG_NAME("Triangle"));
	}

	{
		// Create the graphics program
		Rhi::IGraphicsProgramPtr graphicsProgram;
		{
			// Get the shader source code (outsourced to keep an overview)
			const char* vertexShaderSourceCode = nullptr;
			const char* fragmentShaderSourceCode = nullptr;
#include "Example_GLSL_450.h"	// For Vulkan
#include "Example_GLSL_410.h"	// macOS 10.11 only supports OpenGL 4.1 hence it's our OpenGL minimum
#include "Example_GLSL_ES3.h"
#include "Example_HLSL_D3D11_D3D12.h"
#include "Example_Null.h"

			// Create the graphics program
			Rhi::IShaderLanguage& shaderLanguage = rhi->getDefaultShaderLanguage();
			graphicsProgram = shaderLanguage.createGraphicsProgram(
				*rootSignature,
				vertexAttributes,
				shaderLanguage.createVertexShaderFromSourceCode(vertexAttributes, vertexShaderSourceCode, nullptr RHI_RESOURCE_DEBUG_NAME("Triangle")),
				shaderLanguage.createFragmentShaderFromSourceCode(fragmentShaderSourceCode, nullptr RHI_RESOURCE_DEBUG_NAME("Triangle"))
				RHI_RESOURCE_DEBUG_NAME("Triangle"));
		}

		// Create the graphics pipeline state object (PSO)
		if ( nullptr != graphicsProgram )
		{
			graphicsPipelineState = rhi->createGraphicsPipelineState(Rhi::GraphicsPipelineStateBuilder(rootSignature, graphicsProgram, vertexAttributes, mainSwapChain->getRenderPass()) RHI_RESOURCE_DEBUG_NAME("Triangle"));
		}
	}

	// Record RHI command buffer
	{
		// Scoped debug event
		COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(commandBuffer);

		// Make the graphics main swap chain to the current render target
		Rhi::Command::SetGraphicsRenderTarget::create(commandBuffer, mainSwapChain);

		{ // Since Direct3D 12 is command list based, the viewport and scissor rectangle must be set in every draw call to work with all supported RHI implementations
			// Get the window size
			uint32_t width = 1;
			uint32_t height = 1;
			mainSwapChain->getWidthAndHeight(width, height);

			// Set the graphics viewport and scissor rectangle
			Rhi::Command::SetGraphicsViewportAndScissorRectangle::create(commandBuffer, 0, 0, width, height);
		}

		{ // Clear the graphics color buffer of the current render target with gray, do also clear the depth buffer
			const float color[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
			Rhi::Command::ClearGraphics::create(commandBuffer, Rhi::ClearFlag::COLOR_DEPTH, color);
		}

		// Set the used graphics root signature
		Rhi::Command::SetGraphicsRootSignature::create(commandBuffer, rootSignature);

		// Set the used graphics pipeline state object (PSO)
		Rhi::Command::SetGraphicsPipelineState::create(commandBuffer, graphicsPipelineState);

		// Input assembly (IA): Set the used vertex array
		Rhi::Command::SetGraphicsVertexArray::create(commandBuffer, vertexArray);

		// Set debug marker
		// -> Debug methods: When using Direct3D <11.1, these methods map to the Direct3D 9 PIX functions
		//    (D3DPERF_* functions, also works directly within VisualStudio 2017 out-of-the-box)
		COMMAND_SET_DEBUG_MARKER(commandBuffer, "Everyone ready for the upcoming triangle?")

		{
			// Scoped debug event
			COMMAND_SCOPED_DEBUG_EVENT(commandBuffer, "Drawing the fancy triangle")

				// Render the specified geometric primitive, based on an array of vertices
				Rhi::Command::DrawGraphics::create(commandBuffer, 3);
		}
	}


	return true;
}

void MyApp::update(double delta)
{
	// Submit command buffer to the RHI implementation
	commandBuffer.submitToRhi(*rhi);
}

void MyApp::shutdown()
{
}

ApplicationSetting MyApp::intial_app_settings()
{
    ApplicationSetting settings;
    settings.width = 1280;
    settings.height = 720;
    settings.title = "Hello Game";
    return settings;
}

void MyApp::window_resized(int width, int height)
{
}