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
in  highp vec2 Position;	// Object space vertex position as input, left/bottom is (0,0) and right/top is (1,1)
in  highp vec2 TexCoord;	// Normalized texture coordinate as input
out highp vec2 TexCoordVs;	// Normalized texture coordinate as output
in  highp vec4 Color;		// Color as input
out highp vec4 ColorVs;		// Color as output

// Uniforms
layout(std140) uniform UniformBlockDynamicVs
{
	highp mat4 ObjectSpaceToClipSpaceMatrix;
};

// Programs
void main()
{
	// Calculate the clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
	gl_Position = ObjectSpaceToClipSpaceMatrix * vec4(Position, 0.5f, 1.0f);

	// Pass through the vertex texture coordinate
	TexCoordVs = TexCoord;

	// Pass through the vertex color
	ColorVs = Color;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 300 es	// OpenGL ES 3.0

// Attribute input/output
in  mediump vec2 TexCoordVs;	// Normalized texture coordinate as input
in  mediump vec4 ColorVs;
out highp   vec4 OutputColor;	// Output variable for fragment color

// Uniforms
uniform mediump sampler2D GlyphMap;	// Glyph atlas texture map

// Programs
void main()
{
	// Fetch the texel at the given texture coordinate and return its color
	OutputColor = ColorVs * texture(GlyphMap, TexCoordVs).rrrr;
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else
#endif
