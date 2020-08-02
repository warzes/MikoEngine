#pragma once

#include "ApplicationSettings.h"
#include "Utility/Timer.h"

class Application
{
public:
	Application() = default;
	virtual ~Application();

	int Run(int argc, const char* argv[]);

protected:
private:
};