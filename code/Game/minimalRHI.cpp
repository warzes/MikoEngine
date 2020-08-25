#include "minimalRHI.h"

bool minimalRHI::init(int argc, const char * argv[])
{
	return true;
}

void minimalRHI::update(double delta)
{
}

void minimalRHI::shutdown()
{
}

ApplicationSetting minimalRHI::intial_app_settings()
{
	ApplicationSetting settings;
	settings.width = 1280;
	settings.height = 720;
	settings.title = "minimalRHI";
	settings.rhiApi = RHIApi::Direct3D11;
	return settings;
}

void minimalRHI::window_resized(int width, int height)
{
}

void minimalRHI::fillCommandBuffer()
{
}