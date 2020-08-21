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
};

// Programs
VS_OUTPUT main(float2 Position : POSITION)	// Clip space vertex position as input, left/bottom is (-1,-1) and right/top is (1,1)
{
	VS_OUTPUT output;

	// Calculate the clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
	// -> z = 1 instead of 0 due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
	output.Position = float4(Position * float2(2.0f, 2.0f) - float2(1.0f, 1.0f), 1.0f, 1.0f);

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
float4 main(float4 Position : SV_POSITION) : SV_TARGET
{
	// Grey so the VR hidden area mesh doesn't influence the adaptive luminance
	return float4(0.5f, 0.5f, 0.5f, 0.0f);
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else
#endif
