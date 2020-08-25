#if SE_OPENGLES
if ( rhi->getNameId() == Rhi::NameId::OPENGLES3 )
{
//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(#version 300 es	// OpenGL ES 3.0

// Attribute input/output
in  highp vec2 Position;	// Clip space vertex position as input, left/bottom is (-1,-1) and right/top is (1,1)
in  highp vec3 Color;		// RGB color as input
out highp vec3 ColorVS;		// RGB color as output

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
fragmentShaderSourceCode = R"(#version 300 es	// OpenGL ES 3.0

// Attribute input/output
in  mediump vec3 ColorVS;		// RGB color as input
out highp   vec4 OutputColor;	// Output variable for fragment color

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