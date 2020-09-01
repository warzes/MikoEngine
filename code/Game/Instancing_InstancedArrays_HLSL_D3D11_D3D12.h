#if SE_DIRECT3D11 || SE_DIRECT3D12
if (rhi->getNameId() == Rhi::NameId::DIRECT3D11 || rhi->getNameId() == Rhi::NameId::DIRECT3D12)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(
// Attribute input/output
struct VS_INPUT
{
	// Mesh data
	float2 Position   : POSITION;	// Clip space vertex position as output, left/bottom is (-1,-1) and right/top is (1,1)
	// Per-instance data
	float  InstanceID : TEXCOORD0;	// Simple instance ID in order to keep it similar to the "draw instanced" version on the right side (blue)
};
struct VS_OUTPUT
{
	float4 Position : SV_POSITION;	// Clip space vertex position as output, left/bottom is (-1,-1) and right/top is (1,1)
};

// Programs
VS_OUTPUT main(VS_INPUT input)
{
	// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	VS_OUTPUT output;
	output.Position = float4(input.Position.x, input.Position.y - input.InstanceID, 0.5f, 1.0f);
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
float4 main(float4 Position : SV_POSITION) : SV_TARGET
{
	// Return green
	return float4(0.0f, 1.0f, 0.0f, 1.0f);
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else
#endif