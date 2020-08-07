#include "MyApp.h"

bool MyApp::init(int argc, const char * argv[])
{
	return true;
}

void MyApp::update(double delta)
{
}

void MyApp::shutdown()
{
}

ApplicationSetting MyApp::intial_app_settings()
{
    ApplicationSetting settings;
    settings.width = 1280;
    settings.height = 720;
    settings.title = "Hello Game";
    return settings;
}

void MyApp::window_resized(int width, int height)
{
}