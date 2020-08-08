#ifdef RHI_NULL
	if (rhi->getNameId() == Rhi::NameId::NULL_DUMMY)
	{
		vertexShaderSourceCode = fragmentShaderSourceCode = "42";
	}
	else
#endif
{
	// Error! (unsupported RHI)
	RHI_LOG(rhi->getContext(), CRITICAL, "Error: Unsupported RHI")
}
