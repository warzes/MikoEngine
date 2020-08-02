#pragma once

struct ApplicationSetting
{
    bool        resizable = true;
    bool        maximized = false;
    int         refresh_rate = 60;
    int         width = 800;
    int         height = 600;
    std::string title = "dwSampleFramwork";
};