#if SE_VULKAN
if ( rhi->getNameId() == Rhi::NameId::VULKAN )
{
//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(#version 450 core	// OpenGL 4.5

// Attribute input/output
layout(location = 0) in vec2 Position;	// Clip space vertex position as input, left/bottom is (-1,-1) and right/top is (1,1)
layout(location = 1) in vec3 Color;		// RGB color as input
out gl_PerVertex
{
	vec4 gl_Position;
};
layout(location = 0) out vec3 ColorVS;	// RGB color as output

// Programs
void main()
{
	// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = vec4(Position, 0.5, 1.0);

	// Pass through the color
	ColorVS = Color;
}
)";

//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 450 core	// OpenGL 4.5

// Attribute input/output
layout(location = 0) in  vec3 ColorVS;		// RGB color as input
layout(location = 0) out vec4 OutputColor;	// Output variable for fragment color

// Programs
void main()
{
	// Return white
	OutputColor = vec4(ColorVS, 1.0);
}
)";
}
else
#endif