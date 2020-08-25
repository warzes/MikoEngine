#if SE_OPENGL
if ( rhi->getNameId() == Rhi::NameId::OPENGL )
{


	//[-------------------------------------------------------]
	//[ Vertex shader source code                             ]
	//[-------------------------------------------------------]
	// One vertex shader invocation per vertex
	vertexShaderSourceCode = R"(#version 410 core	// OpenGL 4.1

// Attribute input/output
in  vec3 Position;	// Object space vertex position
out gl_PerVertex
{
	vec4 gl_Position;
};
out vec3 TexCoord;	// Normalized texture coordinate as output

// Uniforms
layout(std140) uniform UniformBlockDynamicVs
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
	fragmentShaderSourceCode = R"(#version 410 core	// OpenGL 4.1

// Attribute input/output
in  vec3 TexCoord;
out vec4 OutputColor;	// Output variable for fragment color

// Uniforms
uniform samplerCube CubeMap;

// Programs
void main()
{
	// Fetch the texel at the given texture coordinate and return its color
	OutputColor = texture(CubeMap, TexCoord);
}
)";


	//[-------------------------------------------------------]
	//[ Shader end                                            ]
	//[-------------------------------------------------------]
}
else
#endif
