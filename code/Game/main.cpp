#include "Engine/stdafx.h"
#include "Engine/Engine.h"
//-----------------------------------------------------------------------------
#pragma comment(lib, "3rdparty.lib")
#pragma comment(lib, "Engine.lib")
#pragma comment(lib, "vulkan-1.lib")
//-----------------------------------------------------------------------------
int main(
	[[maybe_unused]] int argc,
	[[maybe_unused]] char *argv[])
{
	try
	{
		Engine engine;
		engine.Run();
	}
	catch ( const std::exception &e )
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
//-----------------------------------------------------------------------------