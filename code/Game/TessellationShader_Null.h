#if SE_RHINULL
	if (rhi->getNameId() == Rhi::NameId::NULL_DUMMY)
	{
		vertexShaderSourceCode = fragmentShaderSourceCode = "42";
	}
	else
#endif
{
	// Error! (unsupported RHI)
	RHI_LOG(CRITICAL, "Error: Unsupported RHI");
}
