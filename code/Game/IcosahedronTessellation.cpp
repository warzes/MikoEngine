#include "IcosahedronTessellation.h"
#include "Color4.h"

bool IcosahedronTessellation::init(int argc, const char * argv[])
{
	// Get and check the RHI instance
// -> Uniform buffer object (UBO, "constant buffer" in Direct3D terminology) supported?
// -> Geometry shaders supported?
// -> Tessellation control and tessellation evaluation shaders supported?
	if ( nullptr != rhi && rhi->getCapabilities().maximumUniformBufferSize > 0 && rhi->getCapabilities().maximumNumberOfGsOutputVertices > 0 && rhi->getCapabilities().maximumNumberOfPatchVertices > 0 )
	{
		// Create the buffer manager
		mBufferManager = rhi->createBufferManager();

		{ // Create the root signature
			// Setup
			Rhi::DescriptorRangeBuilder ranges[4];
			ranges[0].initialize(Rhi::ResourceType::UNIFORM_BUFFER, 0, "UniformBlockDynamicTcs", Rhi::ShaderVisibility::TESSELLATION_CONTROL);
			ranges[1].initialize(Rhi::ResourceType::UNIFORM_BUFFER, 0, "UniformBlockStaticTes", Rhi::ShaderVisibility::TESSELLATION_EVALUATION);
			ranges[2].initialize(Rhi::ResourceType::UNIFORM_BUFFER, 0, "UniformBlockStaticGs", Rhi::ShaderVisibility::GEOMETRY);
			ranges[3].initialize(Rhi::ResourceType::UNIFORM_BUFFER, 0, "UniformBlockStaticFs", Rhi::ShaderVisibility::FRAGMENT);

			Rhi::RootParameterBuilder rootParameters[4];
			rootParameters[0].initializeAsDescriptorTable(1, &ranges[0]);
			rootParameters[1].initializeAsDescriptorTable(1, &ranges[1]);
			rootParameters[2].initializeAsDescriptorTable(1, &ranges[2]);
			rootParameters[3].initializeAsDescriptorTable(1, &ranges[3]);

			// Setup
			Rhi::RootSignatureBuilder rootSignatureBuilder;
			rootSignatureBuilder.initialize(static_cast<uint32_t>(GLM_COUNTOF(rootParameters)), rootParameters, 0, nullptr, Rhi::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			// Create the instance
			mRootSignature = rhi->createRootSignature(rootSignatureBuilder);
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
			// Create the vertex buffer object (VBO)
			// -> Geometry is from: http://prideout.net/blog/?p=48 (Philip Rideout, "The Little Grasshopper - Graphics Programming Tips")
			static constexpr float VERTEX_POSITION[] =
			{								// Vertex ID
				 0.000f,  0.000f,  1.000f,	// 0
				 0.894f,  0.000f,  0.447f,	// 1
				 0.276f,  0.851f,  0.447f,	// 2
				-0.724f,  0.526f,  0.447f,	// 3
				-0.724f, -0.526f,  0.447f,	// 4
				 0.276f, -0.851f,  0.447f,	// 5
				 0.724f,  0.526f, -0.447f,	// 6
				-0.276f,  0.851f, -0.447f,	// 7
				-0.894f,  0.000f, -0.447f,	// 8
				-0.276f, -0.851f, -0.447f,	// 9
				 0.724f, -0.526f, -0.447f,	// 10
				 0.000f,  0.000f, -1.000f	// 11
			};
			Rhi::IVertexBufferPtr vertexBuffer(mBufferManager->createVertexBuffer(sizeof(VERTEX_POSITION), VERTEX_POSITION));

			// Create the index buffer object (IBO)
			// -> Geometry is from: http://prideout.net/blog/?p=48 (Philip Rideout, "The Little Grasshopper - Graphics Programming Tips")
			static constexpr uint16_t INDICES[] =
			{				// Triangle ID
				0,  1,  2,	// 0
				0,  2,  3,	// 1
				0,  3,  4,	// 2
				0,  4,  5,	// 3
				0,  5,  1,	// 4
				7,  6,  11,	// 5
				8,  7,  11,	// 6
				9,  8,  11,	// 7
				10,  9, 11,	// 8
				6, 10,  11,	// 9
				6,  2,  1,	// 10
				7,  3,  2,	// 11
				8,  4,  3,	// 12
				9,  5,  4,	// 13
				10,  1, 5,	// 14
				6,  7,  2,	// 15
				7,  8,  3,	// 16
				8,  9,  4,	// 17
				9, 10,  5,	// 18
				10,  6, 1	// 19
			};
			Rhi::IIndexBufferPtr indexBuffer(mBufferManager->createIndexBuffer(sizeof(INDICES), INDICES));

			// Create vertex array object (VAO)
			// -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
			// -> This means that there's no need to keep an own vertex buffer object (VBO) reference
			// -> When the vertex array object (VAO) is destroyed, it automatically decreases the
			//    reference of the used vertex buffer objects (VBO). If the reference counter of a
			//    vertex buffer object (VBO) reaches zero, it's automatically destroyed.
			const Rhi::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBuffer };
			mVertexArray = mBufferManager->createVertexArray(vertexAttributes, static_cast<uint32_t>(GLM_COUNTOF(vertexArrayVertexBuffers)), vertexArrayVertexBuffers, indexBuffer);
		}

		{ // Create the uniform buffer group with tessellation control shader visibility
			Rhi::IResource* resources[1] = { mUniformBufferDynamicTcs = mBufferManager->createUniformBuffer(sizeof(float) * 2, nullptr, Rhi::BufferUsage::DYNAMIC_DRAW) };
			mUniformBufferGroupTcs = mRootSignature->createResourceGroup(0, static_cast<uint32_t>(GLM_COUNTOF(resources)), resources);
		}

		{ // Create the uniform buffer group with tessellation evaluation shader visibility: "ObjectSpaceToClipSpaceMatrix"
			const glm::mat4 worldSpaceToViewSpaceMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 3.0f));			// Also known as "view matrix"
			const glm::mat4 viewSpaceToClipSpaceMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 1000.0f, 0.001f);	// Also known as "projection matrix", near and far flipped due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
			const glm::mat4 objectSpaceToClipSpaceMatrix = viewSpaceToClipSpaceMatrix * worldSpaceToViewSpaceMatrix;			// Also known as "model view projection matrix"
			Rhi::IResource* resources[1] = { mBufferManager->createUniformBuffer(sizeof(float) * 4 * 4, glm::value_ptr(objectSpaceToClipSpaceMatrix)) };
			mUniformBufferGroupTes = mRootSignature->createResourceGroup(1, static_cast<uint32_t>(GLM_COUNTOF(resources)), resources);
		}

		{ // Create the uniform buffer group with geometry visibility: "NormalMatrix"
			const glm::mat4 worldSpaceToViewSpaceMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			const glm::mat4 viewSpaceToClipSpaceMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 1000.0f, 0.001f);	// Near and far flipped due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
			const glm::mat4 objectSpaceToClipSpaceMatrix = viewSpaceToClipSpaceMatrix * worldSpaceToViewSpaceMatrix;
			const glm::mat3 nMVP(objectSpaceToClipSpaceMatrix);
			const glm::mat4 tMVP(nMVP);
			Rhi::IResource* resources[1] = { mBufferManager->createUniformBuffer(sizeof(float) * 4 * 4, glm::value_ptr(tMVP)) };
			mUniformBufferGroupGs = mRootSignature->createResourceGroup(2, static_cast<uint32_t>(GLM_COUNTOF(resources)), resources);
		}

		{ // Create the uniform buffer group with fragment shader visibility: Light and material
			static constexpr float LIGHT_AND_MATERIAL[] =
			{
				0.25f, 0.25f, 1.0f,  1.0,	// "LightPosition"
				 0.0f, 0.75f, 0.75f, 1.0, 	// "DiffuseMaterial"
				0.04f, 0.04f, 0.04f, 1.0,	// "AmbientMaterial"
			};
			Rhi::IResource* resources[1] = { mBufferManager->createUniformBuffer(sizeof(LIGHT_AND_MATERIAL), LIGHT_AND_MATERIAL) };
			mUniformBufferGroupFs = mRootSignature->createResourceGroup(3, static_cast<uint32_t>(GLM_COUNTOF(resources)), resources);
		}

		{
			// Create the graphics program
			Rhi::IGraphicsProgramPtr graphicsProgram;
			{
				// Get the shader source code (outsourced to keep an overview)
				const char* vertexShaderSourceCode = nullptr;
				const char* tessellationControlShaderSourceCode = nullptr;
				const char* tessellationEvaluationShaderSourceCode = nullptr;
				const char* geometryShaderSourceCode = nullptr;
				const char* fragmentShaderSourceCode = nullptr;
#include "IcosahedronTessellation_GLSL_450.h"	// For Vulkan
#include "IcosahedronTessellation_GLSL_410.h"	// macOS 10.11 only supports OpenGL 4.1 hence it's our OpenGL minimum
#include "IcosahedronTessellation_HLSL_D3D11_D3D12.h"
#include "IcosahedronTessellation_Null.h"

				// Create the graphics program
				Rhi::IShaderLanguage& shaderLanguage = rhi->getDefaultShaderLanguage();
				graphicsProgram = shaderLanguage.createGraphicsProgram(
					*mRootSignature,
					vertexAttributes,
					shaderLanguage.createVertexShaderFromSourceCode(vertexAttributes, vertexShaderSourceCode),
					shaderLanguage.createTessellationControlShaderFromSourceCode(tessellationControlShaderSourceCode),
					shaderLanguage.createTessellationEvaluationShaderFromSourceCode(tessellationEvaluationShaderSourceCode),
					shaderLanguage.createGeometryShaderFromSourceCode(geometryShaderSourceCode, Rhi::GsInputPrimitiveTopology::TRIANGLES, Rhi::GsOutputPrimitiveTopology::TRIANGLES_STRIP, 3),
					shaderLanguage.createFragmentShaderFromSourceCode(fragmentShaderSourceCode));
			}

			// Create the graphics pipeline state object (PSO)
			if ( nullptr != graphicsProgram )
			{
				Rhi::GraphicsPipelineState graphicsPipelineState = Rhi::GraphicsPipelineStateBuilder(mRootSignature, graphicsProgram, vertexAttributes, getMainRenderTarget()->getRenderPass());
				graphicsPipelineState.primitiveTopology = Rhi::PrimitiveTopology::PATCH_LIST_3;	// Patch list with 3 vertices per patch (tessellation relevant topology type) - "Rhi::PrimitiveTopology::TriangleList" used for tessellation
				graphicsPipelineState.primitiveTopologyType = Rhi::PrimitiveTopologyType::PATCH;
				mGraphicsPipelineState = rhi->createGraphicsPipelineState(graphicsPipelineState);
			}
		}

		// Since we're always submitting the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
		fillCommandBuffer();
	}
	return true;
}

