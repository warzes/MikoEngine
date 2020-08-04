#pragma once

struct ApplicationSetting
{
    bool        resizable    = true;
    bool        maximized    = false;
    bool        vsync        = true;
    int         refresh_rate = 60;
    int         width        = 800;
    int         height       = 600;
    std::string title        = "Miko Engine";

#if SE_VULKAN
    bool ray_tracing = false;
#else
    bool enable_debug_callback = false;
#endif
};

