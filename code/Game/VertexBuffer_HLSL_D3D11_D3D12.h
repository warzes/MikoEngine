#if SE_DIRECT3D11 || SE_DIRECT3D12
if ( rhi->getNameId() == Rhi::NameId::DIRECT3D11 || rhi->getNameId() == Rhi::NameId::DIRECT3D12 )
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
	float3 Color    : COLOR;		// RGB color as output
};

// Programs
VS_OUTPUT main(float2 Position : POSITION,	// Clip space vertex position as input, left/bottom is (-1,-1) and right/top is (1,1)
			   float3 Color    : COLOR)		// RGB color as input
{
	VS_OUTPUT output;

	// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	output.Position = float4(Position, 0.5f, 1.0f);

	// Pass through the color
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
// Programs
float4 main(float4 Position : SV_POSITION, float3 Color : COLOR) : SV_TARGET
{
	// Return white
	return float4(Color, 1.0f);
}
)";


	//[-------------------------------------------------------]
	//[ Shader end                                            ]
	//[-------------------------------------------------------]
}
else
#endif