void IcosahedronTessellation::update(double delta)
{
	// Update the uniform buffer content
	if ( nullptr != mUniformBufferDynamicTcs )
	{
		// Copy data into the uniform buffer
		Rhi::MappedSubresource mappedSubresource;
		if ( rhi->map(*mUniformBufferDynamicTcs, 0, Rhi::MapType::WRITE_DISCARD, 0, mappedSubresource) )
		{
			const float data[] =
			{
				mTessellationLevelOuter,	// "TessellationLevelOuter"
				mTessellationLevelInner		// "TessellationLevelInner"
			};
			memcpy(mappedSubresource.data, data, sizeof(data));
			rhi->unmap(*mUniformBufferDynamicTcs, 0);
		}
	}

	// Submit command buffer to the RHI implementation
	mCommandBuffer.submitToRhi(*rhi);
}

void IcosahedronTessellation::shutdown()
{
	// Release the used resources
	mVertexArray = nullptr;
	mGraphicsPipelineState = nullptr;
	mUniformBufferGroupTcs = nullptr;
	mUniformBufferGroupTes = nullptr;
	mUniformBufferGroupGs = nullptr;
	mUniformBufferGroupFs = nullptr;
	mUniformBufferDynamicTcs = nullptr;
	mRootSignature = nullptr;
	mCommandBuffer.clear();
	mBufferManager = nullptr;
}

