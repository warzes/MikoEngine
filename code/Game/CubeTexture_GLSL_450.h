#if SE_VULKAN
if ( rhi->getNameId() == Rhi::NameId::VULKAN )
{


	//[-------------------------------------------------------]
	//[ Vertex shader source code                             ]
	//[-------------------------------------------------------]
	// One vertex shader invocation per vertex
	vertexShaderSourceCode = R"(#version 450 core	// OpenGL 4.5

// Attribute input/output
layout(location = 0) in  vec3 Position;	// Object space vertex position
layout(location = 0) out gl_PerVertex
{
	vec4 gl_Position;
};
layout(location = 1) out vec3 TexCoord;	// Normalized texture coordinate as output

// Uniforms
layout(std140, set = 0, binding = 0) uniform UniformBlockDynamicVs
{
	mat4 ObjectSpaceToClipSpaceMatrix;	// Object space to clip space matrix
};

// Programs
void main()
{
	// Calculate the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = ObjectSpaceToClipSpaceMatrix * vec4(Position, 1.0);
	TexCoord = normalize(Position);
}
)";


	//[-------------------------------------------------------]
	//[ Fragment shader source code                           ]
	//[-------------------------------------------------------]
	// One fragment shader invocation per fragment
	fragmentShaderSourceCode = R"(#version 450 core	// OpenGL 4.5

// Attribute input/output
layout(location = 1) in  vec3 TexCoord;		// Normalized texture coordinate as input
layout(location = 0) out vec4 OutputColor;	// Output variable for fragment color

// Uniforms
layout(set = 0, binding = 1) uniform samplerCubeArray CubeMapArray;

// Programs
void main()
{
	// Fetch the texel at the given texture coordinate and return its color
	OutputColor = texture(CubeMapArray, vec4(TexCoord, 1.0f));	// The cube at index zero is black by intent, use index one
}
)";


	//[-------------------------------------------------------]
	//[ Shader end                                            ]
	//[-------------------------------------------------------]
}
else
#endif
