#include "MyApp.h"
//-----------------------------------------------------------------------------
#pragma comment(lib, "3rdparty.lib")
#pragma comment(lib, "Engine.lib")
#pragma comment(lib, "MikoEngine.lib")
#pragma comment(lib, "vulkan-1.lib")
//-----------------------------------------------------------------------------
int main(
	[[maybe_unused]] int argc,
	[[maybe_unused]] const char *argv[])
{
	try
	{
		MyApp app;
		return app.Run(argc, argv);
	}
	catch ( const std::exception &e )
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
//-----------------------------------------------------------------------------