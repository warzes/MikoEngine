#include "minimalRenderer.h"

bool minimalRenderer::init(int argc, const char * argv[])
{
	return true;
}

void minimalRenderer::update(double delta)
{
}

void minimalRenderer::shutdown()
{
}

ApplicationSetting minimalRenderer::intial_app_settings()
{
	ApplicationSetting settings;
	settings.width = 1280;
	settings.height = 720;
	settings.title = "minimalRHI";
	settings.rhiApi = RHIApi::Direct3D11;
	return settings;
}

void minimalRenderer::window_resized(int width, int height)
{
}

void minimalRenderer::fillCommandBuffer()
{
}