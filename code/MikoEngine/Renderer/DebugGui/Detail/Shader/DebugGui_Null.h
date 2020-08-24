#if SE_RHINULL
	if (rhi.getNameId() == Rhi::NameId::NULL_DUMMY)
	{
		vertexShaderSourceCode = fragmentShaderSourceCode = "42";
	}
	else
#endif
{
	// Error! (unsupported RHI)
	RHI_LOG(CRITICAL, "The RHI implementation \"%s\" isn't supported by the renderer debug GUI", rhi.getName())
}