ApplicationSetting IcosahedronTessellation::intial_app_settings()
{
	ApplicationSetting settings;
	settings.width = 1280;
	settings.height = 720;
	settings.title = "IcosahedronTessellation";
	settings.rhiApi = RHIApi::Vulkan;
	return settings;
}

void IcosahedronTessellation::window_resized(int width, int height)
{
}

void IcosahedronTessellation::fillCommandBuffer()
{
	// Scoped debug event
	COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(mCommandBuffer);

	// Clear the graphics color buffer of the current render target with gray, do also clear the depth buffer
	Rhi::Command::ClearGraphics::create(mCommandBuffer, Rhi::ClearFlag::COLOR_DEPTH, Color4::GRAY);

	// Set the used graphics root signature
	Rhi::Command::SetGraphicsRootSignature::create(mCommandBuffer, mRootSignature);

	// Set the used graphics pipeline state object (PSO)
	Rhi::Command::SetGraphicsPipelineState::create(mCommandBuffer, mGraphicsPipelineState);

	// Set graphics resource groups
	Rhi::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 0, mUniformBufferGroupTcs);
	Rhi::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 1, mUniformBufferGroupTes);
	Rhi::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 2, mUniformBufferGroupGs);
	Rhi::Command::SetGraphicsResourceGroup::create(mCommandBuffer, 3, mUniformBufferGroupFs);

	// Input assembly (IA): Set the used vertex array
	Rhi::Command::SetGraphicsVertexArray::create(mCommandBuffer, mVertexArray);

	// Render the specified geometric primitive, based on indexing into an array of vertices
	Rhi::Command::DrawIndexedGraphics::create(mCommandBuffer, 60);
}