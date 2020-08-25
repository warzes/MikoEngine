#if SE_DIRECT3D12
if ( rhi->getNameId() == Rhi::NameId::DIRECT3D12 )
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
	float3 TexCoord : TEXCOORD0;	// Normalized texture coordinate as output
};

// Uniforms
cbuffer UniformBlockDynamicVs : register(b0)
{
	float4x4 ObjectSpaceToClipSpaceMatrix;	// Object space to clip space matrix
}

// Programs
VS_OUTPUT main(float3 Position : POSITION)	// Object space vertex position as input
{
	// Vertex output
	VS_OUTPUT output;
	output.Position = mul(ObjectSpaceToClipSpaceMatrix, float4(Position, 1.0)); // Calculate the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	output.TexCoord = normalize(Position);
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
SamplerState SamplerPoint : register(s0);
TextureCube CubeMap : register(t0);

// Programs
float4 main(float4 Position : SV_POSITION, float3 TexCoord : TEXCOORD0) : SV_TARGET
{
	// Fetch the texel at the given texture coordinate and return its color
	return CubeMap.Sample(SamplerPoint, TexCoord);
}
)";


	//[-------------------------------------------------------]
	//[ Shader end                                            ]
	//[-------------------------------------------------------]
}
else
#endif
