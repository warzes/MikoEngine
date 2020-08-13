//[-------------------------------------------------------]
//[ Shader start                                          ]
//[-------------------------------------------------------]
#ifdef SE_OPENGLES
if ( rhi->getNameId() == Rhi::NameId::OPENGLES3 )
{


	//[-------------------------------------------------------]
	//[ Vertex shader source code                             ]
	//[-------------------------------------------------------]
	// One vertex shader invocation per vertex
	vertexShaderSourceCode = R"(#version 300 es	// OpenGL ES 3.0

// Attribute input/output
in  highp vec2 Position;	// Clip space vertex position as input, left/bottom is (-1,-1) and right/top is (1,1)
out highp vec2 TexCoord;	// Normalized texture coordinate as output

// Programs
void main()
{
	// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = vec4(Position, 0.5, 1.0);
	TexCoord = Position.xy;
}
)";


	//[-------------------------------------------------------]
	//[ Fragment shader source code                           ]
	//[-------------------------------------------------------]
	// One fragment shader invocation per fragment
	fragmentShaderSourceCode = R"(#version 300 es	// OpenGL ES 3.0

// Attribute input/output
in  mediump vec2 TexCoord;		// Normalized texture coordinate as input
out highp   vec4 OutputColor;	// Output variable for fragment color

// Uniforms
uniform mediump sampler2D GradientMap;	// OpenGL ES 3 has no 1D textures, just use a 2D texture with a height of one
uniform mediump sampler2D AlbedoMap;

// Programs
void main()
{
	// Fetch the texel at the given texture coordinate and return its color
	OutputColor = texture(GradientMap, vec2(TexCoord.y, 0.0f)).r * texture(AlbedoMap, TexCoord);
}
)";


	//[-------------------------------------------------------]
	//[ Shader end                                            ]
	//[-------------------------------------------------------]
}
else
#endif