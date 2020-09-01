#if SE_DIRECT3D11 || SE_DIRECT3D12
if (rhi->getNameId() == Rhi::NameId::DIRECT3D11 || rhi->getNameId() == Rhi::NameId::DIRECT3D12)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(
// Attribute input/output
struct VS_OUTPUT
{
	// Direct3D/driver says *crash* if we try to write 'void main()'
};

// Programs
VS_OUTPUT main()
{
	VS_OUTPUT output;
	return output;
}
)";


//[-------------------------------------------------------]
//[ Geometry shader source code                           ]
//[-------------------------------------------------------]
// One geometry shader invocation per primitive
geometryShaderSourceCode = R"(
// Attribute input/output
struct GS_INPUT
{
	// Direct3D says: 'error X3514: 'main': no input primitive specified, if your shader doesn't require inputs, then define an empty struct and give it the proper primitive type.'
};
struct GS_OUTPUT
{
	float4 Position : SV_POSITION;
};

[maxvertexcount(3)]
void main(point GS_INPUT input[1], inout TriangleStream<GS_OUTPUT> OutputStream)
{
	GS_OUTPUT output;

	//				Vertex ID	Triangle on screen
	//  0.0f, 1.0f,	// 0			0
	//  1.0f, 0.0f,	// 1		   .   .
	// -0.5f, 0.0f	// 2		  2.......1

	// Emit vertex 0 clip space position, left/bottom is (-1,-1) and right/top is (1,1)
	output.Position = float4(0.0f, 1.0f, 0.5f, 1.0f);
	OutputStream.Append(output);

	// Emit vertex 1 clip space position, left/bottom is (-1,-1) and right/top is (1,1)
	output.Position = float4(1.0f, 0.0f, 0.5f, 1.0f);
	OutputStream.Append(output);

	// Emit vertex 2 clip space position, left/bottom is (-1,-1) and right/top is (1,1)
	output.Position= float4(-0.5f, 0.0f, 0.5f, 1.0f);
	OutputStream.Append(output);

	// Done
	OutputStream.RestartStrip();
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
	// Return white
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else
#endif
