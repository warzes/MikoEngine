#if SE_OPENGLES
if (rhi->getNameId() == Rhi::NameId::OPENGLES3)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(#version 300 es	// OpenGL ES 3.0

// Attribute input/output
in highp vec2 Position;	// Clip space vertex position as input, left/bottom is (-1,-1) and right/top is (1,1)

// Programs
void main()
{
	// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = vec4(Position.x, Position.y - float(gl_InstanceID), 0.5, 1.0);
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 300 es	// OpenGL ES 3.0

// Attribute input/output
out highp vec4 OutputColor;	// Output variable for fragment color

// Programs
void main()
{
	// Return blue
	OutputColor = vec4(0.0, 0.0, 1.0, 1.0);
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else
#endif
