//[-------------------------------------------------------]
//[ Shader start                                          ]
//[-------------------------------------------------------]
#if SE_DIRECT3D11 || SE_DIRECT3D12
if (rhi.getNameId() == Rhi::NameId::DIRECT3D11 || rhi.getNameId() == Rhi::NameId::DIRECT3D12)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(
// Attribute input/output
struct VS_OUTPUT
{
	float4 Position : SV_POSITION;	// Clip space vertex position as output, left/bottom is (-1,-1) and right/top is (1,1)
	float2 TexCoord : TEXCOORD0;	// Normalized texture coordinate as output
	float4 Color    : COLOR0;
};

// Uniforms
cbuffer UniformBlockDynamicVs : register(b0)
{
	float4x4 ObjectSpaceToClipSpaceMatrix;
}

// Programs
VS_OUTPUT main(float2 Position : POSITION,	// Clip space vertex position as input, left/bottom is (-1,-1) and right/top is (1,1)
			   float2 TexCoord : TEXCOORD0,	// Normalized texture coordinate as input
			   float4 Color    : COLOR0)
{
	VS_OUTPUT output;

	// Calculate the clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
	output.Position = mul(ObjectSpaceToClipSpaceMatrix, float4(Position, 0.5f, 1.0f));

	// Pass through the vertex texture coordinate
	output.TexCoord = TexCoord;

	// Pass through the vertex color
	output.Color = Color;

	// Done
	return output;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
// "pixel shader" in Direct3D terminology
fragmentShaderSourceCode = R"(
// Uniforms
SamplerState SamplerLinear : register(s0);
Texture2D GlyphMap : register(t0);	// Glyph atlas texture map

// Programs
float4 main(float4 Position : SV_POSITION, float2 TexCoord : TEXCOORD0, float4 Color : COLOR0) : SV_TARGET
{
	// Fetch the texel at the given texture coordinate and return its color
	return Color * GlyphMap.Sample(SamplerLinear, TexCoord).rrrr;
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else
#endif
