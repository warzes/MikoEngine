//[-------------------------------------------------------]
//[ Shader start                                          ]
//[-------------------------------------------------------]
#if SE_OPENGLES
if (rhi.getNameId() == Rhi::NameId::OPENGLES3)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(#version 300 es	// OpenGL ES 3.0

// Attribute input/output
in highp vec2 Position;	// Object space vertex position as input, left/bottom is (0,0) and right/top is (1,1)

// Programs
void main()
{
	// Calculate the clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
	// -> z = 1 instead of 0 due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
	gl_Position = vec4(Position * vec2(2.0f, 2.0f) - vec2(1.0f, 1.0f), 1.0f, 1.0f);
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
	// Grey so the VR hidden area mesh doesn't influence the adaptive luminance
	OutputColor = vec4(0.5f, 0.5f, 0.5f, 0.0f);
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else
#endif
