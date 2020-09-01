#include "GeometryShader.h"
#include "Color4.h"

bool GeometryShader::init(int argc, const char * argv[])
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
	const Rhi::VertexAttributes vertexAttributes(0, nullptr);

	{
		// Create the graphics program
		Rhi::IGraphicsProgramPtr graphicsProgram;
		{
			// Get the shader source code (outsourced to keep an overview)
			const char* vertexShaderSourceCode = nullptr;
			const char* geometryShaderSourceCode = nullptr;
			const char* fragmentShaderSourceCode = nullptr;
#include "GeometryShader_GLSL_450.h"	// For Vulkan
#include "GeometryShader_GLSL_410.h"	// macOS 10.11 only supports OpenGL 4.1 hence it's our OpenGL minimum
#include "GeometryShader_HLSL_D3D11_D3D12.h"
#include "GeometryShader_Null.h"

			// Create the graphics program
			Rhi::IShaderLanguage& shaderLanguage = rhi->getDefaultShaderLanguage();
			graphicsProgram = shaderLanguage.createGraphicsProgram(
				*mRootSignature,
				vertexAttributes,
				shaderLanguage.createVertexShaderFromSourceCode(vertexAttributes, vertexShaderSourceCode),
				shaderLanguage.createGeometryShaderFromSourceCode(geometryShaderSourceCode, Rhi::GsInputPrimitiveTopology::POINTS, Rhi::GsOutputPrimitiveTopology::TRIANGLES_STRIP, 3),
				shaderLanguage.createFragmentShaderFromSourceCode(fragmentShaderSourceCode));
		}

		// Create the graphics pipeline state object (PSO)
		if ( nullptr != graphicsProgram )
		{
			Rhi::GraphicsPipelineState graphicsPipelineState = Rhi::GraphicsPipelineStateBuilder(mRootSignature, graphicsProgram, vertexAttributes, getMainRenderTarget()->getRenderPass());
			graphicsPipelineState.primitiveTopology = Rhi::PrimitiveTopology::POINT_LIST;
			graphicsPipelineState.primitiveTopologyType = Rhi::PrimitiveTopologyType::POINT;
			mGraphicsPipelineState = rhi->createGraphicsPipelineState(graphicsPipelineState);
		}
	}

	// Since we're always submitting the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
	fillCommandBuffer();

	return true;
}

void GeometryShader::update(double delta)
{
	mCommandBuffer.submitToRhi(*rhi);
}

void GeometryShader::shutdown()
{
	// Release the used resources
	mGraphicsPipelineState = nullptr;
	mRootSignature = nullptr;
	mCommandBuffer.clear();
	mBufferManager = nullptr;
}

ApplicationSetting GeometryShader::intial_app_settings()
{
	ApplicationSetting settings;
	settings.width = 1280;
	settings.height = 720;
	settings.title = "GeometryShader";
	settings.rhiApi = RHIApi::Direct3D12;
	return settings;
}

void GeometryShader::window_resized(int width, int height)
{
}

void GeometryShader::fillCommandBuffer()
{
	// Scoped debug event
	COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(mCommandBuffer);

	// Clear the graphics color buffer of the current render target with gray, do also clear the depth buffer
	Rhi::Command::ClearGraphics::create(mCommandBuffer, Rhi::ClearFlag::COLOR_DEPTH, Color4::GRAY);

	// Set the used graphics root signature
	Rhi::Command::SetGraphicsRootSignature::create(mCommandBuffer, mRootSignature);

	// Set the used graphics pipeline state object (PSO)
	Rhi::Command::SetGraphicsPipelineState::create(mCommandBuffer, mGraphicsPipelineState);

	// Render the specified geometric primitive, based on an array of vertices
	// -> Emit a single point in order to generate a draw call, the geometry shader does the rest
	// -> Attribute-less rendering (aka "drawing without data")
	Rhi::Command::DrawGraphics::create(mCommandBuffer, 1);
}