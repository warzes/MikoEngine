#pragma once

enum class RHIApi
{
	Vulkan,
	OpenGL,
	OpenGLES,
	Direct3D11,
	Direct3D12,
	Null
};

struct ApplicationSetting
{
    bool        resizable    = true;
    bool        maximized    = false;
    bool        vsync        = true;
    int         refresh_rate = 60;
    int         width        = 800;
    int         height       = 600;
    std::string title        = "Miko Engine";
    RHIApi rhiApi            = RHIApi::OpenGL;
};

