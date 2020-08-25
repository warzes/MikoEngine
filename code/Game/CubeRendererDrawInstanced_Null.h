#if SE_RHINULL
	if (mRhi->getNameId() == Rhi::NameId::NULL_DUMMY)
	{
		vertexShaderSourceCode = fragmentShaderSourceCode = "42";
	}
	else
#endif
{
	// Error! (unsupported RHI)
	RHI_LOG(CRITICAL, "Error: Unsupported RHI")
}
