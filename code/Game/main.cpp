#include "Triangle.h"
#include "IndirectBuffer.h"
#include "Queries.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "CubeTexture.h"
#include "InstancedCubes.h"
#include "Mesh.h"
#include "Scene.h"
//-----------------------------------------------------------------------------
#pragma comment(lib, "3rdparty.lib")
#pragma comment(lib, "MikoEngine.lib")
//-----------------------------------------------------------------------------
int main(
	[[maybe_unused]] int argc,
	[[maybe_unused]] const char *argv[])
{
	try
	{
		Texture app;
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