#include "Queries.h"

bool Queries::init(int argc, const char * argv[])
{
	// Create the buffer manager
	m_bufferManager = rhi->createBufferManager();

	{ // Create the root signature
		// Setup
		Rhi::RootSignatureBuilder rootSignatureBuilder;
		rootSignatureBuilder.initialize(0, nullptr, 0, nullptr, Rhi::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		// Create the instance
		m_rootSignature = rhi->createRootSignature(rootSignatureBuilder RHI_RESOURCE_DEBUG_NAME("Triangle"));
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
	//const Rhi::VertexAttributes vertexAttributes(sizeof(vertexAttributesLayout) / sizeof(Rhi::VertexAttribute), vertexAttributesLayout);
	const Rhi::VertexAttributes vertexAttributes(glm::countof(vertexAttributesLayout), vertexAttributesLayout);

	{ // Create vertex array object (VAO)
		// Create the vertex buffer object (VBO)
		// -> Clip space vertex positions, left/bottom is (-1,-1) and right/top is (1,1)
		static constexpr float VERTEX_POSITION[] =
		{					// Vertex ID	Triangle on screen
			 0.0f, 1.0f,	// 0			  	  0
			 1.0f, 0.0f,	// 1			    .   .
			-1.0f, 0.0f		// 2			  2.......1
		};
		Rhi::IVertexBufferPtr vertexBuffer(m_bufferManager->createVertexBuffer(sizeof(VERTEX_POSITION), VERTEX_POSITION, 0, Rhi::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME("Triangle")));

		// Create vertex array object (VAO)
		// -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
		// -> This means that there's no need to keep an own vertex buffer object (VBO) reference
		// -> When the vertex array object (VAO) is destroyed, it automatically decreases the
		//    reference of the used vertex buffer objects (VBO). If the reference counter of a
		//    vertex buffer object (VBO) reaches zero, it's automatically destroyed.
		const Rhi::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBuffer };
		m_vertexArray = m_bufferManager->createVertexArray(vertexAttributes, glm::countof(vertexArrayVertexBuffers), vertexArrayVertexBuffers, nullptr RHI_RESOURCE_DEBUG_NAME("Triangle"));
	}

	{
		// Create the graphics program
		Rhi::IGraphicsProgramPtr graphicsProgram;
		{
			// Get the shader source code (outsourced to keep an overview)
			const char* vertexShaderSourceCode = nullptr;
			const char* fragmentShaderSourceCode = nullptr;
#include "Triangle_GLSL_450.h"	// For Vulkan
#include "Triangle_GLSL_410.h"	// macOS 10.11 only supports OpenGL 4.1 hence it's our OpenGL minimum
#include "Triangle_GLSL_ES3.h"
#include "Triangle_HLSL_D3D11_D3D12.h"
#include "Triangle_Null.h"

			// Create the graphics program
			Rhi::IShaderLanguage& shaderLanguage = rhi->getDefaultShaderLanguage();
			graphicsProgram = shaderLanguage.createGraphicsProgram(
				*m_rootSignature,
				vertexAttributes,
				shaderLanguage.createVertexShaderFromSourceCode(vertexAttributes, vertexShaderSourceCode, nullptr RHI_RESOURCE_DEBUG_NAME("Triangle")),
				shaderLanguage.createFragmentShaderFromSourceCode(fragmentShaderSourceCode, nullptr RHI_RESOURCE_DEBUG_NAME("Triangle"))
				RHI_RESOURCE_DEBUG_NAME("Triangle"));
		}

		// Create the graphics pipeline state object (PSO)
		if ( nullptr != graphicsProgram )
		{
			m_graphicsPipelineState = rhi->createGraphicsPipelineState(Rhi::GraphicsPipelineStateBuilder(m_rootSignature, graphicsProgram, vertexAttributes, mainSwapChain->getRenderPass()) RHI_RESOURCE_DEBUG_NAME("Triangle"));
		}
	}

	// Create the queries
	mOcclusionQueryPool = rhi->createQueryPool(Rhi::QueryType::OCCLUSION);
	mPipelineStatisticsQueryPool = rhi->createQueryPool(Rhi::QueryType::PIPELINE_STATISTICS);
	mTimestampQueryPool = rhi->createQueryPool(Rhi::QueryType::TIMESTAMP, 2);

	// Since we're always submitting the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
	fillCommandBuffer();
	return true;
}

void Queries::update(double delta)
{
	// Submit command buffer to the RHI implementation
	m_commandBuffer.submitToRhi(*rhi);
	// Sanity checks
	RHI_ASSERT(nullptr != mOcclusionQueryPool, "Invalid occlusion query pool");
	RHI_ASSERT(nullptr != mPipelineStatisticsQueryPool, "Invalid pipeline statistics query pool");
	RHI_ASSERT(nullptr != mTimestampQueryPool, "Invalid timestamp query pool");

	{ // Occlusion query pool
		uint64_t numberOfSamples = 0;
		if ( rhi->getQueryPoolResults(*mOcclusionQueryPool, sizeof(uint64_t), reinterpret_cast<uint8_t*>(&numberOfSamples)) )
		{
			//NOP;	// TODO(co) Process result
		}
	}

	{ // Pipeline statistics query pool
		Rhi::PipelineStatisticsQueryResult pipelineStatisticsQueryResult = {};
		if ( rhi->getQueryPoolResults(*mPipelineStatisticsQueryPool, sizeof(Rhi::PipelineStatisticsQueryResult), reinterpret_cast<uint8_t*>(&pipelineStatisticsQueryResult)) )
		{
			//NOP;	// TODO(co) Process result
		}
	}

	{ // Timestamp query pool
		uint64_t timestamp[2] = {};
		if ( rhi->getQueryPoolResults(*mTimestampQueryPool, sizeof(uint64_t) * 2, reinterpret_cast<uint8_t*>(&timestamp), 0, 2, sizeof(uint64_t)) )
		{
			//NOP;	// TODO(co) Process result
		}
	}
}

void Queries::shutdown()
{
	// Release the used resources
	mOcclusionQueryPool = nullptr;
	mPipelineStatisticsQueryPool = nullptr;
	mTimestampQueryPool = nullptr;
	m_vertexArray = nullptr;
	m_graphicsPipelineState = nullptr;
	m_rootSignature = nullptr;
	m_commandBuffer.clear();
	m_bufferManager = nullptr;
}

ApplicationSetting Queries::intial_app_settings()
{
	ApplicationSetting settings;
	settings.width = 1280;
	settings.height = 720;
	settings.title = "Queries";
	settings.rhiApi = RHIApi::Direct3D12;
	return settings;
}

void Queries::window_resized(int width, int height)
{
}

void Queries::fillCommandBuffer()
{
	// Sanity checks
	RHI_ASSERT(m_commandBuffer.isEmpty(), "The command buffer is already filled");
	RHI_ASSERT(nullptr != m_rootSignature, "Invalid root signature");
	RHI_ASSERT(nullptr != m_graphicsPipelineState, "Invalid graphics pipeline state");
	RHI_ASSERT(nullptr != m_vertexArray, "Invalid vertex array");
	RHI_ASSERT(nullptr != mOcclusionQueryPool, "Invalid occlusion query pool");
	RHI_ASSERT(nullptr != mPipelineStatisticsQueryPool, "Invalid pipeline statistics query pool");
	RHI_ASSERT(nullptr != mTimestampQueryPool, "Invalid timestamp query pool");

	// Scoped debug event
	COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(m_commandBuffer);

	// Reset and begin queries
	Rhi::Command::ResetQueryPool::create(m_commandBuffer, *mTimestampQueryPool, 0, 2);
	Rhi::Command::WriteTimestampQuery::create(m_commandBuffer, *mTimestampQueryPool, 0);
	Rhi::Command::ResetAndBeginQuery::create(m_commandBuffer, *mOcclusionQueryPool);
	Rhi::Command::ResetAndBeginQuery::create(m_commandBuffer, *mPipelineStatisticsQueryPool, 0, Rhi::QueryControlFlags::PRECISE);

	// Clear the graphics color buffer of the current render target with gray, do also clear the depth buffer
	const float color[4] = { 0.6f, 0.8f, 1.0f, 1.0f };
	Rhi::Command::ClearGraphics::create(m_commandBuffer, Rhi::ClearFlag::COLOR_DEPTH, color);

	// Set the used graphics root signature
	Rhi::Command::SetGraphicsRootSignature::create(m_commandBuffer, m_rootSignature);

	// Set the used graphics pipeline state object (PSO)
	Rhi::Command::SetGraphicsPipelineState::create(m_commandBuffer, m_graphicsPipelineState);

	// Input assembly (IA): Set the used vertex array
	Rhi::Command::SetGraphicsVertexArray::create(m_commandBuffer, m_vertexArray);

	// Set debug marker
	COMMAND_SET_DEBUG_MARKER(m_commandBuffer, "Everyone ready for the upcoming triangle?");
	{
		// Scoped debug event
		COMMAND_SCOPED_DEBUG_EVENT(m_commandBuffer, "Drawing the fancy triangle");

		// Render the specified geometric primitive, based on an array of vertices
		Rhi::Command::DrawGraphics::create(m_commandBuffer, 3);
	}

	// End queries
	Rhi::Command::EndQuery::create(m_commandBuffer, *mOcclusionQueryPool);
	Rhi::Command::EndQuery::create(m_commandBuffer, *mPipelineStatisticsQueryPool);
	Rhi::Command::WriteTimestampQuery::create(m_commandBuffer, *mTimestampQueryPool, 1);
}